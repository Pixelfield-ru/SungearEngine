//
// Created by stuka on 28.11.2023.
//

#include "ViewsManager.h"

void SGEditor::ViewsManager::renderRootViews() const noexcept
{
    for(const auto& rootView : m_rootViews)
    {
        rootView->render();
    }
}
