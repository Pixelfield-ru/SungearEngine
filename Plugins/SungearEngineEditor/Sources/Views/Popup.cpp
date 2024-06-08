//
// Created by ilya on 07.06.24.
//

#include "Popup.h"
#include "Styles/StylesManager.h"

void SGE::PopupElement::draw(Popup* parentPopup, PopupElement* parentElement) noexcept
{
    if(m_isOpened)
    {
        ImGui::OpenPopup(m_name.c_str());
        
        ImGui::SetNextWindowPos(m_popupPos);
        if(ImGui::BeginPopup(m_name.c_str(), ImGuiWindowFlags_NoMove))
        {
            ImGui::GetWindowDrawList()->AddRectFilled(m_rectToDraw.Min,
                                                      m_rectToDraw.Max,
                                                      ImGui::ColorConvertFloat4ToU32(
                                                              ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)),
                                                      3.0f);
            
            m_rectToDraw = { };
            
            if(ImGui::BeginTable(m_name.c_str(), 3))
            {
                for(auto& elem : m_elements)
                {
                    ImGui::TableNextRow();
                    
                    // FIRST COLUMN FOR ICON
                    ImGui::TableNextColumn();
                    if(elem.m_icon)
                    {
                        ImGui::Image(elem.m_icon->getTextureNativeHandler(), ImVec2(elem.m_icon->getWidth(), elem.m_icon->getHeight()));
                    }
                    
                    // SECOND COLUMN FOR NAME
                    ImGui::TableNextColumn();
                    ImGui::Text(elem.m_name.c_str());
                    
                    // THIRD COLUMN FOR HOT KEYS OR CHEVRON ICON
                    ImGui::TableNextColumn();
                    float thirdColumnRightAlign = 0.0f;
                    if(elem.m_elements.empty())
                    {
                        auto textRightAlign = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() -
                                               ImGui::CalcTextSize(elem.m_hint.c_str()).x);
                        if(textRightAlign > ImGui::GetCursorPosX())
                        {
                            thirdColumnRightAlign = textRightAlign;
                            ImGui::SetCursorPosX(textRightAlign);
                        }
                        ImGui::TextDisabled(elem.m_hint.c_str());
                    }
                    else
                    {
                        auto iconRightAlign = ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - 16;
                        if(iconRightAlign > ImGui::GetCursorPosX())
                        {
                            thirdColumnRightAlign = iconRightAlign;
                            ImGui::SetCursorPosX(iconRightAlign);
                        }
                        
                        ImGui::Image(StylesManager::getCurrentStyle()
                                             ->m_chevronRightIcon
                                             ->getSpecialization(16, 16)
                                             ->getTexture()->getTextureNativeHandler(),
                                     { 16, 16 });
                    }
                    
                    ImRect rowStart = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 0);
                    ImRect rowEnd = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 2);
                    
                    bool hoveringRow = ImGui::IsMouseHoveringRect(rowStart.Min, rowEnd.Max, false);
                    if(hoveringRow)
                    {
                        if(!elem.m_elements.empty())
                        {
                            elem.m_popupPos = { rowEnd.Max.x + 6,
                                                rowStart.Min.y };
                            elem.m_isOpened = true;
                        }
                        
                        for(auto& e : m_elements)
                        {
                            if(&e != &elem)
                            {
                                e.recursiveClose();
                            }
                        }
                        
                        if(elem.m_elements.empty() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            parentPopup->onElementClicked(elem);
                            
                            if(elem.m_closePopupWhenClicked)
                            {
                                parentPopup->setOpened(false);
                            }
                        }
                    }
                    
                    elem.m_isHovered = hoveringRow;
                    
                    if(elem.m_isOpened || elem.m_isHovered)
                    {
                        m_rectToDraw = ImRect({ rowStart.Min.x - 4, rowStart.Min.y + 3 }, { rowEnd.Max.x + 4, rowEnd.Max.y - 3 });
                    }
                    
                    elem.draw(parentPopup, this);
                    
                    if(elem.m_drawSeparatorAfter)
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
        e.m_isOpened = false;
        e.recursiveClose();
    }
}

SGE::Popup::Popup(const std::string& name, const std::initializer_list<PopupElement>& items) noexcept
{
    m_name = name;
    
    for(auto& data : items)
    {
        m_elements.push_back(data);
    }
}

void SGE::Popup::draw() noexcept
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0);
    
    if(m_isOpened && ImGui::BeginPopup(m_name.c_str(), ImGuiWindowFlags_NoMove))
    {
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
                ImGui::TableNextRow();
                
                // FIRST COLUMN FOR ICON
                ImGui::TableNextColumn();
                if(elem.m_icon)
                {
                    ImGui::Image(elem.m_icon->getTextureNativeHandler(), ImVec2(elem.m_icon->getWidth(), elem.m_icon->getHeight()));
                }
                
                // SECOND COLUMN FOR NAME
                ImGui::TableNextColumn();
                ImGui::Text(elem.m_name.c_str());
                
                // THIRD COLUMN FOR HOT KEYS OR CHEVRON ICON
                ImGui::TableNextColumn();
                if(elem.m_elements.empty())
                {
                    auto textRightAlign = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() -
                                       ImGui::CalcTextSize(elem.m_hint.c_str()).x);
                    if(textRightAlign > ImGui::GetCursorPosX())
                    {
                        ImGui::SetCursorPosX(textRightAlign);
                    }
                    ImGui::TextDisabled(elem.m_hint.c_str());
                }
                else
                {
                    auto iconRightAlign = ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - 16;
                    if(iconRightAlign > ImGui::GetCursorPosX())
                    {
                        ImGui::SetCursorPosX(iconRightAlign);
                    }
                    
                    ImGui::Image(StylesManager::getCurrentStyle()
                                         ->m_chevronRightIcon
                                         ->getSpecialization(16, 16)
                                         ->getTexture()->getTextureNativeHandler(),
                                 { 16, 16 });
                }
                
                ImRect rowStart = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 0);
                ImRect rowEnd = ImGui::TableGetCellBgRect(ImGui::GetCurrentContext()->CurrentTable, 2);
                
                bool hoveringRow = ImGui::IsMouseHoveringRect(rowStart.Min, rowEnd.Max, false);
                if(hoveringRow)
                {
                    if(!elem.m_elements.empty())
                    {
                        elem.m_popupPos = { rowEnd.Max.x + 6,
                                            rowStart.Min.y };
                        elem.m_isOpened = true;
                    }
                    
                    for(auto& e : m_elements)
                    {
                        if(&e != &elem)
                        {
                            e.recursiveClose();
                        }
                    }
                    
                    if(elem.m_elements.empty() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        onElementClicked(elem);
                        
                        if(elem.m_closePopupWhenClicked)
                        {
                            setOpened(false);
                        }
                    }
                }
                
                elem.m_isHovered = hoveringRow;
                
                if(elem.m_isOpened || elem.m_isHovered)
                {
                    m_rectToDraw = ImRect({ rowStart.Min.x - 4, rowStart.Min.y + 3 }, { rowEnd.Max.x + 4, rowEnd.Max.y - 3 });
                }
                
                elem.draw(this, &elem);
                
                if(elem.m_drawSeparatorAfter)
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
}

void SGE::Popup::recursiveClose() noexcept
{
    for(auto& e : m_elements)
    {
        e.recursiveClose();
    }
}

bool SGE::Popup::isOpened() const noexcept
{
    return m_isOpened;
}

void SGE::Popup::setOpened(bool isOpened) noexcept
{
    recursiveClose();
    m_isOpened = isOpened;
    ImGui::OpenPopup(m_name.c_str());
}
