//
// Created by ilya on 07.06.24.
//

#include "Popup.h"
#include "Styles/StylesManager.h"

void SGE::PopupElement::draw(Popup* parentPopup, PopupElement* parentElement) noexcept
{
    if(m_isOpened)
    {
        ImGui::OpenPopup(m_name.getName().c_str());
        
        ImGui::SetNextWindowPos(m_popupPos);
        if(ImGui::BeginPopup(m_name.getName().c_str(), ImGuiWindowFlags_NoMove))
        {
            ImGui::GetWindowDrawList()->AddRectFilled(m_rectToDraw.Min,
                                                      m_rectToDraw.Max,
                                                      ImGui::ColorConvertFloat4ToU32(
                                                              ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)),
                                                      3.0f);
            
            m_rectToDraw = { };
            
            if(ImGui::BeginTable(m_name.getName().c_str(), 3))
            {
                for(auto& elem : m_elements)
                {
                    if(!elem) continue;
                    
                    if(!elem->m_isActive) continue;

                    if(elem->m_name.getNamesManager() != m_namesManager)
                    {
                        elem->m_name.attachToManager(m_namesManager);
                    }
                    
                    ++parentPopup->m_drawingElementsCount;
                    
                    ImGui::TableNextRow();

                    float rowHeight = ImGui::GetFrameHeight();
                    
                    // FIRST COLUMN FOR ICON
                    ImGui::TableNextColumn();
                    if(elem->m_icon)
                    {
                        ImGui::Image(elem->m_icon->getTextureNativeHandler(), ImVec2(elem->m_icon->getWidth(), 
                                                                                     elem->m_icon->getHeight()));

                        rowHeight = elem->m_icon->getHeight() > rowHeight ? elem->m_icon->getHeight() : rowHeight;
                    }
                    
                    // SECOND COLUMN FOR NAME
                    ImGui::TableNextColumn();
                    {
                        ImVec2 textSize = ImGui::CalcTextSize(elem->m_name.getName().c_str());
                        if(textSize.y > rowHeight)
                        {
                            rowHeight = textSize.y;
                        }
                        else
                        {
                            float offsetY = (rowHeight - textSize.y) / 2.0f;
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
                        }
                        ImGui::Text(elem->m_name.getName().c_str());
                    }
                    
                    // THIRD COLUMN FOR HOT KEYS OR CHEVRON ICON
                    ImGui::TableNextColumn();
                    float thirdColumnRightAlign = 0.0f;
                    if(elem->m_elements.empty())
                    {
                        ImVec2 textSize = ImGui::CalcTextSize(elem->m_hint.c_str());
                        auto textRightAlign = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() -
                                               ImGui::CalcTextSize(elem->m_hint.c_str()).x);
                        if(textRightAlign > ImGui::GetCursorPosX())
                        {
                            thirdColumnRightAlign = textRightAlign;
                            ImGui::SetCursorPosX(textRightAlign);
                        }

                        if(textSize.y > rowHeight)
                        {
                            rowHeight = textSize.y;
                        }
                        else
                        {
                            float offsetY = (rowHeight - textSize.y) / 2.0f;
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
                        }

                        ImGui::TextDisabled(elem->m_hint.c_str());
                    }
                    else
                    {
                        auto iconRightAlign = ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - rowHeight;
                        if(iconRightAlign > ImGui::GetCursorPosX())
                        {
                            thirdColumnRightAlign = iconRightAlign;
                            ImGui::SetCursorPosX(iconRightAlign);
                        }

                        ImGui::Image(StylesManager::getCurrentStyle()
                                             ->m_chevronRightIcon
                                             ->getSpecialization((std::int32_t) rowHeight, (std::int32_t) rowHeight)
                                             ->getTexture()->getTextureNativeHandler(),
                                     { rowHeight, rowHeight });
                    }
                    
                    ImRect rowStart = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 0);
                    ImRect rowEnd = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 2);
                    
                    bool hoveringRow = ImGui::IsMouseHoveringRect(rowStart.Min, rowEnd.Max, false);
                    if(hoveringRow)
                    {
                        if(!elem->m_elements.empty())
                        {
                            elem->m_popupPos = { rowEnd.Max.x + 6,
                                                rowStart.Min.y };
                            elem->m_isOpened = true;
                        }
                        
                        for(auto& e : m_elements)
                        {
                            if(&e != &elem)
                            {
                                e->recursiveClose();
                            }
                        }
                        
                        if(elem->m_elements.empty() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            parentPopup->onElementClicked(elem);
                            
                            if(elem->m_closePopupWhenClicked)
                            {
                                parentPopup->setOpened(false);
                            }
                        }
                    }
                    
                    elem->m_isHovered = hoveringRow;
                    
                    if(elem->m_isOpened || elem->m_isHovered)
                    {
                        m_rectToDraw = ImRect({ rowStart.Min.x - 4, rowStart.Min.y + 3 }, { rowEnd.Max.x + 4, rowEnd.Max.y - 3 });
                    }
                    
                    elem->draw(parentPopup, this);
                    
                    if(elem->m_drawSeparatorAfter)
                    {
                        ImGui::GetForegroundDrawList()->AddLine({ rowStart.Min.x, rowEnd.Max.y },
                                                                { rowEnd.Max.x, rowEnd.Max.y },
                                                                ImGui::ColorConvertFloat4ToU32(
                                                                        { ImGui::GetStyle().Colors[ImGuiCol_Separator] }));
                    }
                }
                
                ImGui::EndTable();
            }
            
            ImGui::EndPopup();
        }
    }
}

