//
// Created by stuka on 13.05.2023.
//
#ifndef NATIVECORE_ISHADER_H
#define NATIVECORE_ISHADER_H

#include <SGCore/pch.h>
#include "SGCore/Main/CoreMain.h"
#include "IRenderer.h"
// #include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/AssetRefFromThis.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

#include "SGCore/Graphics/ShaderTextureBinding.h"
#include "SGCore/Memory/AssetWeakRef.h"

#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGCore/Memory/Assets/IAssetObserver.h"

#include "IUniformBuffer.h"
#include "ShaderDefine.h"

#include "SGCore/Utils/UniqueName.h"

#include "SGCore/Utils/Utils.h"

#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"

namespace SGCore
{
    class IUniformBuffer;
    class IFrameBuffer;
    class IMaterial;
    class ITexture2D;
    struct SGSLESubShader;

    // todo: add various types of defines like material textures block define e.t.c.
    class IShader : public AssetRefFromThis<IShader>, public IAsset, public IAssetsRefsResolver<IShader>
    {
    public:
        friend class AssetManager;

        sg_serde_as_friend()

        sg_implement_type_id(IShader, 14)

        sg_assets_refs_resolver_as_friend

        std::string m_version;

        bool m_autoRecompile = false;

        Scope<IUniformBuffer> m_uniformBuffer;

        ~IShader() override = default;

        void compile(const AssetRef<TextFileAsset>& textFileAsset) noexcept;
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
        void replaceDefines(const SGShaderDefineType& shaderDefineType, Ref<IShader> otherShader) noexcept;

        void clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept;

        [[nodiscard]] const auto& getDefines() const noexcept
        {
            return m_defines;
        }

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
        virtual void useMaterialFactors(const IMaterial* material) { };

        virtual bool isUniformExists(const std::string& uniformName) const noexcept { return false; };

        // ==========================================
        
        /**
         * @param material - Material to bind textures.
         * @return Offset of samplers.
         */
        size_t bindMaterialTextures(const AssetRef<IMaterial>& material, std::uint8_t samplersOffset = 0) noexcept;
        
        void unbindMaterialTextures(const AssetRef<IMaterial>& material) noexcept;
        
        /**
         * Bind all vector of m_textureBindings.
         * @param samplersOffset - Offset to start bind texture bindings.
         * @return Offset after bind.
         */
        size_t bindTextureBindings(const size_t& samplersOffset) noexcept;
        
        // ==========================================
        
        void addTextureBinding(const std::string& bindingName, const AssetRef<ITexture2D>& texture) noexcept;
        void removeTextureBinding(const std::string& bindingName) noexcept;

        [[nodiscard]] const std::vector<ShaderTextureBinding>& getTextureBindings() const noexcept;

        IShader& operator=(const IShader&) noexcept;

        [[nodiscard]] AssetRef<ShaderAnalyzedFile> getAnalyzedFile() const noexcept;
        [[nodiscard]] AssetRef<TextFileAsset> getFile() const noexcept;

    protected:
        virtual void doCompile() = 0;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

    private:
        AssetWeakRef<ShaderAnalyzedFile> m_shaderAnalyzedFile;
        AssetWeakRef<TextFileAsset> m_fileAsset;

        std::vector<ShaderTextureBinding> m_textureBindings;

        std::unordered_map<SGShaderDefineType, std::list<ShaderDefine>> m_defines;

        void doLoad(const InterpolatedPath& path) override;
        void doLazyLoad() override;
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        template<typename... AssetCtorArgs>
        static Ref<IShader> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto tex = Ref<IShader>(CoreMain::getRenderer()->createShader(std::forward<AssetCtorArgs>(assetCtorArgs)...));

            return tex;
        }
    };
}

#endif //NATIVECORE_ISHADER_H
