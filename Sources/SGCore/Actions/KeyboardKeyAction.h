//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_KEYBOARDKEYACTION_H
#define SUNGEARENGINE_KEYBOARDKEYACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    enum class KeyboardKeyActionType
    {
        KA_DOWN,
        KA_RELEASED,
        KA_PRESSED,
        KA_NONE
    };

    struct KeyboardKeyAction : IAction<KeyboardKeyActionType()>
    {
        Input::KeyboardKey m_key = Input::KeyboardKey::KEY_FIRST;

        KeyboardKeyActionType execute() noexcept final
        {
            if(Input::PC::keyboardKeyDown(m_key))
            {
                return KeyboardKeyActionType::KA_DOWN;
            }
            else if(Input::PC::keyboardKeyReleased(m_key))
            {
                return KeyboardKeyActionType::KA_RELEASED;
            }
            else if(Input::PC::keyboardKeyPressed(m_key))
            {
                return KeyboardKeyActionType::KA_PRESSED;
            }

            return KeyboardKeyActionType::KA_NONE;
        }

        Ref<IAction> copy() noexcept final
        {
            return MakeRef<KeyboardKeyAction>();
        }
    };
}

#endif //SUNGEARENGINE_KEYBOARDKEYACTION_H
