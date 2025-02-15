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
    struct UIElementMesh;

    struct UIElement
    {
        virtual ~UIElement() = default;

        friend struct UIDocument;

        std::vector<Ref<UIElement>> m_children;
        Weak<UIElement> m_parent;

        AssetRef<CSSSelector> m_selector;

        AssetRef<IShader> m_shader;

        Ref<UIElementMesh> m_meshData;

        /**
         *
         * @param parentSelectorCache May be nullptr.
         * @param thisSelectorCache
         * @param parentTransform May be nullptr.
         * @param ownTransform
         */
        void calculateLayout(const CSSSelectorCache* parentSelectorCache, CSSSelectorCache& thisSelectorCache,
                             const Transform* parentTransform, Transform& ownTransform) noexcept;

        [[nodiscard]] UIElementType getType() const noexcept;

        virtual void useUniforms(const CSSSelectorCache& thisCSSSelectorCache) const noexcept;

    protected:
        virtual void doCalculateLayout(const CSSSelectorCache* parentSelectorCache, CSSSelectorCache& thisSelectorCache,
                                       const Transform* parentTransform, Transform& ownTransform) = 0;

        /**
         * Generates a mesh based on the selector (i.e. selector != nullptr).\n
         * You must implement this function in the UIElement derived classes.\n
         * You need to generate vertex positions, uv coordinates of vertices, and vertex colors.\n
         * Calls at the beginning of the calculateLayout function.\n
         * Calls only if mesh has not been generated before (i.e. mesh == nullptr).\n
         * IN THIS FUNCTION, YOU DO NOT NEED TO CREATE A MESH INSTANCE. YOU ONLY NEED TO GENERATE VERTICES.\n
         * POSITIONS OF ALL VERTICES MUST FIT TO RANGE OF -1, 1 BY X AND Y AXIS.
         */
        virtual void doGenerateMeshBaseSelector() noexcept = 0;

        /**
         * Generates basic mesh without selector (i.e. selector == nullptr).\n
         * You must implement this function in the UIElement derived classes.\n
         * You need to generate vertex positions, uv coordinates of vertices, and vertex colors.\n
         * Calls at the beginning of the calculateLayout function.\n
         * Calls only if mesh has not been generated before (i.e. mesh == nullptr).\n
         * IN THIS FUNCTION, YOU DO NOT NEED TO CREATE A MESH INSTANCE. YOU ONLY NEED TO GENERATE VERTICES.\n
         * POSITIONS OF ALL VERTICES MUST FIT TO RANGE OF -1, 1 BY X AND Y AXIS
         */
        virtual void doGenerateBasicMesh() noexcept = 0;

    private:
        UIElementType m_type = UIElementType::ET_UNKNOWN;

        void checkForMeshGenerating() noexcept;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
