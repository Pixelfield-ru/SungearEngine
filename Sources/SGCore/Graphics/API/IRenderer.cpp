#include "IRenderer.h"

Core::Graphics::APIType Core::Graphics::IRenderer::getAPIType() const noexcept
{
    return m_apiType;
}

Core::Memory::Assets::IMaterial* Core::Graphics::IRenderer::createBlankStandardMaterial(const StandardShaderType& standardShaderType)
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Core::Graphics::IShader>(
                    createShader(getShaderPath(standardShaderType))
            ));

    mat->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);

    return mat;
}

Core::Memory::Assets::IMaterial* Core::Graphics::IRenderer::createPBRMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Core::Graphics::IShader>(
                    createShader(getShaderPath(StandardShaderType::SG_PBR_SHADER))
            ));

    mat->setShader(
            SGMAT_SHADOW_GEN_SHADER_NAME,
            std::shared_ptr<Core::Graphics::IShader>(
                    createShader(getShaderPath(StandardShaderType::SG_SHADOWS_GENERATOR_SHADER))
            ));

    mat->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);

    // adding block decls
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS,
                             1, 0);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE,
                             1, 1);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_NORMALS,
                             1, 2);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_BASE_COLOR,
                             1, 3);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHADOW_MAP,
                             5, 4);

    return mat;
}

Core::Memory::Assets::IMaterial* Core::Graphics::IRenderer::createSkyboxMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Core::Graphics::IShader>(
                    createShader(getShaderPath(StandardShaderType::SG_SKYBOX_SHADER))
            ));

    mat->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);

    // adding block decls
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SKYBOX,
                             1, 0);

    return mat;
}

Core::Memory::Assets::IMaterial* Core::Graphics::IRenderer::createPostProcessingMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Core::Graphics::IShader>(
                    createShader(getShaderPath(StandardShaderType::SG_POSTPROCESSING_SHADER))
            ));

    mat->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);

    // adding block decls
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_FRAMEBUFFER_DEPTH_ATTACHMENT,
                             1, 0);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT,
                             1, 1);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_FRAMEBUFFER_COLOR_ATTACHMENT,
                             1, 2);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_FRAMEBUFFER_RENDER_ATTACHMENT,
                             1, 3);

    return mat;
}
