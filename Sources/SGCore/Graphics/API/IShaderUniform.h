#pragma once

#ifndef SUNGEARENGINE_ISHADERUNIFORM_H
#define SUNGEARENGINE_ISHADERUNIFORM_H

#include <memory>
#include "GraphicsDataTypes.h"

namespace Core::Graphics
{
    struct IShaderUniform
    {
        std::string m_name;
        std::int32_t m_location = -1;

        SGGUsage m_usage = SGGUsage::SGG_DYNAMIC;
        SGGDataType m_dataType = SGGDataType::SGG_INT;

        IShaderUniform(std::string name, std::int32_t location, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_location = location;
            m_usage = usage;
            m_dataType = dataType;
        }

        IShaderUniform(std::string name, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_usage = usage;
            m_dataType = dataType;
        }

        IShaderUniform(std::string name, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_dataType = dataType;
        }
    };
}

#endif //SUNGEARENGINE_ISHADERUNIFORM_H
