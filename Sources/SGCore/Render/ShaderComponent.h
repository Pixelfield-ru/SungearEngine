//
// Created by ilya on 03.03.24.
//

#ifndef SUNGEARENGINE_SHADERCOMPONENT_H
#define SUNGEARENGINE_SHADERCOMPONENT_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/EventListener.h"
#include "ShadersUtils.h"

namespace SGCore
{
    class IShader;
    
    struct ShaderComponent
    {
        ShaderComponent();
        // ShaderComponent(const ShaderComponent&) = default;

        bool m_isCustomShader = false;
        
        Ref<IShader> m_shader = SGCore::MakeRef<IShader>();
        
        std::string m_shaderPath;
        
        EventListener<void()> m_onRenderPipelineSet = [this]() {
            ShadersUtils::onRenderPipelineSet(*this);
        };
    };
}

#endif //SUNGEARENGINE_SHADERCOMPONENT_H
