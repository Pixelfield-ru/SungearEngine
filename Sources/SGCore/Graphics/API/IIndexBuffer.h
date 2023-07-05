//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IINDEXBUFFER_H
#define NATIVECORE_IINDEXBUFFER_H

#include <vector>
#include <memory>

#include "GraphicsDataTypes.h"

namespace Core::Graphics
{
    class IIndexBuffer : public std::enable_shared_from_this<IIndexBuffer>
    {
    protected:
        SGGUsage m_usage = SGGUsage::SGG_STATIC;

    public:
        virtual ~IIndexBuffer() = default;

        virtual std::shared_ptr<IIndexBuffer> create() = 0;

        virtual void destroy() = 0;

        virtual std::shared_ptr<IIndexBuffer> putData(std::vector<std::uint32_t> data) = 0;
        virtual std::shared_ptr<IIndexBuffer> subData(std::vector<std::uint32_t> data, const int& offset) = 0;
        virtual std::shared_ptr<IIndexBuffer> bind() = 0;

        virtual std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) = 0;
    };
}

#endif //NATIVECORE_IINDEXBUFFER_H