void SGE::PopupElement::recursiveClose() noexcept
{
    m_isOpened = false;
    for(auto& e : m_elements)
    {
        e->m_isOpened = false;
        e->recursiveClose();
    }
}

void SGE::PopupElement::setAllElementsActive(bool isActive) noexcept
{
    m_isActive = isActive;
    for(auto& e : m_elements)
    {
        e->m_isActive = isActive;
    }
}

SGCore::Ref<SGE::PopupElement> SGE::PopupElement::tryGetElementRecursively(std::string_view name) noexcept
{
    SGCore::Ref<PopupElement> elem;

    for(auto& e : m_elements)
    {
        if(e->m_name.getNamesManager() != m_namesManager)
        {
            e->m_name.attachToManager(m_namesManager);
        }

        if(e->m_name == name)
        {
            elem = e;
            break;
        }

        elem = e->tryGetElementRecursively(name);
    }
    
    return elem;
}

SGE::Popup::Popup(const std::string& name, const std::initializer_list<PopupElement>& items) noexcept
{
    m_name = name;
    
    for(auto& data : items)
    {
        auto e = SGCore::MakeRef<PopupElement>(data);
        m_elements.push_back(e);
    }
}

void SGE::Popup::draw() noexcept
{
    m_drawingElementsCount = 0;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);
    
    if(m_isOpened && ImGui::BeginPopup(m_name.c_str(), ImGuiWindowFlags_NoMove))
    {
        // setOpened(ImGui::BeginPopup(m_name.c_str(), ImGuiWindowFlags_NoMove));
        
        ImGui::GetWindowDrawList()->AddRectFilled(m_rectToDraw.Min,
                                                  m_rectToDraw.Max,
                                                  ImGui::ColorConvertFloat4ToU32(
                                                          ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)),
                                                  3.0f);
        
        m_rectToDraw = { };
        
        if (ImGui::BeginTable(m_name.c_str(), 3))
        {
            for(auto& elem : m_elements)
            {
                if(!elem->m_isActive) continue;

                if(elem->m_name.getNamesManager() != m_namesManager)
                {
                    elem->m_name.attachToManager(m_namesManager);
                }
                
                ++m_drawingElementsCount;
                
                ImGui::TableNextRow();
                
                float rowHeight = ImGui::GetFrameHeight();
                
                // FIRST COLUMN FOR ICON
                ImGui::TableNextColumn();
                if(elem->m_icon)
                {
                    ImGui::Image(elem->m_icon->getTextureNativeHandler(), ImVec2(elem->m_icon->getWidth(), elem->m_icon->getHeight()));
                    
                    rowHeight = elem->m_icon->getHeight() > rowHeight ? elem->m_icon->getHeight() : rowHeight;
                }
                
                // SECOND COLUMN FOR NAME
                ImGui::TableNextColumn();
                {
                    ImVec2 textSize = ImGui::CalcTextSize(elem->m_name.getName().c_str());
                    if(textSize.y > rowHeight)
                    {
                        rowHeight = textSize.y;
                    }
                    else
                    {
                        float offsetY = (rowHeight - textSize.y) / 2.0f;
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
                    }
                    ImGui::Text(elem->m_name.getName().c_str());
                }
                
                // THIRD COLUMN FOR HOT KEYS OR CHEVRON ICON
                ImGui::TableNextColumn();
                if(elem->m_elements.empty())
                {
                    ImVec2 textSize = ImGui::CalcTextSize(elem->m_hint.c_str());
                    auto textRightAlign = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - textSize.x);
                    if(textRightAlign > ImGui::GetCursorPosX())
                    {
                        ImGui::SetCursorPosX(textRightAlign);
                    }
                    
                    if(textSize.y > rowHeight)
                    {
                        rowHeight = textSize.y;
                    }
                    else
                    {
                        float offsetY = (rowHeight - textSize.y) / 2.0f;
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
                    }
                    
                    ImGui::TextDisabled(elem->m_hint.c_str());
                }
                else
                {
                    auto iconRightAlign = ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - rowHeight;
                    if(iconRightAlign > ImGui::GetCursorPosX())
                    {
                        ImGui::SetCursorPosX(iconRightAlign);
                    }
                    
                    ImGui::Image(StylesManager::getCurrentStyle()
                                         ->m_chevronRightIcon
                                         ->getSpecialization((std::int32_t) rowHeight, (std::int32_t) rowHeight)
                                         ->getTexture()->getTextureNativeHandler(),
                                 { rowHeight, rowHeight });
                }
                
                ImRect rowStart = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 0);
                ImRect rowEnd = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 2);
                
                bool hoveringRow = ImGui::IsMouseHoveringRect(rowStart.Min, rowEnd.Max, false);
                if(hoveringRow)
                {
                    if(!elem->m_elements.empty())
                    {
                        elem->m_popupPos = { rowEnd.Max.x + 6,
                                            rowStart.Min.y };
                        elem->m_isOpened = true;
                    }
                    
                    for(auto& e : m_elements)
                    {
                        if(&e != &elem)
                        {
                            e->recursiveClose();
                        }
                    }
                    
                    if(elem->m_elements.empty() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        onElementClicked(elem);
                        
                        if(elem->m_closePopupWhenClicked)
                        {
                            setOpened(false);
                        }
                    }
                }
                
                elem->m_isHovered = hoveringRow;
                
                if(elem->m_isOpened || elem->m_isHovered)
                {
                    m_rectToDraw = ImRect({ rowStart.Min.x - 4, rowStart.Min.y + 3 }, { rowEnd.Max.x + 4, rowEnd.Max.y - 3 });
                }
                
                elem->draw(this, elem.get());
                
                if(elem->m_drawSeparatorAfter)
                {
                    ImGui::GetForegroundDrawList()->AddLine({ rowStart.Min.x, rowEnd.Max.y },
                                                            { rowEnd.Max.x, rowEnd.Max.y },
                                                            ImGui::ColorConvertFloat4ToU32(
                                                                    { ImGui::GetStyle().Colors[ImGuiCol_Separator] }));
                }
            }
            
            ImGui::EndTable();
        }
        
        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
    
    if(m_drawingElementsCount == 0)
    {
        setOpened(false);
    }
}

void SGE::Popup::recursiveClose() noexcept
{
    for(auto& e : m_elements)
    {
        e->recursiveClose();
    }
}

void SGE::Popup::setAllElementsActive(bool isActive) noexcept
{
    for(auto& e : m_elements)
    {
        e->setAllElementsActive(isActive);
    }
}

bool SGE::Popup::isOpened() const noexcept
{
    return m_isOpened;
}

void SGE::Popup::setOpened(bool isOpened) noexcept
{
    if(m_isOpened != isOpened)
    {
        onOpenedChanged(m_isOpened, isOpened);
    }
    
    recursiveClose();
    m_isOpened = isOpened;
    if(m_isOpened)
    {
        ImGui::OpenPopup(m_name.c_str());
    }
}

SGCore::Ref<SGE::PopupElement> SGE::Popup::tryGetElement(std::string_view name) noexcept
{
    SGCore::Ref<PopupElement> elem;

    for(auto& e : m_elements)
    {
        if(e->m_name.getNamesManager() != m_namesManager)
        {
            e->m_name.attachToManager(m_namesManager);
        }

        if(e->m_name == name)
        {
            elem = e;
            break;
        }

        elem = e->tryGetElementRecursively(name);
    }
    
    return elem;
}
