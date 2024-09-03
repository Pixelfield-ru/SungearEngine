//
// Created by ilya on 07.06.24.
//

#ifndef SUNGEARENGINEEDITOR_POPUP_H
#define SUNGEARENGINEEDITOR_POPUP_H

#include <string>
#include <imgui.h>
#include <imgui_internal.h>

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Utils/UniqueNamesManager.h>

namespace SGE
{
    struct Popup;
    
    struct PopupElement
    {
        // INITIALIZE NEXT VARIABLES IN LIST INITIALIZER!
        SGCore::UniqueName m_name;
        std::string m_hint;
        
        SGCore::Ref<SGCore::ITexture2D> m_icon;
        bool m_drawSeparatorAfter = false;
        bool m_closePopupWhenClicked = true;
        bool m_isActive = true;
        
        std::vector<SGCore::Ref<PopupElement>> m_elements;
        
        void draw(Popup* parentPopup, PopupElement* parentElement) noexcept;
        
        // DO NOT INITIALIZE NEXT VARIABLES IN LIST INITIALIZER!
        ImVec2 m_popupPos { };
        bool m_isOpened = false;
        bool m_isHovered = false;
        
        ImRect m_rectToDraw;

        SGCore::Ref<SGCore::UniqueNamesManager> m_namesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();

        void recursiveClose() noexcept;
        void setAllElementsActive(bool isActive) noexcept;
        [[nodiscard]] SGCore::Ref<PopupElement> tryGetElementRecursively(std::string_view name) noexcept;
    };
    
    struct Popup
    {
        friend struct PopupElement;

        Popup() = default;
        Popup(const Popup&) = default;
        Popup(Popup&&) = default;

        Popup(const std::string& name, const std::initializer_list<PopupElement>& items) noexcept;
        
        std::string m_name;
        std::vector<SGCore::Ref<PopupElement>> m_elements;
        
        void draw() noexcept;
        
        SGCore::Event<void(const SGCore::Ref<PopupElement>&)> onElementClicked;
        SGCore::Event<void(bool last, bool current)> onOpenedChanged;

        SGCore::Ref<PopupElement> tryGetElement(std::string_view name) noexcept;
        
        void recursiveClose() noexcept;
        void setAllElementsActive(bool isActive) noexcept;
        
        [[nodiscard]] bool isOpened() const noexcept;
        void setOpened(bool isOpened) noexcept;

        Popup& operator=(const Popup&) = default;
        Popup& operator=(Popup&&) = default;

    private:
        bool m_isOpened = false;
        std::int32_t m_drawingElementsCount = 0;
        
        ImRect m_rectToDraw { };

        SGCore::Ref<SGCore::UniqueNamesManager> m_namesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINEEDITOR_POPUP_H
