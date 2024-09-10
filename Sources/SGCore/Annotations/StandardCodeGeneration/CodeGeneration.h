//
// Created by stuka on 10.09.2024.
//

#ifndef SUNGEARENGINE_CODEGENERATION_H
#define SUNGEARENGINE_CODEGENERATION_H

#include <SGCore/Annotations/AnnotationsProcessor.h>

namespace SGCore::CodeGen
{
    namespace Lang
    {
        // range-based for. example: ## for struct in structs
        struct ForCycle
        {
            // name of variable
            std::string m_variableName;
            // in what collection cycle is
            std::string m_cycleIn;
        };
    }

    struct Generator
    {
        void generate(AnnotationsProcessor& annotationsProcessor, const std::filesystem::path& referenceFile, const std::filesystem::path& outputFile) noexcept;

    private:
        std::vector<Lang::ForCycle> m_forCycles;
    };
}

#endif //SUNGEARENGINE_CODEGENERATION_H
