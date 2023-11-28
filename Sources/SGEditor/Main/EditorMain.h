//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EDITORMAIN_H
#define SUNGEARENGINE_EDITORMAIN_H

#include "SGEditor/Views/ViewsManager.h"

namespace SGEditor
{
    struct EditorMain
    {
        static void start() noexcept;

        static auto getMainViewsManager() noexcept
        {
            return m_mainViewsManager;
        }

    private:
        static inline std::shared_ptr<ViewsManager> m_mainViewsManager = std::make_shared<ViewsManager>();
    };
}

#endif //SUNGEARENGINE_EDITORMAIN_H
