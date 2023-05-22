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
    class IShader
    {
    protected:
        std::list<ShaderDefine> m_defines;
        std::string_view m_path;

    public:
        //IShader(const IShader&) = delete;
        //IShader(IShader&&) noexcept = default;

        virtual ~IShader();

        virtual void destroy() = 0;

        virtual void bind() = 0;

        // shaderVirtualPath must be specified without extension
        virtual void compile(const std::string_view& shaderVirtualPath, const std::string& code) = 0;

        void addShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(const std::string& shaderDefineName);

        #pragma region Operators
        //IShader& operator=(IShader&&) noexcept;
        #pragma endregion
    };
    //class IUniformType
}

#endif //NATIVECORE_SHADER_H
