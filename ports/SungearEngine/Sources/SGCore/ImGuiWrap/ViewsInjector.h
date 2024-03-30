//
// Created by stuka on 30.11.2023.
//

#ifndef SUNGEARENGINE_VIEWSINJECTOR_H
#define SUNGEARENGINE_VIEWSINJECTOR_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <sgcore_export.h>

#include "Views/IView.h"
#include "SGUtils/Singleton.h"

namespace SGCore::ImGuiWrap
{
    struct SGCORE_EXPORT ViewsInjector
    {
        // in those children other views can not be injected
        std::vector<std::weak_ptr<IView>> m_childrenViews;
        std::weak_ptr<IView> m_rootView;
        // in those children injector other views can be injected
        std::unordered_map<std::string, std::shared_ptr<ViewsInjector>> m_childrenInjectors;
        
        ViewsInjector& operator[](const std::string& viewName) noexcept;
        
        void renderViews();

    private:

        SG_FULL_SINGLETON(ViewsInjector)
    };
}

#endif //SUNGEARENGINE_VIEWSINJECTOR_H
