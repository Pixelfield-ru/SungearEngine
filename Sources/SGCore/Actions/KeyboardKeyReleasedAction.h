//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H
#define SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
    * Action to check whether keyboard key is released.
    */
    struct KeyboardKeyReleasedAction : IAction<bool()>
    {
        /**
         * Keyboard key to check state.
         */
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        /**
         * @return If keyboard key is released.
         */
        bool execute() noexcept final
        {
            return Input::PC::keyboardKeyReleased(m_key);
        }

        /**
         * @return New KeyboardKeyReleasedAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<KeyboardKeyReleasedAction>();
            newAction->m_key = m_key;

            return newAction;
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYRELEASEDACTION_H
