//
// Created by stuka on 18.05.2025.
//

#ifndef SUNGEARENGINE_CORO_TASK_H
#define SUNGEARENGINE_CORO_TASK_H

#include <coroutine>
#include <exception>
#include <iostream>
#include <functional>

#include "CoroScheduler.h"

#include "CoroUtils.h"
#include "SGCore/Logger/Logger.h"

namespace SGCore::Coro
{
    template<typename>
    struct Task;

    template<typename T, template<typename> typename PromiseT>
    struct PromiseBase
    {
        template<typename>
        friend struct Task;

        using value_type = T;
        using task_type = Task<T>;
        using promise_type = PromiseT<T>;

        PromiseBase() = default;

        std::suspend_never initial_suspend() noexcept { return { }; }

        // using suspend always to handle coroutine state and manually destroy it
        // if it will be std::suspend_never then i cant understand if coroutine is done
        std::suspend_always final_suspend() noexcept { return { }; }

        void unhandled_exception()
        {
            try
            {
                std::rethrow_exception(std::current_exception());
            }
            catch(const std::exception& e)
            {
                const std::string msg = fmt::format("SGCore::Coro : caught exception: {}", e.what());
                LOG_E_UNFORMATTED(SGCORE_TAG, msg);
                std::cerr << msg << std::endl;
            }
        }

        task_type get_return_object() noexcept
        {
            return task_type { std::coroutine_handle<promise_type>::from_promise(*static_cast<promise_type*>(this)) };
        }
    };

    template<typename T>
    struct ValuePromise : PromiseBase<T, ValuePromise>
    {
        template<typename>
        friend struct Task;

        using typename PromiseBase<T, ValuePromise>::task_type;

        std::suspend_always yield_value(T value)
        {
            this->m_value = std::move(value);

            if(onStep)
            {
                onStep(*this->m_value);
            }

            CoroUtils::assumeCurrentThread()->m_coroScheduler.addYieldedCoro(std::coroutine_handle<ValuePromise>::from_promise(*static_cast<ValuePromise*>(this)));

            return { };
        }

        void return_value(T value)
        {
            this->m_value = std::move(value);

            if(onStep) { onStep(*this->m_value); }

            if(this->onDone)
            {
                this->onDone(*this->m_value);
            }
        }

        T get_value()
        {
            // m_value is always not empty, in another case program is ill formed
            return *m_value;
        }

    private:
        std::optional<T> m_value = std::nullopt;

        std::function<void(const T& nextResult)> onStep;
        std::function<void(const T& coroResult)> onDone;
    };

    template<typename T>
    struct VoidPromise : PromiseBase<T, VoidPromise>
    {
        template<typename>
        friend struct Task;

        void return_void()
        {
            if(this->onDone)
            {
                this->onDone();
            }
        }

    private:
        std::function<void()> onDone;
    };

    template<typename T = void>
    struct Task
    {
        using promise_type = std::conditional_t<std::is_same_v<T, void>, VoidPromise<T>, ValuePromise<T>>;
        using value_type = T;

        ~Task()
        {
            if(m_nativeCoroHandle && m_nativeCoroHandle.done())
            {
                destroyHandle();
            }
        }

        Task() = default;

        Task(const Task&) = delete;

        Task(Task&& other) noexcept
        {
            m_nativeCoroHandle = other.m_nativeCoroHandle;
            m_isRunning = other.m_isRunning;

            other.m_nativeCoroHandle = nullptr;
            other.m_isRunning = false;
        }

        explicit Task(std::coroutine_handle<promise_type> coroHandle) : m_nativeCoroHandle(std::move(coroHandle))
        {
        }

        Task&& run()
        {
            if(!m_nativeCoroHandle || m_nativeCoroHandle.done() || m_isRunning) return std::move(*this);

            m_nativeCoroHandle.resume();
            m_isRunning = true;

            return std::move(*this);
        }

        template<typename FuncT>
        Task&& whenStep(FuncT&& func) noexcept
        {
            if(!m_nativeCoroHandle) return std::move(*this);

            auto& promise = m_nativeCoroHandle.promise();
            promise.onStep = std::forward<FuncT>(func);

            if(promise.onStep && promise.m_value.has_value())
            {
                promise.onStep(*promise.m_value);
            }

            return std::move(*this);
        }

        template<typename FuncT>
        Task&& whenDone(FuncT&& func) noexcept
        {
            if(!m_nativeCoroHandle) return std::move(*this);

            auto& promise = m_nativeCoroHandle.promise();

            promise.onDone = std::forward<FuncT>(func);

            if(m_nativeCoroHandle.done() && promise.onDone)
            {
                if constexpr(std::is_void_v<T>)
                {
                    promise.onDone();
                }
                else
                {
                    promise.onDone(*promise.m_value);
                }
            }

            return std::move(*this);
        }

        Task& operator=(const Task&) = delete;

        Task& operator=(Task&& other) noexcept
        {
            if(std::addressof(other) == this) return *this;

            m_nativeCoroHandle = other.m_nativeCoroHandle;
            m_isRunning = other.m_isRunning;

            other.m_nativeCoroHandle = nullptr;
            other.m_isRunning = false;

            return *this;
        }

        std::coroutine_handle<promise_type> getHandle() const noexcept { return m_nativeCoroHandle; }

    private:
        void destroyHandle() noexcept
        {
            if(m_nativeCoroHandle)
            {
                m_nativeCoroHandle.destroy();
                m_nativeCoroHandle = nullptr;
            }
        }

        std::coroutine_handle<promise_type> m_nativeCoroHandle = nullptr;

        bool m_isRunning = true;
    };
}

#endif // SUNGEARENGINE_CORO_TASK_H
