//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H
#define SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct MouseButtonDownAction : IAction<bool()>
    {
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        bool execute() noexcept final
        {
            return Input::PC::mouseButtonDown(m_button);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonDownAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}

#endif // SUNGEARENGINE_MOUSEBUTTONDOWNACTION_H
