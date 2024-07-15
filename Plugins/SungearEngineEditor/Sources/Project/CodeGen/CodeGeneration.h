//
// Created by Ilya on 12.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_CODEGENERATION_H
#define SUNGEARENGINEEDITOR_CODEGENERATION_H

#include <vector>
#include <SGCore/Annotations/AnnotationsProcessor.h>

namespace SGE
{
    struct CodeGeneration
    {
        static void generateCode(const std::vector<SGCore::AnnotationsProcessor>& annotationsProcessors,
                                 const std::filesystem::path& toDirectory) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_CODEGENERATION_H
