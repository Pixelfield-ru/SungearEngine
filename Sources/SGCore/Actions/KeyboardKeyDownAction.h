//
// Created by stuka on 14.01.2025.
//

#pragma once

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
    * Action to check whether keyboard key is down at the moment.
    */
    struct SGCORE_EXPORT KeyboardKeyDownAction : IAction<bool()>
    {
        /**
         * Keyboard key to check state.
         */
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        /**
         * @return If keyboard key is down at the moment.
         */
        bool execute() noexcept final
        {
            return Input::PC::keyboardKeyDown(m_key);
        }

        /**
         * @return New KeyboardKeyDownAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<KeyboardKeyDownAction>();
            newAction->m_key = m_key;

            return newAction;
        }
    };
}
