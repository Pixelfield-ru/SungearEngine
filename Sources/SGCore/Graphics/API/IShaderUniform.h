#pragma once

#ifndef SUNGEARENGINE_ISHADERUNIFORM_H
#define SUNGEARENGINE_ISHADERUNIFORM_H

#include <memory>
#include <any>
#include "GraphicsDataTypes.h"

namespace SGCore
{
    struct IShaderUniform
    {
        friend class IUniformBuffer;

        std::string m_name;
        std::int32_t m_location = -1;

        SGGUsage m_usage = SGGUsage::SGG_DYNAMIC;
        SGGDataType m_dataType = SGGDataType::SGG_INT;

        std::any m_data;

        IShaderUniform() noexcept = default;

        IShaderUniform(std::string&& name, std::int32_t location, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_location = location;
            m_usage = usage;
            m_dataType = dataType;
        }

        IShaderUniform(std::string&& name, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_usage = usage;
            m_dataType = dataType;
        }

        IShaderUniform(std::string&& name, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_dataType = dataType;
        }

        bool operator==(const IShaderUniform& other) const noexcept
        {
            return m_name == other.m_name;
        }

        bool operator!=(const IShaderUniform& other) const noexcept
        {
            return !(*this == other);
        }

    private:
        /*
         * In bytes. USE ONLY IN UNIFORM BUFFER.
         */
        size_t m_offsetInUniformBuffer = 0;

        /*
         * In bytes. USE ONLY IN UNIFORM BUFFER.
         */
        size_t m_dataSizeInUniformBuffer = 0;
    };
}

#endif //SUNGEARENGINE_ISHADERUNIFORM_H
