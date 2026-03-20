//
// Created by stuka on 26.11.2024.
//

#pragma once

#include "PostProcessFXSubPass.h"
#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    struct PostProcessLayer;

    struct SGCORE_EXPORT PostProcessEffect
    {
        virtual ~PostProcessEffect() = default;

        friend struct PostProcessLayer;

        std::vector<PostProcessFXSubPass> m_subPasses;

        std::vector<std::pair<std::string, SGFrameBufferAttachmentType>> m_usedAttachments;

        std::string m_name;

        virtual void onAttachToLayer(const Ref<PostProcessLayer>& toLayer);
        virtual void onFXPass(const Ref<PostProcessLayer>& currentLayer) { }
        virtual void onDetachFromLayer(const Ref<PostProcessLayer>& fromLayer) { }

        virtual void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept { }

        virtual void passValuesToSubPassShader() noexcept { }

        AssetRef<IShader> getShader() const noexcept;
        void setShader(const AssetRef<IShader>& shader) noexcept;

        [[nodiscard]] bool isEnabled() const noexcept;
        void setEnabled(bool isEnabled) noexcept;

    protected:
        bool m_isEnabled = true;

        Slot<void(IAsset*)> onLayerShaderChangedSlot;

        AssetRef<IShader> m_effectShader;

        std::vector<Weak<PostProcessLayer>> m_parentPostProcessLayers;
    };
}
