//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H
#define SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    /**
    * Action to check whether mouse button is down at the moment.
    */
    struct MouseButtonDownAction : IAction<bool()>
    {
        /**
         * Mouse button to check state.
         */
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        /**
         * @return If mouse button is down at the moment.
         */
        bool execute() noexcept final
        {
            return Input::PC::mouseButtonDown(m_button);
        }

        /**
         * @return New MouseButtonDownAction instance.
         */
        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonDownAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}

#endif // SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H
