//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H
#define SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct KeyboardKeyReleasedAction : IAction<bool()>
    {
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        bool execute() noexcept final
        {
            return Input::PC::keyboardKeyReleased(m_key);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<KeyboardKeyDownAction>();
            newAction->m_key = m_key;

            return newAction;
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H
