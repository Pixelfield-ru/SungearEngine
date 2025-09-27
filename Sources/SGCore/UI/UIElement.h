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
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Utils/Unique/UniqueColor.h"

namespace SGCore::UI
{
    struct UIElementMesh;
    struct UIDocument;

    struct UIElement
    {
        virtual ~UIElement() = default;

        friend struct UIDocument;

        std::vector<Ref<UIElement>> m_children;
        Weak<UIElement> m_parent;

        // =================== XML ATTRIBUTES
        AssetRef<CSSSelector> m_selector;
        std::string m_name;
        // ===================

        AssetRef<IShader> m_shader;
        Ref<UIElementMesh> m_meshData;
        UniqueColor m_uniqueColor;

        /**
         * 
         * @return False if was not rendered, true - rendered.
         */
        virtual bool draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                          const Transform& elementTransform,
                          UIElementCache& elementCache) noexcept { return false; }
        
        /**
         *
         * @param parentElementCache May be nullptr.
         * @param thisElementCache
         * @param parentTransform May be nullptr.
         * @param ownTransform
         */
        void calculateLayout(const UIElementCache* parentElementCache,
                             const UIElementCache& thisLastElementCache,
                             UIElementCache& thisElementCache,
                             const Transform* parentTransform,
                             Transform& ownTransform) noexcept;

        [[nodiscard]] size_t getTypeHash() const noexcept;

        virtual void useUniforms(UIElementCache& thisElementCache) const noexcept;

        void regenerateMesh(const UIElementCache* parentElementCache,
                            UIElementCache& thisElementCache) noexcept;

        Ref<UIElement> findElement(const std::string& name) noexcept;

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, UIElement*, UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) noexcept
        {
            func(m_parent.lock().get(), this);
            if(breakToken) return;

            for(const auto& child : m_children)
            {
                child->iterate(func, breakToken);
                if(breakToken) return;
            }
        }

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, const UIElement*, const UIElement*>)
        void iterate(const FuncT& func, bool& breakToken) const noexcept
        {
            func(m_parent.lock().get(), this);
            if(breakToken) return;

            for(const auto& child : m_children)
            {
                child->iterate(func);
                if(breakToken) return;
            }
        }

    protected:
        virtual void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                                       const Transform* parentTransform, Transform& ownTransform) = 0;

        /**
         * Generates a mesh based on the selector (i.e. selector != nullptr).\n
         * You must implement this function in the UIElement derived classes.\n
         * You need to generate vertex positions, uv coordinates of vertices, and vertex colors.\n
         * Calls at the beginning of the calculateLayout function.\n
         * Calls only if mesh has not been generated before (i.e. mesh == nullptr).\n
         * IN THIS FUNCTION, YOU DO NOT NEED TO CREATE A MESH INSTANCE. YOU ONLY NEED TO GENERATE VERTICES.
         */
        virtual void doGenerateMeshBaseSelector(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept = 0;

        /**
         * Generates basic mesh without selector (i.e. selector == nullptr).\n
         * You must implement this function in the UIElement derived classes.\n
         * You need to generate vertex positions, uv coordinates of vertices, and vertex colors.\n
         * Calls at the beginning of the calculateLayout function.\n
         * Calls only if mesh has not been generated before (i.e. mesh == nullptr).\n
         * IN THIS FUNCTION, YOU DO NOT NEED TO CREATE A MESH INSTANCE. YOU ONLY NEED TO GENERATE VERTICES.
         */
        virtual void doGenerateBasicMesh() noexcept = 0;

    private:
        size_t m_typeHash = 0;

        void checkForMeshGenerating(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
