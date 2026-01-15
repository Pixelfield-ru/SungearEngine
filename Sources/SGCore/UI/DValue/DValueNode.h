#pragma once
#include <functional>
#include <unordered_set>
#include <vector>

#include "DValueNode.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI::DValue
{
    template <typename In>
    struct DValueNodeInput;

    /**
     * Node output, can be linked to multiple @ref DValueNodeInput
     * @tparam Out Value type
     */
    template<typename Out>
    struct DValueNodeOutput
    {
        friend DValueNodeInput<Out>;

        copy_constructor(DValueNodeOutput) = default;
        move_constructor(DValueNodeOutput) = default;

        virtual ~DValueNodeOutput() noexcept;

        void connectNext(DValueNodeInput<Out>* next) noexcept;
        void disconnectNext(DValueNodeInput<Out>* next) noexcept;

        void setValue(Out value) noexcept;

    private:
        std::unordered_set<DValueNodeInput<Out>*> m_nextNodes {};
    };

    /**
     * Node input, that can be connected to @ref DValueNodeOutput
     * @tparam In Value type
     */
    template <typename In>
    struct DValueNodeInput final
    {
        friend DValueNodeOutput<In>;

        // Using std::function instead of signal for optimization
        std::function<void(In value)> onReceived = []{};

        copy_constructor(DValueNodeInput) = default;
        move_constructor(DValueNodeInput) = default;

        explicit DValueNodeInput(decltype(onReceived) onReceived) noexcept : onReceived(onReceived) {};

        ~DValueNodeInput() noexcept = default;

        void connectPrevious(DValueNodeOutput<In>* previous) noexcept;
        void disconnectPrevious() noexcept;

    private:
        DValueNodeOutput<In>* m_previousNode = nullptr;
    };

    template<typename In, typename Out>
    struct DValueNodeValueConvertor : virtual DValueNodeOutput<Out>
    {
        move_constructor(DValueNodeValueConvertor) = default;
        copy_constructor(DValueNodeValueConvertor) = default;

        ~DValueNodeValueConvertor() noexcept override = default;

        DValueNodeInput<In> m_input {[this](In val){ this->setValue(convertPassedValue(val)); }};

        virtual In convertPassedValue(Out value) noexcept = 0;
    };

    /**
     * Passes value to next connected nodes when received
     * @tparam T Value type
     */
    template<typename T>
    struct DValueNode final : virtual DValueNodeValueConvertor<T, T>
    {
        copy_constructor(DValueNode) = default;
        move_constructor(DValueNode) = default;

        ~DValueNode() override = default;

        T convertPassedValue(T value) override {
            return value;
        }
    };
}

template <typename Out>
SGCore::UI::DValue::DValueNodeOutput<Out>::~DValueNodeOutput() noexcept {
    for (auto node : m_nextNodes) {
        node->disconnectPrevious(this);
    }

    m_nextNodes.clear();
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::connectNext(DValueNodeInput<Out>* next)noexcept {
    if (next->m_previousNode == this) {return;}

    if (next->m_previousNode != nullptr) {
        next->disconnectPrevious(this);
    }

    m_nextNodes.insert(next);
    next->m_previousNode = this;
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::disconnectNext(DValueNodeInput<Out>* next) noexcept {
    if (m_nextNodes.erase(next)) {
        next->m_previousNode = nullptr;
    }
}

template <typename Out>
void SGCore::UI::DValue::DValueNodeOutput<Out>::setValue(Out value) noexcept {
    for (auto node : m_nextNodes) {
        node->onReceived(value);
    }
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::connectPrevious(DValueNodeOutput<In>* previous) noexcept {
    previous->connectNext(this);
}

template <typename In>
void SGCore::UI::DValue::DValueNodeInput<In>::disconnectPrevious() noexcept {
    m_previousNode->disconnectPrevious(this);
}
