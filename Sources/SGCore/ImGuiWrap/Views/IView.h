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
#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/UniqueName.h"
#include "SGUtils/UniqueNamesManager.h"

namespace SGCore::ImGuiWrap
{
    struct IView : public std::enable_shared_from_this<IView>
    {
        friend struct ViewsInjector;

        bool m_active = true;

        Event<void()> onRender;

        UniqueName m_name = SGUtils::UUID::generateNew();
        std::string m_tag;

        virtual bool begin() { return true; };
        virtual void renderBody() { };
        virtual void end() { };
        
        virtual void render();
        
        Ref<IView> getViewByName(const std::string& name) const noexcept;
        std::vector<Ref<IView>> getViewsWithTag(const std::string& tag) const noexcept;

        Weak<IView> getParent() const noexcept;
        void setParent(const Ref<IView>& view) noexcept;
        
        std::vector<Weak<IView>>& getChildren() noexcept;
        
        void addChild(const Weak<IView>& view) noexcept;
        void addChildren(const std::vector<Weak<IView>>& views) noexcept;
        void removeChild(const Weak<IView>& view) noexcept;
        void removeChildren(const std::vector<Weak<IView>>& views) noexcept;
        
        SG_NOINLINE static Ref<IView> getRoot() noexcept;
        
    private:
        static inline Ref<IView> m_rootView = MakeRef<IView>();
        
        std::vector<Weak<IView>> m_children;
        
        Weak<IView> m_parent;
        
        Ref<UniqueNamesManager> m_namesManager = MakeRef<UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINE_IVIEW_H
