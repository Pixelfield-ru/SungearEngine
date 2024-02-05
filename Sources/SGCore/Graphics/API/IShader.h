//
// Created by stuka on 29.12.2023.
//

#ifndef SUNGEARENGINE_ISHADER_H
#define SUNGEARENGINE_ISHADER_H

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

#include "SGCore/Memory/Assets/IAssetObserver.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/FileAsset.h"
#include "GraphicsDataTypes.h"

namespace SGCore
{
    class ISubPassShader;
    class IMaterial;
    class ITexture2D;

    class IShader : public IAssetObserver
    {
    public:
        ShaderAnalyzedFile m_shaderAnalyzedFile;

        Weak<FileAsset> m_fileAsset;

        void addSubPassShadersAndCompile(Ref<FileAsset> asset) noexcept;

        void setSubPassShader(const Ref<IShader>& from, const std::string& subPassName) noexcept;

        Ref<ISubPassShader> getSubPassShader(const std::string& subPassName) noexcept;

        /**
         * Calls recompile of shader program.
         */
        void onAssetModified() override;

        /**
         * Calls recompile of shader program.
         */
        void onAssetPathChanged() override;

        void collectTextureFromMaterial(const Ref<ITexture2D>& texture, SGTextureType textureType) noexcept;

        void setParentMaterial(const Ref<IMaterial>& material) noexcept;
        Weak<IMaterial> getParentMaterial() const noexcept;

        const auto& getSubPassesShaders() const noexcept
        {
            return m_subPassesShaders;
        }

        void removeAllSubPassShadersByDiskPath(const std::string& path) noexcept;
        void removeSubPass(const std::string& virtualPath) noexcept;

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

    private:
        // first - sub pass name
        std::unordered_map<std::string, Ref<ISubPassShader>> m_subPassesShaders;

        Weak<IMaterial> m_parentMaterial;
    };
}

#endif //SUNGEARENGINE_ISHADER_H
