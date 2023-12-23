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
#include "ITexture2D.h"
#include "ShaderDefine.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Utils/UniqueName.h"

namespace SGCore
{
    class IUniformBuffer;
    class IFrameBuffer;

    struct ShaderTexturesFromGlobalStorageBlock
    {
        friend class GLShadersPreprocessor;

        std::string m_uniformName;

        bool m_isSingleTextureBlock = false;

        void addTexture(const Ref<ITexture2D>& texture2D) noexcept;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept;
        void clearTextures() noexcept;

        [[nodiscard]] const auto& getTextures() const noexcept
        {
            return m_textures;
        }

        bool operator==(const ShaderTexturesFromGlobalStorageBlock& other) const noexcept;
        bool operator!=(const ShaderTexturesFromGlobalStorageBlock& other) const noexcept;

    private:
        Weak<IShader> m_parentShader;

        std::list<Weak<ITexture2D>> m_textures;

        std::list<std::string> m_requiredTexturesNames;
    };

    // todo: add various types of defines like material textures block define e.t.c.
    class IShader : public IAssetObserver, public UniqueNameWrapper, public std::enable_shared_from_this<IShader>
    {
        friend struct ShaderTexturesFromGlobalStorageBlock;

    public:
        std::string m_version;

        Scope<IUniformBuffer> m_uniformBuffer;

        Weak<FileAsset> m_fileAsset;

        virtual ~IShader() = default;

        virtual void destroy() = 0;

        virtual void bind() = 0;

        virtual void compile(Ref<FileAsset> asset) = 0;

        void updateFrameBufferAttachmentsCount(const Ref <SGCore::IFrameBuffer>& frameBuffer,
                                               const std::string& frameBufferNameInShader) noexcept;

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
        void replaceDefines(const SGShaderDefineType& shaderDefineType, Ref<IShader> otherShader) noexcept;

        void clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept;

        /**
         * Calls recompile of shader program.
         */
        void onAssetModified() override;

        /**
         * Calls recompile of shader program.
         */
        void onAssetPathChanged() override;

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

        Ref<IShader> addToGlobalStorage() noexcept;

        template<typename Block = ShaderTexturesFromGlobalStorageBlock>
        void addTexturesFromGlobalStorageBlock(Block&& block) noexcept
        {
            if(std::find(m_texturesFromGlobalStorageBlocks.begin(), m_texturesFromGlobalStorageBlocks.end(), block) ==
                m_texturesFromGlobalStorageBlocks.end())
            {
                m_texturesFromGlobalStorageBlocks.emplace_back(std::forward<Block>(block));
                block.m_parentShader = shared_from_this();
            }
        }

        void removeTexturesFromGlobalStorageBlock(const ShaderTexturesFromGlobalStorageBlock& block) noexcept;

        void clearTexturesFromGlobalStorageBlocks() noexcept;

        IShader& operator=(const IShader&) noexcept;

    protected:
        void onTexturesCountChanged() noexcept;

        std::vector<ShaderTexturesFromGlobalStorageBlock> m_texturesFromGlobalStorageBlocks;

        std::unordered_map<std::string, IShaderUniform> m_uniforms;

        std::unordered_map<SGShaderDefineType, std::list<ShaderDefine>> m_defines;
    };
    //class IUniformType
}

#endif //NATIVECORE_ISHADER_H
