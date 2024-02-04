//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_IVIEW_H
#define SUNGEARENGINE_IVIEW_H

#include <vector>
#include <string>
#include <memory>

#include "SGUtils/UUID.h"
#include "SGUtils/Event.h"

namespace SGCore::ImGuiWrap
{
    struct IView : public std::enable_shared_from_this<IView>
    {
        friend struct ViewsInjector;

        //IView() noexcept;

        bool m_active = true;

        Event<void()> m_onRenderEvent = MakeEvent<void()>();

        std::string m_name = SGUtils::UUID::generateNew();

        virtual bool begin() = 0;
        virtual void renderBody() = 0;
        virtual void end() = 0;

        [[nodiscard]] std::string getUniquePathPart() const noexcept;

        virtual void inject() noexcept;

    private:
        // you may use pointer to string
        std::string m_uniquePathPart;

        virtual void updateUniquePathPart(const std::string& uniquePathPart) const noexcept { }
    };
}

#endif //SUNGEARENGINE_IVIEW_H
