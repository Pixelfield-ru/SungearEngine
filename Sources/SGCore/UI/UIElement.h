//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIELEMENT_H
#define SUNGEARENGINE_UIELEMENT_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Transformations/Transform.h"
#include "CSS/CSSSelector.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Graphics/API/IShader.h"
#include "UIElementType.h"

namespace SGCore::UI
{
    struct UIElement
    {
        friend struct UIDocument;

        std::vector<glm::vec3> m_verticesPositions;
        std::vector<glm::vec3> m_verticesUV;
        std::vector<glm::vec4> m_verticesColors;

        std::vector<Ref<UIElement>> m_children;
        Weak<UIElement> m_parent;

        AssetRef<CSSSelector> m_selector;

        AssetRef<IShader> m_shader;

        virtual void calculateLayout() noexcept = 0;

        [[nodiscard]] UIElementType getType() const noexcept;

    private:
        UIElementType m_type = UIElementType::ET_UNKNOWN;

        Transform m_transform;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
