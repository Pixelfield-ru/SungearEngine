//
// Created by ilya on 30.04.24.
//

#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

SGE::ImClickInfo
SGE::ImGuiUtils::ImageButton(void* imageNativeHandler, const ImVec2& imageSize, const ImVec2& hoverMinOffset,
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
        ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(cursorPos.x + hoverMinOffset.x, cursorPos.y + hoverMinOffset.y),
                ImVec2(cursorPos.x + imageSize.x + hoverMaxOffset.x, cursorPos.y + imageSize.y + hoverMaxOffset.y),
                ImGui::ColorConvertFloat4ToU32(hoverBgColor), 3);
    }
    
    ImGui::GetWindowDrawList()->AddImage(imageNativeHandler, cursorPos, ImVec2(cursorPos.x + imageSize.x, cursorPos.y + imageSize.y));
    
    bool clicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    bool doubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    /*float clickableMinX = cursorPos.x + hoverMinOffset.x;
    float clickableMinY = cursorPos.y + hoverMinOffset.y;
    float clickableMaxX = cursorPos.x + imageSize.x + hoverMaxOffset.x;
    float clickableMaxY = cursorPos.y + imageSize.y + hoverMaxOffset.y;
    
    float imageMinX = cursorPos.x;
    float imageMinY = cursorPos.y;
    float imageMaxX = cursorPos.x + imageSize.x;
    float imageMaxY = cursorPos.y + imageSize.y;
    
    float maxX = clickableMaxX > imageMaxX ? clickableMaxX : imageMaxX;
    float maxY = clickableMaxY > imageMaxY ? clickableMaxY : imageMaxY;
    float minX = clickableMinX < imageMinX ? clickableMinX : imageMinX;
    float minY = clickableMinY < imageMinY ? clickableMinY : imageMinY;
    
    ImVec2 maxItemArea = ImVec2(maxX - minX, maxY - minY);*/

    ImGui::Dummy(imageSize);
    
    ImClickInfo clickInfo {
        .m_isClicked = clicked,
        .m_isDoubleClicked = doubleClicked,
        .m_isHovered = mouseHoveringBg
    };
    
    return clickInfo;
}

SGCore::Ref<SGCore::ITexture2D> SGE::ImGuiUtils::getFileIcon(const std::filesystem::path& filePath,
                                                             const SGCore::ivec2_32& iconSize,
                                                             SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>&,
                                                                                const std::string&,
                                                                                const std::string&)>* onIconSet) noexcept
{
    SGCore::Ref<SGCore::ITexture2D> fileIcon;
    
    std::string fileExtension = filePath.extension().string();
    std::string fileName = filePath.stem().string();
    
    if(fileExtension == ".h")
    {
        fileIcon = StylesManager::getCurrentStyle()->m_headerIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    else if(fileExtension == ".cpp")
    {
        fileIcon = StylesManager::getCurrentStyle()->m_cppIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    else if(fileExtension == ".cmake" || (fileName + fileExtension) == "CMakeLists.txt")
    {
        fileIcon = StylesManager::getCurrentStyle()->m_cmakeIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    else if(fileExtension == ".txt" || fileExtension == ".log")
    {
        fileIcon = StylesManager::getCurrentStyle()->m_linesIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    else if(fileExtension == ".dll" || fileExtension == ".so")
    {
        fileIcon = StylesManager::getCurrentStyle()->m_libraryIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    
    if(onIconSet)
    {
        (*onIconSet)(fileIcon, fileExtension, fileName);
    }
    
    // unknown file
    if(!fileIcon)
    {
        fileIcon = StylesManager::getCurrentStyle()->m_questionIcon->getSpecialization(iconSize.x, iconSize.y)->getTexture();
    }
    
    return fileIcon;
}
