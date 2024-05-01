//
// Created by ilya on 30.04.24.
//

#include "ImGuiUtils.h"

bool SGE::ImGuiUtils::ImageButton(void* imageNativeHandler, const ImVec2& imageSize, const ImVec2& hoverMinOffset,
                                  const ImVec2& hoverMaxOffset, const ImVec4& hoverBgColor) noexcept
{
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    
    bool mouseHoveringBg = ImGui::IsMouseHoveringRect(
            ImVec2(cursorPos.x + hoverMinOffset.x, cursorPos.y + hoverMinOffset.y),
            ImVec2(cursorPos.x + imageSize.x + hoverMaxOffset.x,
                   cursorPos.y + imageSize.y + hoverMaxOffset.y));
    
    if(mouseHoveringBg)
    {
        // ImGui::GetWindowDrawList()
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x + hoverMinOffset.x, cursorPos.y + hoverMinOffset.y),
                                                      ImVec2(cursorPos.x + imageSize.x + hoverMaxOffset.x, cursorPos.y + imageSize.y + hoverMaxOffset.y),
                                                      ImGui::ColorConvertFloat4ToU32(hoverBgColor), 3);
    }
    
    ImGui::GetWindowDrawList()->AddImage(imageNativeHandler, cursorPos, ImVec2(cursorPos.x + imageSize.x, cursorPos.y + imageSize.y));
    
    bool clicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cursorPos.x + imageSize.x + hoverMaxOffset.x) - (cursorPos.x + hoverMinOffset.x));
    
    ImGui::NewLine();
    
    return clicked;
}
