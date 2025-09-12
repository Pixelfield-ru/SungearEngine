//
// Created by stuka on 22.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Actions/KeyboardKeyAction.h"

namespace SGCore
{
    struct KeyboardKeyReleasedAction;
    struct KeyboardKeyPressedAction;
    struct KeyboardKeyDownAction;
    struct AlwaysFalseAction;
    struct AlwaysTrueAction;
}

namespace SGCore::Serde
{
    // ======================================================== IAction FWD

    template<typename ActionReturnT, typename... ExecuteArgs, FormatType TFormatType>
    struct SerdeSpec<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<
                AlwaysTrueAction,
                AlwaysFalseAction,
                KeyboardKeyAction,
                KeyboardKeyDownAction,
                KeyboardKeyPressedAction,
                KeyboardKeyReleasedAction
            >
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::IAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AlwaysTrueAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AlwaysTrueAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::AlwaysTrueAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AlwaysTrueAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AlwaysTrueAction, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AlwaysFalseAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AlwaysFalseAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::AlwaysFalseAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AlwaysFalseAction, TFormatType>& valueView)  noexcept;

        static void deserialize(DeserializableValueView<AlwaysFalseAction, TFormatType>& valueView) noexcept;
    };

    // ======================================================== KeyboardKeyAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyAction, TFormatType> :
            BaseTypes<IAction<KeyboardKeyActionType()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::KeyboardKeyAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<KeyboardKeyAction, TFormatType>& valueView) noexcept;
    };

    // ======================================================== KeyboardKeyDownAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyDownAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::KeyboardKeyDownAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyDownAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<KeyboardKeyDownAction, TFormatType>& valueView) noexcept;
    };

    // ======================================================== KeyboardKeyPressedAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyPressedAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::KeyboardKeyPressedAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyPressedAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<KeyboardKeyPressedAction, TFormatType>& valueView) noexcept;
    };

    // ======================================================== KeyboardKeyReleasedAction FWD

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyReleasedAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::KeyboardKeyReleasedAction")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept;
    };
}
