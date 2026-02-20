//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H
#define SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
    * Action to check whether mouse button is pressed once.
    */
    struct MouseButtonPressedAction : IAction<bool()>
    {
        /**
         * Mouse button to check state.
         */
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        /**
         * @return If mouse button is pressed once.
         */
        bool execute() noexcept final
        {
            return Input::PC::mouseButtonPressed(m_button);
        }

        /**
         * @return New MouseButtonPressedAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonPressedAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}

#endif // SUNGEARENGINE_MOUSEBUTTONPRESSEDACTION_H
