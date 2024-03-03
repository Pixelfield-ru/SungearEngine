//
// Created by ilya on 03.03.24.
//

#ifndef SUNGEARENGINE_SHADERCOMPONENT_H
#define SUNGEARENGINE_SHADERCOMPONENT_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/EventListener.h"
#include "ShadersUtils.h"

namespace SGCore
{
    class IShader;
    
    struct ShaderComponent
    {
        ShaderComponent();
        // ShaderComponent(const ShaderComponent&) = default;
        
        Ref<IShader> m_shader = SGCore::MakeRef<SGCore::IShader>();
        
        std::string m_shaderPath;
        
        EventListener<void()> m_onRenderPipelineSet = MakeEventListener<void()>([this]() { ShadersUtils::onRenderPipelineSet(*this); });
    };
}

#endif //SUNGEARENGINE_SHADERCOMPONENT_H
