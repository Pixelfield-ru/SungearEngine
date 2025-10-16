//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H
#define SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct KeyboardKeyDownAction : IAction<bool()>
    {
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        bool execute() noexcept final
        {
            return Input::PC::keyboardKeyDown(m_key);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<KeyboardKeyDownAction>();
            newAction->m_key = m_key;

            return newAction;
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYDOWNACTION_H
