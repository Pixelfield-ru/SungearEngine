//
// Created by ilya on 27.03.24.
//

#include <SGUtils/Singleton.h>
#include <SGCore/Main/CoreMain.h>

#include "MainView.h"
#include "PluginMain.h"

SGE::MainView::MainView()
{
    m_topToolbarView = SGCore::MakeRef<TopToolbarView>();
    m_topToolbarView->m_name = "SGE_TOP_TOOLBAR";
    
    addChild(m_topToolbarView);
}

SGE::MainView::~MainView()
{
    removeChild(m_topToolbarView);
}

bool SGE::MainView::begin()
{
    return true;
}

void SGE::MainView::renderBody()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    /*ImGui::Begin("test");
    ImGui::Text("hello!");
    ImGui::End();*/
}

void SGE::MainView::end()
{
    IView::end();
}
