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

namespace SGCore::Coro
{
    template<typename>
    struct Task;

    template<typename T, template<typename> typename PromiseT>
    struct PromiseBase
    {
        using value_type = T;
        using task_type = Task<T>;
        using promise_type = PromiseT<T>;

        PromiseBase() = default;

        std::suspend_always initial_suspend() noexcept { return { }; }

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
                std::cout << e.what() << '\n';
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
                onStep(this->m_value);
            }

            CoroScheduler::addYieldedCoro(std::coroutine_handle<ValuePromise>::from_promise(*static_cast<ValuePromise*>(this)));

            return { };
        }

        void return_value(T value)
        {
            this->m_value = std::move(value);

            if(onStep) { onStep(this->m_value); }

            if(onDone) { onDone(); }
        }

        T get_value() { return m_value; }

    private:
        T m_value;

        std::function<void(T nextResult)> onStep;
        std::function<void()> onDone;
    };

    template<typename T>
    struct VoidPromise : PromiseBase<T, VoidPromise>
    {
        void return_void()
        {
        }
    };

    template<typename T = void>
    struct Task
    {
        using promise_type = std::conditional_t<std::is_same_v<T, void>, VoidPromise<T>, ValuePromise<T>>;
        using value_type = T;

        ~Task() { if(m_nativeCoroHandle && m_nativeCoroHandle.done()) { destroyHandle(); } }

        Task() = default;

        Task(const Task&) = default;

        Task(Task&&) = default;

        explicit Task(std::coroutine_handle<promise_type> coroHandle) : m_nativeCoroHandle(std::move(coroHandle))
        {
        }

        Task& run()
        {
            if(!m_nativeCoroHandle || m_nativeCoroHandle.done() || m_isRunning) return *this;

            m_nativeCoroHandle.resume();
            m_isRunning = true;

            return *this;
        }

        Task& whenStep(auto&& func) noexcept
        {
            m_nativeCoroHandle.promise().onStep = std::forward<decltype(func)>(func);

            return *this;
        }

        Task& whenDone(auto&& func) noexcept
        {
            m_nativeCoroHandle.promise().onDone = std::forward<decltype(func)>(func);

            return *this;
        }

        Task& operator=(const Task&) = default;

        Task& operator=(Task&&) = default;

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

        bool m_isRunning = false;
    };
}

#endif // SUNGEARENGINE_CORO_TASK_H
