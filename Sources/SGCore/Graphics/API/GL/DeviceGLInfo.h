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
        static void GLAPIENTRY messageCallback(GLenum source,
                                               GLenum type,
                                               GLuint id,
                                               GLenum severity,
                                               GLsizei length,
                                               const GLchar* message,
                                               const void* userParam)
        {
            /*spdlog::error("GL CALLBACK: {0} type = 0x{1}, severity = 0x{2}, message = {3}\n",
                     (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                     type, severity, message);*/
            /*fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                     ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                     type, severity, message );*/
        }
        
        static void init() noexcept
        {
            // todo:
            // glEnable(GL_DEBUG_OUTPUT);
            // glDebugMessageCallback(messageCallback, 0);
            
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
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &MAX_TEXTURE_BUFFER_SIZE);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MAX_TEXTURE_SIZE);
            
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
            spdlog::info("GL_MAX_TEXTURE_BUFFER_SUZE: {0}", MAX_TEXTURE_BUFFER_SIZE);
            spdlog::info("GL_MAX_TEXTURE_SIZE: {0}", MAX_TEXTURE_SIZE);

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

        [[maybe_unused]] [[nodiscard]] static auto getMaxTextureBufferSize() noexcept
        {
            return MAX_TEXTURE_BUFFER_SIZE;
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxTextureSize() noexcept
        {
            return MAX_TEXTURE_SIZE;
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
        static inline int MAX_TEXTURE_BUFFER_SIZE = 0;
        static inline int MAX_TEXTURE_SIZE = 0;

        static inline std::string VERSION;
    };
}

#endif //SUNGEARENGINE_DEVICEGLINFO_H
