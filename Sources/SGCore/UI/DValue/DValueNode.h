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

        copy_constructor(DValueNodeOutput);
        move_constructor(DValueNodeOutput);

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

        copy_constructor(DValueNodeInput);
        move_constructor(DValueNodeInput);

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
        move_constructor(DValueNodeValueConvertor);
        copy_constructor(DValueNodeValueConvertor);

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
        copy_constructor(DValueNode);
        move_constructor(DValueNode);

        ~DValueNode() override = default;

        T convertPassedValue(T value) override {
            return value;
        }
    };
}
