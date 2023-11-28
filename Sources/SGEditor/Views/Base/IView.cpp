//
// Created by stuka on 28.11.2023.
//

#include "IView.h"

void SGEditor::IView::renderSubViews() const noexcept
{
    for(const auto& subView : m_subViews)
    {
        subView->render();
    }
}
