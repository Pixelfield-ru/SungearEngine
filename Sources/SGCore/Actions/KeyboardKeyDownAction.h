//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H
#define SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H

#include "IAction.h"
#include "SGCore/Input/InputManager.h"

namespace SGCore
{
    struct KeyboardKeyDownAction : IAction<bool>
    {
        KeyboardKey m_key = KeyboardKey::KEY_FIRST;

        bool execute() noexcept final
        {
            return InputManager::getMainInputListener()->keyboardKeyDown(m_key);
        }

        Ref<IAction<bool>> copy() noexcept final
        {
            return MakeRef<KeyboardKeyDownAction>();
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H
