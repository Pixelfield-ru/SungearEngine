//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"

void SGCore::ShaderAnalyzedFile::doLoad(const std::string& path)
{
    SGSLETranslator translator;
    
    translator.processCode(path, SGCore::FileUtils::readFile(path), shared_from_this());
}

