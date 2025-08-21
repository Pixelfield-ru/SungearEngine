//
// Created by stuka on 22.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Actions/AlwaysFalseAction.h"
#include "SGCore/Actions/AlwaysTrueAction.h"
#include "SGCore/Actions/KeyboardKeyAction.h"
#include "SGCore/Actions/KeyboardKeyDownAction.h"
#include "SGCore/Actions/KeyboardKeyPressedAction.h"
#include "SGCore/Actions/KeyboardKeyReleasedAction.h"

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
        static inline const std::string type_name = "SGCore::IAction";
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
        static inline const std::string type_name = "SGCore::AlwaysTrueAction";
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
        static inline const std::string type_name = "SGCore::AlwaysFalseAction";
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
        static inline const std::string type_name = "SGCore::KeyboardKeyAction";
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
        static inline const std::string type_name = "SGCore::KeyboardKeyDownAction";
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
        static inline const std::string type_name = "SGCore::KeyboardKeyPressedAction";
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
        static inline const std::string type_name = "SGCore::KeyboardKeyReleasedAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept;
    };
}