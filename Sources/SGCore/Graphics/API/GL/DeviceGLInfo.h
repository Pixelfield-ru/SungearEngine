//
// Created by stuka on 13.12.2023.
//

#ifndef SUNGEARENGINE_DEVICEGLINFO_H
#define SUNGEARENGINE_DEVICEGLINFO_H

#include <cstddef>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "SGUtils/Utils.h"

namespace SGCore
{
    struct DeviceGLInfo
    {
        static void prepareInfo() noexcept
        {
            GLint extensionsNum = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);
            
            spdlog::info("OpenGL supporting extensions count: {0}", extensionsNum);
            spdlog::info("OpenGL supporting extensions:");
            
            for(int i = 0; i < extensionsNum; i++)
            {
                const char* extName = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
                s_supportingExtensions.insert(extName);
                spdlog::info(extName);
            }
            
            VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));

            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &MAX_FB_COLOR_ATTACHMENTS);
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &MAX_UNIFORM_BUFFER_BINDINGS);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_IMAGE_UNITS);
            
            if(s_supportingExtensions.contains(SG_STRINGIFY(GL_EXT_texture_filter_anisotropic)))
            {
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MAX_TEXTURE_MAX_ANISOTROPY);
            }
            else
            {
                MAX_TEXTURE_MAX_ANISOTROPY = 0.0f;
            }

            spdlog::info("============== DEVICE OPENGL INFO ==============");

            spdlog::info("GL_VERSION: {0}\n", VERSION);

            spdlog::info("GL_MAX_COLOR_ATTACHMENTS: {0}", MAX_FB_COLOR_ATTACHMENTS);
            spdlog::info("GL_MAX_UNIFORM_BUFFER_BINDINGS: {0}", MAX_UNIFORM_BUFFER_BINDINGS);
            spdlog::info("GL_MAX_TEXTURE_IMAGE_UNITS: {0}", MAX_TEXTURE_IMAGE_UNITS);
            
            spdlog::info("GL_MAX_TEXTURE_MAX_ANISOTROPY: {0}", MAX_TEXTURE_MAX_ANISOTROPY);

            spdlog::info("================================================");
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxFBColorAttachments() noexcept
        {
            return MAX_FB_COLOR_ATTACHMENTS;
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxUniformBufferBindings() noexcept
        {
            return MAX_UNIFORM_BUFFER_BINDINGS;
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxTextureImageUnits() noexcept
        {
            return MAX_TEXTURE_IMAGE_UNITS;
        }

        [[maybe_unused]] [[nodiscard]] static auto getVersion() noexcept
        {
            return VERSION;
        }
        
        [[maybe_unused]] [[nodiscard]] static auto getMaxTextureMaxAnisotropy() noexcept
        {
            return MAX_TEXTURE_MAX_ANISOTROPY;
        }
        
        static const auto& getSupportingExtensions() noexcept
        {
            return s_supportingExtensions;
        }

    private:
        static inline std::set<std::string> s_supportingExtensions;
        
        static inline int MAX_FB_COLOR_ATTACHMENTS = 0;
        static inline int MAX_UNIFORM_BUFFER_BINDINGS = 0;
        static inline int MAX_TEXTURE_IMAGE_UNITS = 0;
        
        static inline float MAX_TEXTURE_MAX_ANISOTROPY = 0.0f;

        static inline std::string VERSION;
    };
}

#endif //SUNGEARENGINE_DEVICEGLINFO_H
