//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H
#define SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H

#include "IAction.h"
#include "SGCore/Input/InputManager.h"

namespace SGCore
{
    struct KeyboardKeyPressedAction : IAction<bool>
    {
        KeyboardKey m_key = KeyboardKey::KEY_FIRST;

        bool execute() noexcept final
        {
            return InputManager::getMainInputListener()->keyboardKeyPressed(m_key);
        }

        Ref<IAction<bool>> copy() noexcept final
        {
            return MakeRef<KeyboardKeyPressedAction>();
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYPRESSEDACTION_H
