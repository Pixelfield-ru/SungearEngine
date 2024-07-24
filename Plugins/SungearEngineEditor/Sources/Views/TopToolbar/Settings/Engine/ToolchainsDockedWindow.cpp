//
// Created by Ilya on 22.07.2024.
//

#include "ToolchainsDockedWindow.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"

SGE::ToolchainsDockedWindow::ToolchainsDockedWindow()
{
    m_toolchainsVariantsPopup = Popup {
            "ToolchainsVariantsPopup",
            {
                    {
                            .m_name = "Visual Studio",
                            .m_icon = StylesManager::getCurrentStyle()
                                    ->m_visualStudioIcon->getSpecialization(16, 16)->getTexture(),
                            .m_drawSeparatorAfter = false
                    },
                    {
                            .m_name = "MinGW",
                            .m_icon = StylesManager::getCurrentStyle()
                                    ->m_questionIcon->getSpecialization(16, 16)->getTexture(),
                            .m_drawSeparatorAfter = false
                    }
            }
    };

    m_toolchainsVariantsPopup.onElementClicked += [this](PopupElement& element) {
        if(element.m_name == "Visual Studio")
        {
            m_currentAddedToolchainsTree.addTreeNode({
                .m_text = "Visual Studio",
                .m_icon = StylesManager::getCurrentStyle()
                    ->m_visualStudioIcon->getSpecialization(16, 16)->getTexture(),
                .onClicked = [this](TreeNode& self) {

                },
                .m_useNameAsText = true
            });
        }
    };
}

void SGE::ToolchainsDockedWindow::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 4 });
    ImGui::BeginChildFrame(ImGui::GetID((m_name.getName() + "TopToolbar").c_str()),
                           { ImGui::GetContentRegionAvail().x, 35 },
                           ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 7 });

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 0 });
    m_toolchainsVariantsPopup.draw();
    ImGui::PopStyleVar();

    auto plusIconSpec = StylesManager::getCurrentStyle()->m_plusIcon->getSpecialization(20, 20);
    if(ImGuiUtils::ImageButton(plusIconSpec->getTexture()->getTextureNativeHandler(), { 26, 26 }, { 20, 20 }).m_isLMBClicked)
    {
        m_toolchainsVariantsPopup.setOpened(true);
    }

    ImGui::SameLine();

    auto minusIconSpec = StylesManager::getCurrentStyle()->m_minusIcon->getSpecialization(20, 20);
    if(ImGuiUtils::ImageButton(minusIconSpec->getTexture()->getTextureNativeHandler(), { 26, 26 }, { 20, 20 }).m_isLMBClicked)
    {

    }

    ImGui::SameLine();

    auto arrowUpIconSpec = StylesManager::getCurrentStyle()->m_arrowUpIcon->getSpecialization(20, 20);
    if(ImGuiUtils::ImageButton(arrowUpIconSpec->getTexture()->getTextureNativeHandler(), { 26, 26 }, { 20, 20 }).m_isLMBClicked)
    {

    }

    ImGui::SameLine();

    auto arrowDownIconSpec = StylesManager::getCurrentStyle()->m_arrowDownIcon->getSpecialization(20, 20);
    if(ImGuiUtils::ImageButton(arrowDownIconSpec->getTexture()->getTextureNativeHandler(), { 26, 26 }, { 20, 20 }).m_isLMBClicked)
    {

    }

    ImGui::PopStyleVar();
    ImGui::EndChildFrame();
    ImGui::PopStyleVar();

    ImGui::Separator();
    // ===============================================================================

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 7 });
    ImGui::BeginChildFrame(ImGui::GetID((m_name.getName() + "Toolchains").c_str()),
                           ImGui::GetContentRegionAvail(),
                           ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 7 });

    // ImGui::Text("hello");
    m_currentAddedToolchainsTree.draw({ m_UIScale.x, m_UIScale.y });

    ImGui::PopStyleVar();
    ImGui::EndChildFrame();
    ImGui::PopStyleVar();

}
