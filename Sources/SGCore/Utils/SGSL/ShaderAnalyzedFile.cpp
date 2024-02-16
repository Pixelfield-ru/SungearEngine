//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGSLETranslator.h"
#include "SGUtils/FileUtils.h"

void SGCore::ShaderAnalyzedFile::load(const std::string& path)
{
    SGSLETranslator translator;
    
    translator.processCode(path, SGUtils::FileUtils::readFile(path), shared_from_this());
}

