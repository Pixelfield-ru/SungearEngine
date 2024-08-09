//
// Created by stuka on 29.12.2023.
//

#ifndef SUNGEARENGINE_ISHADER_H
#define SUNGEARENGINE_ISHADER_H

#include <SGCore/pch.h>

#include "SGCore/Memory/Assets/IAssetObserver.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "GraphicsDataTypes.h"

namespace SGCore
{
    class ISubPassShader;
    class IMaterial;
    class ITexture2D;

    class IShader : public IAssetObserver
    {
    public:
        Ref<ShaderAnalyzedFile> m_shaderAnalyzedFile;

        Weak<TextFileAsset> m_fileAsset;

        void addSubPassShadersAndCompile(Ref<TextFileAsset> asset) noexcept;

        void setSubPassShader(const std::string& subPassName, const Ref<IShader>& from) noexcept;
        void setSubPassShader(const std::string& subPassName, const Ref<ISubPassShader>& subPassShader) noexcept;

        Ref<ISubPassShader> getSubPassShader(const std::string& subPassName) noexcept;

        /**
         * Calls recompile of shader program.
         */
        void onAssetModified() override;

        /**
         * Calls recompile of shader program.
         */
        void onAssetPathChanged() override;

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

    private:
        std::vector<Ref<ISubPassShader>> m_subPassesShaders;
    };
}

#endif //SUNGEARENGINE_ISHADER_H
