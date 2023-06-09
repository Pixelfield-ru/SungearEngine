#pragma once

#ifndef SUNGEARENGINE_ISIMPLESHADERUNIFORM_H
#define SUNGEARENGINE_ISIMPLESHADERUNIFORM_H

#include "IShaderUniform.h"

namespace Core::Graphics::API
{
    struct ISimpleShaderUniform : public IShaderUniform
    {
        /*
        SGGDataType m_dataType = SGGDataType::SGG_INT;

        ISimpleShaderUniform(std::string name, std::int32_t location, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_location = location;
            m_usage = usage;
            m_dataType = dataType;
        }

        ISimpleShaderUniform(std::string name, SGGUsage usage, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_usage = usage;
            m_dataType = dataType;
        }

        ISimpleShaderUniform(std::string name, SGGDataType dataType)
        {
            m_name = std::move(name);
            m_dataType = dataType;
        }
         */
    };
}

#endif //SUNGEARENGINE_ISIMPLESHADERUNIFORM_H
