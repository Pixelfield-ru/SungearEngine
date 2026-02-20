//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYACTION_H
#define SUNGEARENGINE_KEYBOARDKEYACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
     * Action to check keyboard key state.
     */
    struct KeyboardKeyAction : IAction<Input::KeyState()>
    {
        /**
         * Keyboard key to check state.
         */
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        /**
         * @return State of key (SGCore::Input::KeyState::KS_DOWN, KS_RELEASE, KS_PRESSED or KS_NONE if no action was performed with key).
         */
        Input::KeyState execute() noexcept final
        {
            if(Input::PC::keyboardKeyDown(m_key))
            {
                return Input::KeyState::KS_DOWN;
            }
            if(Input::PC::keyboardKeyReleased(m_key))
            {
                return Input::KeyState::KS_RELEASE;
            }
            if(Input::PC::keyboardKeyPressed(m_key))
            {
                return Input::KeyState::KS_PRESSED;
            }

            return Input::KeyState::KS_NONE;
        }

        /**
         * @return New KeyboardKeyAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto instance = MakeRef<KeyboardKeyAction>();
            instance->m_key = m_key;

            return instance;
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYACTION_H
