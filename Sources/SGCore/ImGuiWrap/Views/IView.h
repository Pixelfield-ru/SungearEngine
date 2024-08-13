//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_IVIEW_H
#define SUNGEARENGINE_IVIEW_H

#include <SGCore/pch.h>

#include "SGCore/Utils/UUID.h"
#include "SGCore/Utils/Event.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Utils/UniqueNamesManager.h"

namespace SGCore::ImGuiWrap
{
    struct IView : public std::enable_shared_from_this<IView>
    {
        friend struct ViewsInjector;

        Event<void()> onRender;
        Event<void(bool lastActive, bool isActive)> onActiveChanged;

        UniqueName m_name = SGCore::UUID::generateNew();
        std::string m_tag;

        glm::vec2 m_UIScale { 1, 1 };
        
        virtual bool begin() { return true; };
        virtual void renderBody() { };
        virtual void end() { };
        
        virtual void onActiveChangedListener() { };
        
        virtual void render();
        virtual void postRender() { };

        Ref<IView> getViewByName(const std::string& name) const noexcept;
        std::vector<Ref<IView>> getViewsWithTag(const std::string& tag) const noexcept;

        Weak<IView> getParent() const noexcept;
        void setParent(const Ref<IView>& view) noexcept;
        
        std::vector<Weak<IView>>& getChildren() noexcept;
        
        void addChild(const Weak<IView>& view) noexcept;
        void addChildren(const std::vector<Weak<IView>>& views) noexcept;
        void removeChild(const Weak<IView>& view) noexcept;
        void removeChildren(const std::vector<Weak<IView>>& views) noexcept;
        
        [[nodiscard]] bool isActive() const noexcept;
        void setActive(bool active) noexcept;

        [[nodiscard]] bool isAppearing() const noexcept;
        
        SG_NOINLINE static Ref<IView> getRoot() noexcept;
        
    private:
        static inline Ref<IView> m_rootView = MakeRef<IView>();

        bool m_appearing = true;
        bool m_active = true;

        std::vector<Weak<IView>> m_children;
        
        Weak<IView> m_parent;
        
        Ref<UniqueNamesManager> m_namesManager = MakeRef<UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINE_IVIEW_H
