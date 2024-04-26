//
// Created by ilya on 27.03.24.
//

#include <SGUtils/Singleton.h>
#include <SGCore/ImGuiWrap/ViewsInjector.h>
#include "MainView.h"
#include "PluginMain.h"

SGE::MainView::MainView()
{
    m_topToolbarView = SGCore::MakeRef<TopToolbarView>();
    
    mainViewsInjector["SGE_RootView"]; //.m_childrenViews.push_back(m_topToolbarView);
}

SGE::MainView::~MainView()
{
    mainViewsInjector.m_childrenInjectors.erase("SGE_RootView");
    std::cout << "erased" << std::endl;
}
