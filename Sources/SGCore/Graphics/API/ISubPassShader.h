//
// Created by stuka on 13.05.2023.
//
#ifndef NATIVECORE_ISHADER_H
#define NATIVECORE_ISHADER_H

#include <SGCore/pch.h>

#include "SGCore/Graphics/ShaderTextureBinding.h"
#include "SGCore/Memory/AssetWeakRef.h"

#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGCore/Memory/Assets/IAssetObserver.h"

#include "IUniformBuffer.h"
#include "ShaderDefine.h"

#include "SGCore/Utils/UniqueName.h"

#include "SGCore/Utils/SGSL/SGSLESubShaderType.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    class IUniformBuffer;
    class IFrameBuffer;
    class IMaterial;
    class ITexture2D;
    struct SGSLESubShader;

    // todo: add subshaders and add preprocess for it
    // todo: add various types of defines like material textures block define e.t.c.
    class ISubPassShader : public UniqueNameWrapper, public std::enable_shared_from_this<ISubPassShader>
    {
    public:
        std::string m_subPassName;
        
        std::string m_version;

        bool m_autoRecompile = true;

        Scope<IUniformBuffer> m_uniformBuffer;

        std::unordered_map<SGSLESubShaderType, Ref<SGSLESubShader>> m_subShaders;

        AssetWeakRef<TextFileAsset> m_fileAsset;

        virtual ~ISubPassShader() = default;

        virtual void compile(const std::string& subPassName) = 0;
        virtual void recompile() noexcept;

        virtual void destroy() = 0;

        virtual void bind() = 0;

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
        virtual void useInteger(const std::string& uniformName, const int& i) { };
        virtual void useTextureBlock(const std::string& uniformName, const int& textureBlock) { };
        
        virtual bool isUniformExists(const std::string& uniformName) const noexcept { return false; };

        // ==========================================
        
        /**
         * @param material - Material to bind textures.
         * @return Offset of samplers.
         */
        size_t bindMaterialTextures(const AssetRef<IMaterial>& material) noexcept;
        
        void unbindMaterialTextures(const AssetRef<IMaterial>& material) noexcept;
        
        /**
         * Bind all vector of m_textureBindings.
         * @param samplersOffset - Offset to start bind texture bindings.
         * @return Offset after bind.
         */
        size_t bindTextureBindings(const size_t& samplersOffset) noexcept;
        
        // ==========================================
        
        /*size_t getTexturesCount() const noexcept
        {
            size_t cnt = 0;
            for(const auto& block : m_texturesBlocks)
            {
                cnt += block->getTextures().size();
            }
            
            return cnt;
        }*/
        
        void addTextureBinding(const std::string& bindingName, const Ref<ITexture2D>& texture) noexcept;
        void removeTextureBinding(const std::string& bindingName) noexcept;
        
        std::vector<ShaderTextureBinding> m_textureBindings;
        
        ISubPassShader& operator=(const ISubPassShader&) noexcept;
    protected:
        std::unordered_map<std::string, IShaderUniform> m_uniforms;

        std::unordered_map<SGShaderDefineType, std::list<ShaderDefine>> m_defines;
    };
}

#endif //NATIVECORE_ISHADER_H
