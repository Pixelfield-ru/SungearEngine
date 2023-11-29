//
// Created by stuka on 30.11.2023.
//

#ifndef SUNGEARENGINE_VIEWSINJECTOR_H
#define SUNGEARENGINE_VIEWSINJECTOR_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "Views/IView.h"
#include "SGCore/Patterns/Singleton.h"

namespace SGCore::ImGuiWrap
{
    struct ViewsInjector
    {
        // in those children other views can not be injected
        std::vector<std::weak_ptr<IView>> m_childrenViews;
        std::weak_ptr<IView> m_rootView;

        ViewsInjector& operator[](const std::string& viewName) noexcept;

        void renderViews();

    private:
        // in those children injector other views can be injected
        std::unordered_map<std::string, std::shared_ptr<ViewsInjector>> m_childrenInjectors;

        SG_FULL_SINGLETON(ViewsInjector)
    };
}

#endif //SUNGEARENGINE_VIEWSINJECTOR_H
