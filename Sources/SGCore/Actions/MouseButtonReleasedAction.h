//
// Created by stuka on 29.05.2025.
//

#pragma once

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
    * Action to check whether mouse button is released.
    */
    struct MouseButtonReleasedAction : IAction<bool()>
    {
        /**
         * Mouse button to check state.
         */
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        /**
         * @return If mouse button is released.
         */
        bool execute() noexcept final
        {
            return Input::PC::mouseButtonReleased(m_button);
        }

        /**
         * @return New MouseButtonReleasedAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonReleasedAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}
