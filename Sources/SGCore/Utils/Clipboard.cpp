//
// Created by stuka on 07.08.2026.
//

#include "Clipboard.h"

#include "Platform.h"

#if SG_PLATFORM_OS_WINDOWS
#include <shellapi.h>
#include <shlobj.h>
#endif

#include "Utils.h"

void SGCore::Clipboard::setText(const std::string& text) noexcept
{
    setData(MimeType::text, text.c_str(), text.size() + 1);
}

std::string SGCore::Clipboard::getText() const noexcept
{
    const auto data = getData(MimeType::text);
    return { data.begin(), data.end() };
}

void SGCore::Clipboard::setImage(const std::uint8_t* imageData, std::size_t imageByteSize) noexcept
{
    setData(MimeType::jpeg, reinterpret_cast<const char*>(imageData), imageByteSize);
}

std::vector<std::uint8_t> SGCore::Clipboard::getImage() const noexcept
{
    const auto data = getData(MimeType::jpeg);
    return { data.begin(), data.end() };
}

void SGCore::Clipboard::setFiles(const std::vector<std::filesystem::path>& files) noexcept
{
#if SG_PLATFORM_OS_WINDOWS
    if(files.empty())
    {
        return;
    }

    // double-null-terminated list of wide paths for DROPFILES
    std::wstring paths;
    for(const auto& file : files)
    {
        paths += file.wstring();
        paths.push_back(L'\0');
    }
    paths.push_back(L'\0');

    const SIZE_T bytesSize = sizeof(DROPFILES) + paths.size() * sizeof(wchar_t);
    auto memHandle = GlobalAlloc(GMEM_MOVEABLE, bytesSize);
    if(!memHandle)
    {
        return;
    }

    auto* dropFiles = static_cast<DROPFILES*>(GlobalLock(memHandle));
    if(!dropFiles)
    {
        GlobalFree(memHandle);
        return;
    }

    dropFiles->pFiles = sizeof(DROPFILES);
    dropFiles->pt = { 0, 0 };
    dropFiles->fNC = FALSE;
    dropFiles->fWide = TRUE;

    std::memcpy(reinterpret_cast<BYTE*>(dropFiles) + sizeof(DROPFILES),
                paths.data(),
                paths.size() * sizeof(wchar_t));

    GlobalUnlock(memHandle);

    if(!OpenClipboard(nullptr))
    {
        GlobalFree(memHandle);
        return;
    }

    EmptyClipboard();

    if(!SetClipboardData(CF_HDROP, memHandle))
    {
        GlobalFree(memHandle);
    }

    CloseClipboard();
#elif SG_PLATFORM_OS_LINUX
#endif
}

std::vector<std::filesystem::path> SGCore::Clipboard::getFiles() const noexcept
{
#if SG_PLATFORM_OS_WINDOWS
    if(!OpenClipboard(nullptr))
    {
        return {};
    }

    auto dataHandle = GetClipboardData(CF_HDROP);
    if(!dataHandle)
    {
        CloseClipboard();
        return {};
    }

    const auto hDrop = static_cast<HDROP>(dataHandle);
    const UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);

    std::vector<std::filesystem::path> result;
    result.reserve(fileCount);

    for(UINT i = 0; i < fileCount; ++i)
    {
        const UINT length = DragQueryFileW(hDrop, i, nullptr, 0);
        std::wstring path(length, L'\0');
        DragQueryFileW(hDrop, i, path.data(), length + 1);
        result.emplace_back(std::move(path));
    }

    CloseClipboard();
    return result;
#elif SG_PLATFORM_OS_LINUX
    return {};
#else
    return {};
#endif
}

SGCore::Clipboard& SGCore::Clipboard::instance() noexcept
{
    static Clipboard clipboard;
    return clipboard;
}

void SGCore::Clipboard::setData(const std::string& mimeType, const char* data, std::size_t size) noexcept
{
#if SG_PLATFORM_OS_WINDOWS
    if(!OpenClipboard(nullptr))
    {
        return;
    }

    UINT winMimeType;

    const auto winMimeTypeIt = m_winMimesMapping.find(mimeType);
    if(winMimeTypeIt == m_winMimesMapping.end())
    {
        winMimeType = RegisterClipboardFormat(mimeType.c_str());
        m_winMimesMapping[mimeType] = winMimeType;
    }
    else winMimeType = winMimeTypeIt->second;

    auto memHandle = GlobalAlloc(GMEM_MOVEABLE, size);
    if(!memHandle)
    {
        CloseClipboard();
        return;
    }

    auto* memory = static_cast<char*>(GlobalLock(memHandle));
    std::memcpy(memory, data, size);
    GlobalUnlock(memHandle);

    SetClipboardData(winMimeType, memory);

    CloseClipboard();
#elif SG_PLATFORM_OS_LINUX
#endif
}

std::vector<char> SGCore::Clipboard::getData(const std::string& mimeType) const noexcept
{
#if SG_PLATFORM_OS_WINDOWS
    if(!OpenClipboard(nullptr))
    {
        return {};
    }

    const auto winMimeTypeIt = m_winMimesMapping.find(mimeType);
    if(winMimeTypeIt == m_winMimesMapping.end())
    {
        CloseClipboard();
        return {};
    }

    auto dataHandle = GetClipboardData(winMimeTypeIt->second);
    if(!dataHandle)
    {
        CloseClipboard();
        return {};
    }

    const std::size_t size = GlobalSize(dataHandle);

    char* data = static_cast<char*>(GlobalLock(dataHandle));
    if(!data)
    {
        CloseClipboard();
        return {};
    }

    std::vector result(data, data + size);

    GlobalUnlock(data);
    CloseClipboard();

    return result;
#elif SG_PLATFORM_OS_LINUX
    return {};
#endif
}
