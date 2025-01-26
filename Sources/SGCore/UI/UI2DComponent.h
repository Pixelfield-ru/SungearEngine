//
// Created by stuka on 26.01.2025.
//

#ifndef SUNGEARENGINE_UI2DCOMPONENT_H
#define SUNGEARENGINE_UI2DCOMPONENT_H

#include "SGCore/ECS/Component.h"
#include "UIDocument.h"

namespace SGCore::UI
{
    struct UI2DComponent : ECS::Component<UI2DComponent, const UI2DComponent>
    {
        AssetRef<UIDocument> m_document;
    };
}

#endif //SUNGEARENGINE_UI2DCOMPONENT_H
