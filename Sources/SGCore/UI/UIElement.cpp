//
// Created by stuka on 25.01.2025.
//
#include "UIElement.h"

#include "SGCore/ImportedScenesArch/IMeshData.h"

SGCore::UI::UIElementType SGCore::UI::UIElement::getType() const noexcept
{
    return m_type;
}

void SGCore::UI::UIElement::calculateLayout(const Transform& parentTransform, Transform& ownTransform) noexcept
{
    checkForMeshGenerating();

    doCalculateLayout(parentTransform, ownTransform);
}

void SGCore::UI::UIElement::checkForMeshGenerating() noexcept
{
    if(!m_meshData)
    {
        m_meshData = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());

        if(m_selector)
        {
            doGenerateMeshBaseSelector();
        }
        else
        {
            doGenerateBasicMesh();
        }
    }
}

