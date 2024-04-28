//
// Created by ilya on 28.04.24.
//

#include "Resources.h"

void SGE::Resources::releaseResources() noexcept
{
    for(auto& font : m_imguiFonts)
    {
        delete font;
        // ImGui::GetIO().Fonts.Clea
    }
    
    m_imguiFonts.clear();
}

std::vector<ImFont*>& SGE::Resources::getImGuiFonts() noexcept
{
    return m_imguiFonts;
}

