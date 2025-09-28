//
// Created by stuka on 26.01.2025.
//

#ifndef SUNGEARENGINE_UICOMPONENT_H
#define SUNGEARENGINE_UICOMPONENT_H

#include "SGCore/ECS/Component.h"
#include "UIDocument.h"
#include "SGCore/UI/TransformTree/UITransformTree.h"
#include "SGCore/Scene/EntityRef.h"

namespace SGCore::UI
{
    struct UIComponent : ECS::Component<UIComponent, const UIComponent>
    {
        EntityRef m_attachedToCamera;

        AssetRef<UIDocument> m_document;

        UITransformTree m_transformTree;
    };
}

#endif //SUNGEARENGINE_UICOMPONENT_H
