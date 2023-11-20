//
// Created by stuka on 09.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERASSET_H
#define NATIVECORE_SHADERASSET_H

#include "IAsset.h"
#include "SGCore/Utils/FileUtils.h"

namespace SGCore
{
    class FileAsset : public IAsset, public std::enable_shared_from_this<FileAsset>
    {
    private:
        std::string m_data;

    public:
        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        [[nodiscard]] std::string getData() const noexcept;
    };
}

#endif //NATIVECORE_SHADERASSET_H
