//
// Created by ilya on 30.04.24.
//

#include <imgui_internal.h>
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

SGE::ImClickInfo
SGE::ImGuiUtils::ImageButton(void* imageNativeHandler, const ImVec2& buttonSize,
                             const ImVec2& imageSize, const ImVec2& imageOffset,
                             const ImVec4& hoverBgColor) noexcept
{
    assert(buttonSize.x >= imageSize.x && buttonSize.y >= imageSize.y && "Button size must be greater then image size!");
    
    ImVec2 offset = imageOffset;
    if(imageOffset.x == -1 && imageOffset.y == -1)
    {
        offset = {
                (buttonSize.x - imageSize.x) / 2.0f,
                (buttonSize.y - imageSize.y) / 2.0f
        };
    }
    
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    
    bool isWindowHovered = ImGui::IsWindowHovered();
    
    bool mouseHoveringBg = ImGui::IsMouseHoveringRect(
            ImVec2(cursorScreenPos.x, cursorScreenPos.y),
            ImVec2(cursorScreenPos.x + buttonSize.x,
                   cursorScreenPos.y + buttonSize.y));
    
    if(mouseHoveringBg && isWindowHovered)
    {
        // ImGui::GetWindowDrawList()
        ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(cursorScreenPos.x, cursorScreenPos.y),
                ImVec2(cursorScreenPos.x + buttonSize.x, cursorScreenPos.y + buttonSize.y),
                ImGui::ColorConvertFloat4ToU32(hoverBgColor), 3);
    }
    
    ImGui::GetWindowDrawList()->AddImage(imageNativeHandler, ImVec2(cursorScreenPos.x + offset.x, cursorScreenPos.y + offset.y),
                                         ImVec2(cursorScreenPos.x + imageSize.x + offset.x, cursorScreenPos.y + imageSize.y + offset.y));
    
    bool leftClicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    bool leftDoubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    bool rightClicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    bool rightDoubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right);
    
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);
    // ImGui::Dummy(buttonSize);
    
    /*ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return { };
    }*/
    
    ImGui::Dummy({ buttonSize.x, buttonSize.y - offset.y });
    
    // ImGui::NewLine();
    
    ImClickInfo clickInfo {
        .m_isLMBClicked = leftClicked && isWindowHovered,
        .m_isLMBDoubleClicked = leftDoubleClicked && isWindowHovered,
        .m_isRMBClicked = rightClicked && isWindowHovered,
        .m_isRMBDoubleClicked = rightDoubleClicked && isWindowHovered,
        .m_isHovered = mouseHoveringBg,
        .m_elementPosition = cursorScreenPos,
        .m_elementClickableSize = buttonSize
    };
    
    return clickInfo;
}

SGE::ImClickInfo SGE::ImGuiUtils::ImageButton(void* imageNativeHandler,
                                              const float& buttonRadius,
                                              const ImVec2& imageSize,
                                              const ImVec2& imageOffset,
                                              const ImVec4& hoverBgColor) noexcept
{
    ImVec2 buttonSquareSize { buttonRadius * 2, buttonRadius * 2 };
    
    assert(buttonSquareSize.x >= imageSize.x && buttonSquareSize.y >= imageSize.y && "Button radius must be greater then image size!");
    
    ImVec2 offset = imageOffset;
    if(imageOffset.x == -1 && imageOffset.y == -1)
    {
        offset = {
                (buttonSquareSize.x - imageSize.x) / 2.0f,
                (buttonSquareSize.y - imageSize.y) / 2.0f
        };
    }
    
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    
    bool isWindowHovered = ImGui::IsWindowHovered();
    
    bool mouseHoveringBg = ImGui::IsMouseHoveringRect(
            ImVec2(cursorScreenPos.x, cursorScreenPos.y),
            ImVec2(cursorScreenPos.x + buttonSquareSize.x,
                   cursorScreenPos.y + buttonSquareSize.y));
    
    //ImGui::GetWindowDrawList()->AddCircleFilled();
    
    if(mouseHoveringBg && isWindowHovered)
    {
        // ImGui::GetWindowDrawList()
        ImGui::GetWindowDrawList()->AddCircleFilled(
                ImVec2(cursorScreenPos.x + buttonSquareSize.x / 2, cursorScreenPos.y + buttonSquareSize.y / 2),
                buttonRadius,
                ImGui::ColorConvertFloat4ToU32(hoverBgColor), 18);
    }
    
    ImGui::GetWindowDrawList()->AddImage(imageNativeHandler, ImVec2(cursorScreenPos.x + offset.x, cursorScreenPos.y + offset.y),
                                         ImVec2(cursorScreenPos.x + imageSize.x + offset.x, cursorScreenPos.y + imageSize.y + offset.y));
    
    bool leftClicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    bool leftDoubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    
    bool rightClicked = mouseHoveringBg && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    bool rightDoubleClicked = mouseHoveringBg && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right);
    
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);
    // ImGui::Dummy(buttonSize);
    
    /*ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return { };
    }*/
    
    ImGui::Dummy({ buttonSquareSize.x, buttonSquareSize.y - offset.y });
    
    // ImGui::NewLine();
    
    ImClickInfo clickInfo {
            .m_isLMBClicked = leftClicked && isWindowHovered,
            .m_isLMBDoubleClicked = leftDoubleClicked && isWindowHovered,
            .m_isRMBClicked = rightClicked && isWindowHovered,
            .m_isRMBDoubleClicked = rightDoubleClicked && isWindowHovered,
            .m_isHovered = mouseHoveringBg,
            .m_elementPosition = cursorScreenPos,
            .m_elementClickableSize = buttonSquareSize
    };
    
    return clickInfo;
}

