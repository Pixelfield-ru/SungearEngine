//
// Created by stuka on 13.12.2023.
//

#ifndef SUNGEARENGINE_DEVICEGLINFO_H
#define SUNGEARENGINE_DEVICEGLINFO_H

#include <cstddef>
#include <glad/glad.h>

#include "SGCore/Logging/Log.h"

namespace SGCore
{
    struct DeviceGLInfo
    {
        static void prepareInfo() noexcept
        {
            VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));

            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &MAX_FB_COLOR_ATTACHMENTS);
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &MAX_UNIFORM_BUFFER_BINDINGS);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_IMAGE_UNITS);
            glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &MAX_UNIFORMS);

            SGCF_INFO("============== DEVICE OPENGL INFO ==============", SG_LOG_GAPI_FILE);

            SGCF_INFO("GL_VERSION: " + VERSION + "\n", SG_LOG_GAPI_FILE);

            SGCF_INFO("GL_MAX_COLOR_ATTACHMENTS: " + std::to_string(MAX_FB_COLOR_ATTACHMENTS), SG_LOG_GAPI_FILE);
            SGCF_INFO("GL_MAX_UNIFORM_BUFFER_BINDINGS: " + std::to_string(MAX_UNIFORM_BUFFER_BINDINGS), SG_LOG_GAPI_FILE);
            SGCF_INFO("GL_MAX_TEXTURE_IMAGE_UNITS: " + std::to_string(MAX_TEXTURE_IMAGE_UNITS), SG_LOG_GAPI_FILE);
            SGCF_INFO("GL_MAX_UNIFORM_LOCATIONS: " + std::to_string(MAX_UNIFORMS), SG_LOG_GAPI_FILE);

            SGCF_INFO("================================================", SG_LOG_GAPI_FILE);
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

        [[maybe_unused]] [[nodiscard]] static auto getMaxUniforms() noexcept
        {
            return MAX_UNIFORMS;
        }

    private:
        static inline int MAX_FB_COLOR_ATTACHMENTS = 0;
        static inline int MAX_UNIFORM_BUFFER_BINDINGS = 0;
        static inline int MAX_TEXTURE_IMAGE_UNITS = 0;
        static inline int MAX_UNIFORMS = 0;

        static inline std::string VERSION;
    };
}

#endif //SUNGEARENGINE_DEVICEGLINFO_H
