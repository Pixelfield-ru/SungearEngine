//
// Created by stuka on 29.12.2023.
//

#ifndef SUNGEARENGINE_ISHADER_H
#define SUNGEARENGINE_ISHADER_H

#include <SGCore/pch.h>
#include "SGCore/Memory/AssetWeakRef.h"

#include "SGCore/Memory/Assets/IAssetObserver.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "GraphicsDataTypes.h"

namespace SGCore
{
    class ISubPassShader;
    class IMaterial;
    class ITexture2D;

    class IShader : public IAsset
    {
    public:
        sg_serde_as_friend()

        sg_implement_type_id(IShader, 14)

        /**
         * Removes all sub pass shaders in this IShader and adds new sub pass shaders.
         * @param fromFile
         */
        void compile(AssetRef<TextFileAsset> fromFile) noexcept;

        void setSubPassShader(const std::string& subPassName, const IShader* from) noexcept;
        void setSubPassShader(const std::string& subPassName, const Ref<ISubPassShader>& subPassShader) noexcept;

        Ref<ISubPassShader> getSubPassShader(const std::string& subPassName) const noexcept;

        /**
         * Calls recompile of shader program.
         *//*
        void onAssetModified() override;

        *//**
         * Calls recompile of shader program.
         *//*
        void onAssetPathChanged() override;*/

        const auto& getSubPassesShaders() const noexcept
        {
            return m_subPassesShaders;
        }

        void removeAllSubPassShadersByDiskPath(const std::string& path) noexcept;
        void removeSubPass(const std::string& subPassName) noexcept;

        /*static void useTextureGlobal(const std::string& uniformName, const std::uint8_t& texBlock) noexcept;

        static void useMatrixGlobal(const std::string& uniformName, const glm::mat4& matrix) noexcept;

        static void useVectorfGlobal(const std::string& uniformName, const float& x, const float& y) noexcept;

        static void useVectorfGlobal(const std::string& uniformName,
                                     const float& x, const float& y, const float& z) noexcept;

        static void useVectorfGlobal(const std::string& uniformName,
                                     const float& x, const float& y, const float& z, const float& w) noexcept;

        static void useVectorfGlobal(const std::string& uniformName, const glm::vec2& vec) noexcept;

        static void useVectorfGlobal(const std::string& uniformName, const glm::vec3& vec) noexcept;

        static void useVectorfGlobal(const std::string& uniformName, const glm::vec4& vec) noexcept;

        static void useFloatGlobal(const std::string& uniformName, const float& f) noexcept;

        static void useIntegerGlobal(const std::string& uniformName, const size_t& i) noexcept;

        static void useTextureBlockGlobal(const std::string& uniformName, const size_t& textureBlock) noexcept;*/

        [[nodiscard]] AssetRef<TextFileAsset> getFile() const noexcept;

    protected:
        void doLoad(const std::filesystem::path& path) override;
        void doLazyLoad() override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

    private:
        AssetRef<ShaderAnalyzedFile> m_shaderAnalyzedFile;
        AssetWeakRef<TextFileAsset> m_fileAsset;

        std::vector<Ref<ISubPassShader>> m_subPassesShaders;
    };
}

#endif //SUNGEARENGINE_ISHADER_H
