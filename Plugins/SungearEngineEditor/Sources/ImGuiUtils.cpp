//
// Created by ilya on 30.04.24.
//

#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

SGE::ImClickInfo
SGE::ImGuiUtils::ImageButton(void* imageNativeHandler, const ImVec2& buttonSize,
                             const ImVec2& imageSize, const ImVec2& imageOffset,
                             const ImVec4& hoverBgColor) noexcept
{
    assert(buttonSize.x >= imageSize.x && buttonSize.y >= imageSize.y && "Button size must be greater then image size!");
    
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    
    ImVec2 offset = imageOffset;
    if(imageOffset.x == -1 && imageOffset.y == -1)
    {
        offset = {
                (buttonSize.x - imageSize.x) / 2.0f,
                (buttonSize.y - imageSize.y) / 2.0f
        };
    }
    
    bool mouseHoveringBg = ImGui::IsMouseHoveringRect(
            ImVec2(cursorPos.x, cursorPos.y),
            ImVec2(cursorPos.x + buttonSize.x,
                   cursorPos.y + buttonSize.y));
    
    if(mouseHoveringBg)
    {
        // ImGui::GetWindowDrawList()
        ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(cursorPos.x, cursorPos.y),
                ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y),
                ImGui::ColorConvertFloat4ToU32(hoverBgColor), 3);
    }
    
    ImGui::GetWindowDrawList()->AddImage(imageNativeHandler, ImVec2(cursorPos.x + offset.x, cursorPos.y + offset.y),
                                         ImVec2(cursorPos.x + imageSize.x + offset.x, cursorPos.y + imageSize.y + offset.y));
    
    bool clicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    bool doubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

    ImGui::Dummy(buttonSize);
    
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
