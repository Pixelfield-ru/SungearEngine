//
// Created by Ilya on 24.07.2024.
//

#include "SelectedToolchainDockedWindow.h"

void SGE::SelectedToolchainDockedWindow::renderBody()
{
    if(m_selectedToolchain)
    {
        ImGui::Text(m_selectedToolchain->m_name.getName().c_str());
    }

    //std::printf("sfsfsdfsdf\n");
}
