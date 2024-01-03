//
// Created by stuka on 29.12.2023.
//

#ifndef SUNGEARENGINE_ISHADER_H
#define SUNGEARENGINE_ISHADER_H

#include <unordered_map>
#include <string>
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

    private:
        std::unordered_map<std::string, Ref<ISubPassShader>> m_subPassesShaders;

        Weak<IMaterial> m_parentMaterial;
    };
}

#endif //SUNGEARENGINE_ISHADER_H
