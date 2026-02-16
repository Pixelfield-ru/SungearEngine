//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_MOUSEBUTTONRELEASEDACTION_H
#define SUNGEARENGINE_MOUSEBUTTONRELEASEDACTION_H

#include "IAction.h"
#include "SGCore/Input/PCInput.h"

namespace SGCore
{
    struct MouseButtonReleasedAction : IAction<bool()>
    {
        Input::MouseButton m_button = Input::MouseButton::MOUSE_BUTTON_1;

        bool execute() noexcept final
        {
            return Input::PC::mouseButtonReleased(m_button);
        }

        Ref<IAction> copy() noexcept final
        {
            auto newAction = MakeRef<MouseButtonReleasedAction>();
            newAction->m_button = m_button;

            return newAction;
        }
    };
}

#endif // SUNGEARENGINE_MOUSEBUTTONRELEASEDACTION_H
