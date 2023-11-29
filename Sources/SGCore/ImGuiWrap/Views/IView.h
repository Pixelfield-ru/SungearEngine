//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_IVIEW_H
#define SUNGEARENGINE_IVIEW_H

#include <vector>
#include <string>
#include <memory>

#include "SGCore/Patterns/UUID.h"
#include "SGCore/Patterns/Event.h"

namespace SGCore::ImGuiWrap
{
    struct IView
    {
        bool m_active = true;

        std::shared_ptr<Event<void()>> m_onRenderEvent = std::make_shared<Event<void()>>();

        std::string m_name = UUID::generateNew();

        virtual bool begin() = 0;
        virtual void renderBody() = 0;
        virtual void end() = 0;
    };
}

#endif //SUNGEARENGINE_IVIEW_H
