//
// Created by stuka on 13.12.2023.
//

#ifndef SUNGEARENGINE_DEVICEGLINFO_H
#define SUNGEARENGINE_DEVICEGLINFO_H

#include <cstddef>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

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

            spdlog::info("============== DEVICE OPENGL INFO ==============");

            spdlog::info("GL_VERSION: {0}\n", VERSION);

            spdlog::info("GL_MAX_COLOR_ATTACHMENTS: {0}", MAX_FB_COLOR_ATTACHMENTS);
            spdlog::info("GL_MAX_UNIFORM_BUFFER_BINDINGS: {0}", MAX_UNIFORM_BUFFER_BINDINGS);
            spdlog::info("GL_MAX_TEXTURE_IMAGE_UNITS: {0}", MAX_TEXTURE_IMAGE_UNITS);

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

    private:
        static inline int MAX_FB_COLOR_ATTACHMENTS = 0;
        static inline int MAX_UNIFORM_BUFFER_BINDINGS = 0;
        static inline int MAX_TEXTURE_IMAGE_UNITS = 0;

        static inline std::string VERSION;
    };
}

#endif //SUNGEARENGINE_DEVICEGLINFO_H
