//
// Created by stuka on 13.05.2023.
//
#pragma once

#ifndef NATIVECORE_SHADER_H
#define NATIVECORE_SHADER_H

#include <list>

#include "ShaderDefine.h"
#include "../../Memory/Assets/FileAsset.h"

namespace Core::Graphics::API
{
    class Shader
    {
    protected:
        std::list<ShaderDefine> m_defines;
        std::string_view m_path;

    public:
        //Shader(const Shader&) = delete;
        //Shader(Shader&&) noexcept = default;

        ~Shader();

        virtual void destroy() = 0;

        virtual void bind() = 0;

        // shaderVirtualPath must be specified without extension
        virtual void compile(const std::string_view& shaderVirtualPath, const std::string& code) = 0;

        void addShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(const std::string& shaderDefineName);

        #pragma region Operators
        //Shader& operator=(Shader&&) noexcept;
        #pragma endregion
    };
    //class IUniformType
}

#endif //NATIVECORE_SHADER_H
