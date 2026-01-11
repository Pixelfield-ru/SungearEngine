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

        virtual ~DValueNodeOutput();

        void connectNext(DValueNodeInput<Out>* next);
        void disconnectNext(DValueNodeInput<Out>* next);

        void setValue(Out value);

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

        explicit DValueNodeInput(decltype(onReceived) onReceived) : onReceived(onReceived) {};

        ~DValueNodeInput() = default;

        void connectPrevious(DValueNodeOutput<In>* previous);
        void disconnectPrevious();

    private:
        DValueNodeOutput<In>* m_previousNode = nullptr;
    };

    template<typename In, typename Out>
    struct DValueNodeValueConvertor : virtual DValueNodeOutput<Out>
    {
        move_constructor(DValueNodeValueConvertor);
        copy_constructor(DValueNodeValueConvertor);

        ~DValueNodeValueConvertor() override = default;

        DValueNodeInput<In> m_input {[this](In val){ this->setValue(convertPassedValue(val)); }};

        virtual In convertPassedValue(Out value) = 0;
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
