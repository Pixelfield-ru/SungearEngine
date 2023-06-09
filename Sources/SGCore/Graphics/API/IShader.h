//
// Created by stuka on 13.05.2023.
//
#pragma once

#ifndef NATIVECORE_SHADER_H
#define NATIVECORE_SHADER_H

#include <list>

#include "ShaderDefine.h"
#include "SGCore/Memory/Assets/FileAsset.h"
#include "SGCore/Memory/Assets/IAssetObserver.h"

namespace Core::Graphics::API
{
    class IShader : public Memory::Assets::IAssetObserver
    {
    protected:
        std::list<ShaderDefine> m_defines;
        std::weak_ptr<Memory::Assets::FileAsset> m_fileAsset;

    public:
        virtual ~IShader();

        virtual void destroy() = 0;

        virtual void bind() = 0;

        virtual void compile(std::shared_ptr<Memory::Assets::FileAsset> asset) = 0;

        [[nodiscard]] virtual std::int32_t getShaderUniformLocation(const std::string& uniformName) const = 0;

        void addShaderDefine(const ShaderDefine& shaderDefine);
        void removeShaderDefine(const ShaderDefine& shaderDefine);
        void removeShaderDefine(const std::string& shaderDefineName);

        void onAssetModified() override;
        void onAssetPathChanged() override;

        #pragma region Operators
        //IShader& operator=(IShader&&) noexcept;
        #pragma endregion
    };
    //class IUniformType
}

#endif //NATIVECORE_SHADER_H
