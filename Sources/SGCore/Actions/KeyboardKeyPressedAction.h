//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H
#define SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct KeyboardKeyPressedAction : IAction<bool()>
    {
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        bool execute() noexcept final
        {
            return Input::PC::keyboardKeyPressed(m_key);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<KeyboardKeyPressedAction>();
            newAction->m_key = m_key;

            return newAction;
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H
