//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_VIEWSMANAGER_H
#define SUNGEARENGINE_VIEWSMANAGER_H

#include "SGEditor/Views/Base/IView.h"

namespace SGEditor
{
    struct ViewsManager
    {
        std::list<std::shared_ptr<IView>> m_rootViews;

        void renderRootViews() const noexcept;
    };
}

#endif //SUNGEARENGINE_VIEWSMANAGER_H
