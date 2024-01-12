//
// Created by stuka on 13.05.2023.
//
#ifndef NATIVECORE_ISHADER_H
#define NATIVECORE_ISHADER_H

#include <list>
#include <glm/matrix.hpp>

#include "SGCore/Memory/Assets/FileAsset.h"
#include "SGCore/Memory/Assets/IAssetObserver.h"

#include "IUniformBuffer.h"
#include "ShaderDefine.h"

#include "SGCore/Utils/UniqueName.h"

#include "SGCore/Graphics/ShaderTexturesFromGlobalStorageBlock.h"
#include "SGCore/Graphics/ShaderTexturesFromMaterialBlock.h"
#include "SGCore/Utils/SGSL/SGSLSubShaderType.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    class IUniformBuffer;
    class IFrameBuffer;
    class IMaterial;
    class ITexture2D;

    // todo: add subshaders and add preprocess for it
    // todo: add various types of defines like material textures block define e.t.c.
    class ISubPassShader : public UniqueNameWrapper, public std::enable_shared_from_this<ISubPassShader>
    {
    public:
        std::string m_version;

        bool m_autoRecompile = true;

        Scope<IUniformBuffer> m_uniformBuffer;

        std::unordered_map<SGSLSubShaderType, std::string> m_subShadersCodes;

        Weak<FileAsset> m_fileAsset;

        virtual ~ISubPassShader() = default;

        virtual void compile(Ref<FileAsset> fileAsset) = 0;
        virtual void recompile() noexcept;

        virtual void destroy() = 0;

        virtual void bind() = 0;

        void bindTexturesBlocks() const noexcept;

        [[nodiscard]] virtual std::int32_t getShaderUniformLocation(const std::string& uniformName) = 0;

        // TODO: wtf is this. clean code
        void addDefines(const SGShaderDefineType& shaderDefineType, const std::vector<ShaderDefine>& shaderDefines);
        void emplaceDefines(const SGShaderDefineType& shaderDefineType, std::vector<ShaderDefine>& shaderDefines);

        void addDefine(const SGShaderDefineType& shaderDefineType, const ShaderDefine& shaderDefine);
        void emplaceDefine(const SGShaderDefineType& shaderDefineType, ShaderDefine&& shaderDefine);

        void removeDefine(const SGShaderDefineType& shaderDefineType, const ShaderDefine& shaderDefine);
        void removeDefine(const SGShaderDefineType& shaderDefineType, const std::string& shaderDefineName);

        void updateDefine(const SGShaderDefineType& shaderDefineType, const ShaderDefine& shaderDefine);
        void emplaceUpdateDefine(const SGShaderDefineType& shaderDefineType, ShaderDefine&& shaderDefine);

        void updateDefines(const SGShaderDefineType& shaderDefineType, const std::vector<ShaderDefine>& shaderDefines);
        void emplaceUpdateDefines(const SGShaderDefineType& shaderDefineType, std::vector<ShaderDefine>& shaderDefines);

        void replaceDefines(const SGShaderDefineType& shaderDefineType, const std::list<ShaderDefine>& otherDefines) noexcept;
        void replaceDefines(const SGShaderDefineType& shaderDefineType, Ref<ISubPassShader> otherShader) noexcept;

        void clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept;

        virtual void useUniformBuffer(const Ref<IUniformBuffer>&) { };
        virtual void useTexture(const std::string& uniformName, const std::uint8_t& texBlock) { };

        virtual void useMatrix(const std::string& uniformName, const glm::mat4& matrix) { };

        virtual void useVectorf(const std::string& uniformName, const float& x, const float& y) { };
        virtual void useVectorf(const std::string& uniformName,
                                const float& x, const float& y, const float& z) { };
        virtual void useVectorf(const std::string& uniformName,
                                const float& x, const float& y, const float& z, const float& w) { };

        virtual void useVectorf(const std::string& uniformName, const glm::vec2& vec) { };
        virtual void useVectorf(const std::string& uniformName, const glm::vec3& vec) { };
        virtual void useVectorf(const std::string& uniformName, const glm::vec4& vec) { };

        virtual void useFloat(const std::string& uniformName, const float& f) { };
        virtual void useInteger(const std::string& uniformName, const size_t& i) { };
        virtual void useTextureBlock(const std::string& uniformName, const size_t& textureBlock) { };

        Ref<ISubPassShader> addToGlobalStorage() noexcept;

        // ==========================================

        void addTexturesBlock(const Ref<ShaderTexturesBlock>& block) noexcept;

        void removeTexturesBlock(const Ref<ShaderTexturesBlock>& block) noexcept;

        void clearTexturesBlocks() noexcept;

        template<typename TexturesBlockType>
        void clearTexturesBlocksOfType() noexcept
        {
            for(auto& textureBlock : m_texturesBlocks)
            {
                if(SG_INSTANCEOF(textureBlock.get(), TexturesBlockType))
                {
                    textureBlock->clearTextures();
                }
            }
        }

        // ==========================================

        void addTexture(const Ref<ITexture2D>& texture2D) noexcept;
        void addTexture(const Ref<ITexture2D>& texture2D, SGTextureType textureType) noexcept;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept;

        void collectTexturesFromMaterial(const Ref<IMaterial>& material) noexcept;

        void onTexturesCountChanged() noexcept;

        ISubPassShader& operator=(const ISubPassShader&) noexcept;
    protected:
        std::vector<Ref<ShaderTexturesBlock>> m_texturesBlocks;

        std::unordered_map<std::string, IShaderUniform> m_uniforms;

        std::unordered_map<SGShaderDefineType, std::list<ShaderDefine>> m_defines;
    };
}

#endif //NATIVECORE_ISHADER_H