const char ColorMarkerStart = '{';
const char ColorMarkerEnd = '}';

bool SGE::ImGuiUtils::ProcessInlineHexColor(const char* start, const char* end, ImVec4& color) noexcept
{
    const int hexCount = (int) (end - start);
    if(hexCount == 6 || hexCount == 8)
    {
        char hex[9];
        strncpy(hex, start, hexCount);
        hex[hexCount] = 0;
        
        unsigned int hexColor = 0;
        if(sscanf(hex, "%x", &hexColor) > 0)
        {
            color.x = static_cast< float >((hexColor & 0x00FF0000) >> 16 ) / 255.0f;
            color.y = static_cast< float >((hexColor & 0x0000FF00) >> 8  ) / 255.0f;
            color.z = static_cast< float >((hexColor & 0x000000FF)) / 255.0f;
            color.w = 1.0f;
            
            if(hexCount == 8)
            {
                color.w = static_cast< float >((hexColor & 0xFF000000) >> 24 ) / 255.0f;
            }
            
            return true;
        }
    }
    
    return false;
}

void SGE::ImGuiUtils::TextWithColors(const char* fmt, ...) noexcept
{
    char tempStr[4096];
    
    va_list argPtr;
    va_start(argPtr, fmt);
    vsnprintf(tempStr, sizeof(tempStr), fmt, argPtr);
    va_end(argPtr);
    tempStr[sizeof(tempStr) - 1] = '\0';
    
    bool pushedColorStyle = false;
    const char* textStart = tempStr;
    const char* textCur = tempStr;
    while(textCur < (tempStr + sizeof(tempStr)) && *textCur != '\0')
    {
        if(*textCur == ColorMarkerStart)
        {
            // Print accumulated text
            if(textCur != textStart)
            {
                ImGui::TextUnformatted(textStart, textCur);
                ImGui::SameLine(0.0f, 0.0f);
            }
            
            // Process color code
            const char* colorStart = textCur + 1;
            do
            {
                ++textCur;
            } while(*textCur != '\0' && *textCur != ColorMarkerEnd);
            
            // Change color
            if(pushedColorStyle)
            {
                ImGui::PopStyleColor();
                pushedColorStyle = false;
            }
            
            ImVec4 textColor;
            if(ProcessInlineHexColor(colorStart, textCur, textColor))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, textColor);
                pushedColorStyle = true;
            }
            
            textStart = textCur + 1;
        }
        else if(*textCur == '\n')
        {
            // Print accumulated text an go to next line
            ImGui::TextUnformatted(textStart, textCur);
            textStart = textCur + 1;
        }
        
        ++textCur;
    }
    
    if(textCur != textStart)
    {
        ImGui::TextUnformatted(textStart, textCur);
    }
    else
    {
        ImGui::NewLine();
    }
    
    if(pushedColorStyle)
    {
        ImGui::PopStyleColor();
    }
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