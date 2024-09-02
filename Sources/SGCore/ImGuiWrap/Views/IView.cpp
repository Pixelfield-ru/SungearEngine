//
// Created by stuka on 28.11.2023.
//

#include "IView.h"

void SGCore::ImGuiWrap::IView::render()
{
    if(!m_active) return;

    if(begin())
    {
        renderBody();
        
        auto childrenIt = m_children.begin();
        while(childrenIt != m_children.end())
        {
            if(auto lockedChild = childrenIt->lock())
            {
                lockedChild->render();
                ++childrenIt;
            }
            else
            {
                childrenIt = m_children.erase(childrenIt);
            }
        }
        
        end();

        m_appearing = false;
    }

    postRender();
}

SGCore::Ref<SGCore::ImGuiWrap::IView> SGCore::ImGuiWrap::IView::getViewByName(const std::string& name) const noexcept
{
    for(const auto& child : m_children)
    {
        if(auto lockedChild = child.lock())
        {
            if(lockedChild->m_name == name)
            {
                return lockedChild;
            }
        }
    }
    
    return nullptr;
}

std::vector<SGCore::Ref<SGCore::ImGuiWrap::IView>> SGCore::ImGuiWrap::IView::getViewsWithTag(const std::string& tag) const noexcept
{
    std::vector<Ref<IView>> foundViews;
    for(const auto& child : m_children)
    {
        if(auto lockedChild = child.lock())
        {
            if(lockedChild->m_tag == tag)
            {
                foundViews.push_back(lockedChild);
            }
        }
    }
    
    return foundViews;
}

SGCore::Weak<SGCore::ImGuiWrap::IView> SGCore::ImGuiWrap::IView::getParent() const noexcept
{
    return m_parent;
}

void SGCore::ImGuiWrap::IView::setParent(const SGCore::Ref<SGCore::ImGuiWrap::IView>& view) noexcept
{
    m_parent = view;
    
    m_name.attachToManager(view->m_namesManager);
}

std::vector<SGCore::Weak<SGCore::ImGuiWrap::IView>>& SGCore::ImGuiWrap::IView::getChildren() noexcept
{
    return m_children;
}

void SGCore::ImGuiWrap::IView::addChild(const SGCore::Weak<SGCore::ImGuiWrap::IView>& view) noexcept
{
    if(auto lockedView = view.lock())
    {
        lockedView->m_name.attachToManager(m_namesManager);
        m_children.push_back(view);
    }
}

void SGCore::ImGuiWrap::IView::addChildren(const std::vector<SGCore::Weak<SGCore::ImGuiWrap::IView>>& views) noexcept
{
    for(const auto& view : views)
    {
        addChild(view);
    }
}

void SGCore::ImGuiWrap::IView::removeChild(const SGCore::Weak<SGCore::ImGuiWrap::IView>& view) noexcept
{
    std::erase_if(m_children, [&view](const Weak<IView>& v) {
        auto lockedView0 = v.lock();
        auto lockedView1 = view.lock();
        return lockedView0 && lockedView1 && lockedView0 == lockedView1;
    });
}

void SGCore::ImGuiWrap::IView::removeChildren(const std::vector<SGCore::Weak<SGCore::ImGuiWrap::IView>>& views) noexcept
{
    for(const auto& view : views)
    {
        removeChild(view);
    }
}

bool SGCore::ImGuiWrap::IView::isActive() const noexcept
{
    return m_active;
}

void SGCore::ImGuiWrap::IView::setActive(bool active) noexcept
{
    bool lastActive = m_active;
    m_active = active;

    if(!lastActive && m_active)
    {
        m_appearing = true;
    }

    onActiveChangedListener();
    onActiveChanged(lastActive, m_active);
}

SGCore::Ref<SGCore::ImGuiWrap::IView> SGCore::ImGuiWrap::IView::getRoot() noexcept
{
    return m_rootView;
}

bool SGCore::ImGuiWrap::IView::isAppearing() const noexcept
{
    return m_appearing;
}

void SGCore::ImGuiWrap::IView::renderBody()
{
    onRenderBody();
}
