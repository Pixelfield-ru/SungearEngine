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
    private:
        std::list<ShaderDefine> m_defines;

    public:
        ~Shader();

        virtual void bind() noexcept { }

        virtual void compile(const Memory::Assets::FileAsset*) { }

        void addShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(ShaderDefine& shaderDefine);
        void removeShaderDefine(const std::string& shaderDefineName);
    };
    //class IUniformType
}

#endif //NATIVECORE_SHADER_H
