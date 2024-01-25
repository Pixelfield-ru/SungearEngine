//
// Created by stuka on 28.11.2023.
//

#include "IView.h"
#include "SGCore/ImGuiWrap/ViewsInjector.h"

/*SGCore::ImGuiWrap::IView::IView() noexcept
{
    inject();
}*/

std::string SGCore::ImGuiWrap::IView::getUniquePathPart() const noexcept
{
    return m_uniquePathPart;
}

void SGCore::ImGuiWrap::IView::inject() noexcept
{
    //auto f = weak_from_this();
    (*SGUtils::Singleton::getSharedPtrInstance<ViewsInjector>())["DefaultView"].m_childrenViews.push_back(weak_from_this());
}
