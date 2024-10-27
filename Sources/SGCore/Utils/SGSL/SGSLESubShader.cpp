//
// Created by stuka on 27.10.2024.
//
#include "SGSLESubShader.h"

#include "SGCore/Memory/AssetManager.h"

void SGCore::SGSLESubShader::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    m_code = parentAssetManager->getPackage().readData<std::string>(m_codeOffsetInPackage, m_codeSizeInPackage);
}

