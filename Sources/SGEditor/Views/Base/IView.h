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

namespace SGEditor
{
    struct IView
    {
        std::shared_ptr<SGCore::Event<void()>> m_onRenderEvent = std::make_shared<SGCore::Event<void()>>();

        std::vector<std::shared_ptr<IView>> m_subViews;

        std::string m_name = UUID::generateNew();

        virtual void render() = 0;
        virtual void renderSubViews() const noexcept;
    };
}

#endif //SUNGEARENGINE_IVIEW_H
