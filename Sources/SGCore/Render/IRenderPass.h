//
// Created by stuka on 24.11.2023.
//

#pragma once

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/RenderState.h"

namespace SGCore
{
    struct IRenderPipeline;
    class Scene;

    struct EntityBaseInfo;
    struct Camera3D;
    class LayeredFrameReceiver;
    struct RenderingBase;
    struct Transform;
    struct CSMTarget;

    struct IRenderPass
    {
        virtual ~IRenderPass() = default;

        friend struct IRenderPipeline;

        bool m_active = true;

        AssetRef<IShader> m_shader;

        RenderState m_renderState;
        MeshRenderState m_meshRenderState;

        std::vector<Weak<IUniformBuffer>> m_uniformBuffersToUse;

        virtual void create(const Ref<IRenderPipeline>& parentRenderPipeline) { }
        
        virtual void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) = 0;

        void bindUniformBuffers(IShader* toShader) noexcept;

        float getExecutionTime() const noexcept
        {
            return m_executionTime;
        }

    protected:
        struct CameraRenderingInfo
        {
            /// Always valid.
            ECS::entity_t m_cameraEntity {};
            /// Always not nullptr & always valid.
            const EntityBaseInfo* m_cameraInfo {};
            /// Always not nullptr & always valid.
            const Camera3D* m_camera3D {};
            /// Always not nullptr & always valid.
            LayeredFrameReceiver* m_cameraFrameReceiver {};
            /// Always not nullptr & always valid.
            const RenderingBase* m_cameraRenderingBase {};
            /// Always not nullptr & always valid.
            const Transform* m_cameraTransform {};
            /// Can be nullptr.
            const CSMTarget* m_cameraCSMTarget {};
        };

        void iterateCameras(const Scene* scene, const std::function<void(CameraRenderingInfo&)>& func) noexcept;

    private:
        float m_executionTime = 0.0f;
    };
}
