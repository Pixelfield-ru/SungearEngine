//
// Created by stuka on 07.08.2026.
//

#pragma once

#include <span>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "MimeType.h"
#include "Platform.h"
#include "sgcore_export.h"

#if SG_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif

namespace SGCore
{
    struct SGCORE_EXPORT Clipboard
    {
        void setText(const std::string& text) noexcept;
        [[nodiscard]] std::string getText() const noexcept;

        void setImage(const std::uint8_t* imageData, std::size_t imageByteSize) noexcept;
        [[nodiscard]] std::vector<std::uint8_t> getImage() const noexcept;

        void setFiles(const std::vector<std::filesystem::path>& files) noexcept;
        [[nodiscard]] std::vector<std::filesystem::path> getFiles() const noexcept;

        static Clipboard& instance() noexcept;

    private:
#if SG_PLATFORM_OS_WINDOWS
        std::unordered_map<std::string, UINT> m_winMimesMapping {
            { MimeType::text, CF_TEXT },
            { MimeType::jpeg, CF_BITMAP },
            { MimeType::mp3, CF_RIFF },
            { MimeType::ogg, CF_RIFF },
            { MimeType::wav, CF_WAVE }
        };
#endif

        void setData(const std::string& mimeType, const char* data, std::size_t size) noexcept;
        [[nodiscard]] std::vector<char> getData(const std::string& mimeType) const noexcept;
    };
}
