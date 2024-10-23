//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"

void SGCore::ShaderAnalyzedFile::doLoad(const std::filesystem::path& path)
{
    SGSLETranslator translator;
    
    translator.processCode(path, SGCore::FileUtils::readFile(path), shared_from_this());
}

void SGCore::ShaderAnalyzedFile::serializeToPackage(SGCore::AssetsPackage::AssetSection& currentAssetSection,
                                                    bool isDataSerializing)
{
    currentAssetSection.addStandardInfo(this);

    if(isDataSerializing)
    {
        // currentAssetSection.addSection("m_allCode", getAllCode());
    }
}
