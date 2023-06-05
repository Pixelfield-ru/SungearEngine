#pragma once

#ifndef SUNGEARENGINE_ISHADERUNIFORM_H
#define SUNGEARENGINE_ISHADERUNIFORM_H

#include <memory>

#include "IShader.h"

namespace Core::Graphics::API
{
    class IShaderUniform
    {
    protected:
        std::string m_name;
        std::int32_t m_location = -1;
        std::shared_ptr<void> m_data;

    public:
        IShaderUniform(std::string name, std::int32_t location, std::shared_ptr<void> data) :
        m_name(std::move(name)), m_location(location), m_data(std::move(data))
        {

        }

        IShaderUniform(std::string name, std::shared_ptr<void> data) : m_name(std::move(name)), m_data(std::move(data))
        {

        }

        virtual void updateLocation(const std::shared_ptr<IShader>& fromShader) = 0;
    };
}

#endif //SUNGEARENGINE_ISHADERUNIFORM_H
