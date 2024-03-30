//
// Created by stuka on 30.11.2023.
//

#include "ViewsInjector.h"

SGCore::ImGuiWrap::ViewsInjector& SGCore::ImGuiWrap::ViewsInjector::operator[](const std::string& viewName) noexcept
{
    auto foundInjector = m_childrenInjectors.find(viewName);

    if(foundInjector == m_childrenInjectors.end())
    {
        auto newInjector = std::shared_ptr<ViewsInjector>(new ViewsInjector);

        m_childrenInjectors[viewName] = newInjector;

        return *newInjector;
    }

    return *foundInjector->second;
}

void SGCore::ImGuiWrap::ViewsInjector::renderViews()
{
    auto lockedRootView = m_rootView.lock();

    if(lockedRootView)
    {
        if(lockedRootView->begin())
        {
            lockedRootView->renderBody();

            auto viewsIter = m_childrenViews.begin();

            while(viewsIter != m_childrenViews.end())
            {
                auto lockedView = viewsIter->lock();

                if(lockedView && lockedView->m_uniquePathPart == lockedRootView->m_uniquePathPart)
                {
                    if(lockedView->begin())
                    {
                        lockedView->renderBody();

                        lockedView->end();
                    }

                    ++viewsIter;
                }
                else
                {
                    viewsIter = m_childrenViews.erase(viewsIter);
                }
            }

            for(const auto& viewsInjector : m_childrenInjectors)
            {
                auto lockedChildRootView = viewsInjector.second->m_rootView.lock();

                if (lockedChildRootView && lockedChildRootView->m_uniquePathPart == lockedRootView->m_uniquePathPart)
                {
                    viewsInjector.second->renderViews();
                }
            }

            lockedRootView->end();
        }
    }
    else
    {
        for(const auto& viewsInjector : m_childrenInjectors)
        {
            viewsInjector.second->renderViews();
        }
    }
}
