//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H
#define SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct MouseButtonPressedAction : IAction<bool()>
    {
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        bool execute() noexcept final
        {
            return Input::PC::mouseButtonPressed(m_button);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonPressedAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}

#endif // SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H
