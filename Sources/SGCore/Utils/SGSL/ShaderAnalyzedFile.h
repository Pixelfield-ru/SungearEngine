//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SHADERANALYZEDFILE_H
#define SGSLETRANSLATOR_SHADERANALYZEDFILE_H

#include "SGCore/Utils/SGSL/SGSLESubShader.h"

#include "SGCore/Memory/Assets/IAsset.h"

namespace SGCore
{
    struct ShaderAnalyzedFile : public IAsset, public std::enable_shared_from_this<ShaderAnalyzedFile>
    {
        sg_serde_as_friend();

        sg_implement_type_id(ShaderAnalyzedFile, 4)

        friend struct SGSLETranslator;
        friend class AssetManager;

        [[nodiscard]] const auto& getSubPassName() const noexcept
        {
            return m_subPassName;
        }

        [[nodiscard]] const auto& getAttributes() const noexcept
        {
            return m_attributes;
        }

        /*[[nodiscard]] std::optional<const std::any*> getAttribute(const std::string& name) const noexcept
        {
            auto it = m_attributes.find(name);
            return it != m_attributes.end() ? (std::optional<const std::any*>) &it->second : std::nullopt;
        }*/

        [[nodiscard]] SGSLESubShader* getSubShaderByType(SGSLESubShaderType type) noexcept;

        [[nodiscard]] const auto& getSubShaders() const noexcept
        {
            return m_subShaders;
        }
        
    protected:
        void doLoad(const InterpolatedPath& path) final;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

    private:
        // sub pass of shader
        std::string m_subPassName;
        // attributes of shader
        std::unordered_map<std::string, std::string> m_attributes;
        // code of fragment or vertex or smth else sub shaders
        std::vector<SGSLESubShader> m_subShaders;
    };
}

#endif //SGSLETRANSLATOR_SHADERANALYZEDFILE_H
