//
// Created by stuka on 13.12.2023.
//

#ifndef SUNGEARENGINE_DEVICEGLINFO_H
#define SUNGEARENGINE_DEVICEGLINFO_H

#include <iostream>
#include <glad/glad.h>
#include <set>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/Macroses.h"

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
            const auto formatted = fmt::format("GL CALLBACK: {} type = 0x{}, severity = 0x{}, message = {}\n",
                                               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                                               type, severity, message);

            /*if(formatted.contains("GL_INVALID_OPERATION error generated. <location> is invalid."))
            {
                std::cout << Utils::getTimeAsString("%Y-%m-%d %H:%M:%S") << ", bipka" << std::endl;
                return;
            }*/

            LOG_E_UNFORMATTED(SGCORE_TAG, formatted)
            // std::cerr << formatted << std::endl;
            // fprintf( stderr, formatted.c_str());
        }
        
        static void init() noexcept
        {
            // todo:
            /*glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(messageCallback, 0);*/

            GLint extensionsNum = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);
            
            LOG_I(SGCORE_TAG, "OpenGL supporting extensions count: {}", extensionsNum);
            LOG_I(SGCORE_TAG, "OpenGL supporting extensions:");
            
            for(int i = 0; i < extensionsNum; i++)
            {
                const char* extName = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
                s_supportingExtensions.insert(extName);
                LOG_I_UNFORMATTED(SGCORE_TAG, extName);
            }
            
            VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));

            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &MAX_FB_COLOR_ATTACHMENTS);
            glGetIntegerv(GL_MAX_DRAW_BUFFERS, &MAX_DRAW_BUFFERS);
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &MAX_UNIFORM_BUFFER_BINDINGS);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_IMAGE_UNITS);
            glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &MAX_TEXTURE_BUFFER_SIZE);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MAX_TEXTURE_SIZE);
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MAX_VERTEX_ATTRIBS);

            if(s_supportingExtensions.contains(SG_STRINGIFY(GL_EXT_texture_filter_anisotropic)))
            {
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MAX_TEXTURE_MAX_ANISOTROPY);
            }
            else
            {
                MAX_TEXTURE_MAX_ANISOTROPY = 0.0f;
            }

            LOG_I(SGCORE_TAG, "============== DEVICE OPENGL INFO ==============");

            LOG_I(SGCORE_TAG, "GL_VERSION: {}\n", VERSION);

            LOG_I(SGCORE_TAG, "GL_MAX_COLOR_ATTACHMENTS: {}", MAX_FB_COLOR_ATTACHMENTS);
            LOG_I(SGCORE_TAG, "GL_MAX_DRAW_BUFFERS: {}", MAX_DRAW_BUFFERS);
            LOG_I(SGCORE_TAG, "GL_MAX_UNIFORM_BUFFER_BINDINGS: {}", MAX_UNIFORM_BUFFER_BINDINGS);
            LOG_I(SGCORE_TAG, "GL_MAX_TEXTURE_IMAGE_UNITS: {}", MAX_TEXTURE_IMAGE_UNITS);
            LOG_I(SGCORE_TAG, "GL_MAX_TEXTURE_MAX_ANISOTROPY: {}", MAX_TEXTURE_MAX_ANISOTROPY);
            LOG_I(SGCORE_TAG, "GL_MAX_TEXTURE_BUFFER_SUZE: {}", MAX_TEXTURE_BUFFER_SIZE);
            LOG_I(SGCORE_TAG, "GL_MAX_TEXTURE_SIZE: {}", MAX_TEXTURE_SIZE);
            LOG_I(SGCORE_TAG, "GL_MAX_VERTEX_ATTRIBS: {}", MAX_VERTEX_ATTRIBS);

            LOG_I(SGCORE_TAG, "================================================");
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxFBColorAttachments() noexcept
        {
            return MAX_FB_COLOR_ATTACHMENTS;
        }

        [[maybe_unused]] [[nodiscard]] static auto getMaxDrawBuffers() noexcept
        {
            return MAX_DRAW_BUFFERS;
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

        [[maybe_unused]] [[nodiscard]] static auto getMaxVertexAttribsCount() noexcept
        {
            return MAX_VERTEX_ATTRIBS;
        }
        
        static const auto& getSupportingExtensions() noexcept
        {
            return s_supportingExtensions;
        }

    private:
        static inline std::set<std::string> s_supportingExtensions;
        
        static inline int MAX_FB_COLOR_ATTACHMENTS = 0;
        static inline int MAX_DRAW_BUFFERS = 0;
        static inline int MAX_UNIFORM_BUFFER_BINDINGS = 0;
        static inline int MAX_TEXTURE_IMAGE_UNITS = 0;
        static inline float MAX_TEXTURE_MAX_ANISOTROPY = 0.0f;
        static inline int MAX_TEXTURE_BUFFER_SIZE = 0;
        static inline int MAX_TEXTURE_SIZE = 0;
        static inline int MAX_VERTEX_ATTRIBS = 0;

        static inline std::string VERSION;
    };
}

#endif //SUNGEARENGINE_DEVICEGLINFO_H
