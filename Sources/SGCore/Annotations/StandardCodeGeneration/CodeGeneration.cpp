//
// Created by stuka on 10.09.2024.
//

#include "CodeGeneration.h"

void SGCore::CodeGen::Generator::generate(SGCore::AnnotationsProcessor& annotationsProcessor,
                                          const std::filesystem::path& referenceFile,
                                          const std::filesystem::path& outputFile) noexcept
{
    std::string referenceFileText = Utils::reduce(FileUtils::readFile(referenceFile));

    
}
