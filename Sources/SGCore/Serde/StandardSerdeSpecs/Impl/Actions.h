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
    // =============================================== impl IAction

    template<typename ActionReturnT, typename... ExecuteArgs, FormatType TFormatType>
    void SerdeSpec<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>::serialize(SerializableValueView<const IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView) noexcept
    {
    }

    template<typename ActionReturnT, typename... ExecuteArgs, FormatType TFormatType>
    void SerdeSpec<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>::deserialize(DeserializableValueView<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl AlwaysTrueAction

    template<FormatType TFormatType>
    void SerdeSpec<AlwaysTrueAction, TFormatType>::serialize(SerializableValueView<const AlwaysTrueAction, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<AlwaysTrueAction, TFormatType>::deserialize(DeserializableValueView<AlwaysTrueAction, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl AlwaysFalseAction

    template<FormatType TFormatType>
    void SerdeSpec<AlwaysFalseAction, TFormatType>::serialize(SerializableValueView<const AlwaysFalseAction, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<AlwaysFalseAction, TFormatType>::deserialize(DeserializableValueView<AlwaysFalseAction, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl KeyboardKeyAction

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyAction, TFormatType>::serialize(SerializableValueView<const KeyboardKeyAction, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
    }

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyAction, TFormatType>::deserialize(DeserializableValueView<KeyboardKeyAction, TFormatType>& valueView) noexcept
    {
        auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
        if(key)
        {
            valueView.m_data->m_key = std::move(*key);
        }
    }

    // =============================================== impl KeyboardKeyDownAction

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyDownAction, TFormatType>::serialize(SerializableValueView<const KeyboardKeyDownAction, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
    }

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyDownAction, TFormatType>::deserialize(DeserializableValueView<KeyboardKeyDownAction, TFormatType>& valueView) noexcept
    {
        auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
        if(key)
        {
            valueView.m_data->m_key = std::move(*key);
        }
    }

    // =============================================== impl KeyboardKeyPressedAction

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyPressedAction, TFormatType>::serialize(SerializableValueView<const KeyboardKeyPressedAction, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
    }

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyPressedAction, TFormatType>::deserialize(DeserializableValueView<KeyboardKeyPressedAction, TFormatType>& valueView) noexcept
    {
        auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
        if(key)
        {
            valueView.m_data->m_key = std::move(*key);
        }
    }

    // =============================================== impl KeyboardKeyReleasedAction

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyReleasedAction, TFormatType>::serialize(SerializableValueView<const KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
    }

    template<FormatType TFormatType>
    void SerdeSpec<KeyboardKeyReleasedAction, TFormatType>::deserialize(DeserializableValueView<KeyboardKeyReleasedAction, TFormatType>& valueView) noexcept
    {
        auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
        if(key)
        {
            valueView.m_data->m_key = std::move(*key);
        }
    }
}