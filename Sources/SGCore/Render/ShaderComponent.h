//
// Created by ilya on 03.03.24.
//

#ifndef SUNGEARENGINE_SHADERCOMPONENT_H
#define SUNGEARENGINE_SHADERCOMPONENT_H

#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/EventListener.h"
#include "ShadersUtils.h"

namespace SGCore
{
    class IShader;
    
    struct ShaderComponent
    {
        sg_serde_as_friend()

        friend struct ShadersUtils;

        ShaderComponent();
        // ShaderComponent(const ShaderComponent&) = default;

        bool m_isCustomShader = false;
        
        AssetRef<IShader> m_shader;
        
        EventListener<void()> m_onRenderPipelineSet = [this]() {
            ShadersUtils::onRenderPipelineSet(*this);
        };

    private:
        std::string m_shaderPath;
    };
}

#endif //SUNGEARENGINE_SHADERCOMPONENT_H
