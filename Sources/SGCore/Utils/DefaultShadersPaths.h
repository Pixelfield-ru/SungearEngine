//
// Created by ilya on 16.04.24.
//

#ifndef SUNGEARENGINE_DEFAULTSHADERPATHES_H
#define SUNGEARENGINE_DEFAULTSHADERPATHES_H

#include "Utils.h"
#include "SGCore/Main/CoreMain.h"

#include "ShadersPaths.h"

namespace SGCore
{
    struct DefaultShadersPaths
    {
        static void init()
        {
            m_paths["Gizmos"]["ComplexGizmosShader"].m_GLSL4RealizationPath = CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/gizmos/complex_gizmos_shader.glsl";
            
            m_paths["Gizmos"]["LinesGizmosShader"].m_GLSL4RealizationPath = CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/gizmos/lines_shader.glsl";
            
            m_paths["LayeredPP"]["ReceiverShader"].m_GLSL4RealizationPath = CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/receiver_shader.glsl";
            m_paths["LayeredPP"]["LayerFXShader"].m_GLSL4RealizationPath = CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/layer_default_fx.glsl";
            m_paths["LayeredPP"]["FinalFrameFXShader"].m_GLSL4RealizationPath = CoreMain::getSungearEngineRootPath() / "Resources/shaders/glsl4/postprocessing/layered/final_frame_fx.glsl";
        }
        
        SG_NOINLINE static auto& getPaths() noexcept
        {
            return m_paths;
        }
        
    private:
        static inline ShadersPaths m_paths;
    };
}

#endif // SUNGEARENGINE_DEFAULTSHADERPATHES_H
