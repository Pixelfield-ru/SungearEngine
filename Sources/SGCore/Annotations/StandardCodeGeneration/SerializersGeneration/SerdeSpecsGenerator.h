//
// Created by ilya on 03.07.24.
//

#ifndef SUNGEARENGINE_SERDESPECSGENERATOR_H
#define SUNGEARENGINE_SERDESPECSGENERATOR_H

#include "SGCore/pch.h"

namespace SGCore
{
    struct AnnotationsProcessor;
}

namespace SGCore::CodeGen
{
    struct SerdeSpecsGenerator
    {
        [[nodiscard]] std::string generateSerializers(const AnnotationsProcessor& annotationsProcessor,
                                                      const std::filesystem::path& generatedHeaderPath);

    private:
        void analyzeStruct(const AnnotationsProcessor& annotationsProcessor,
                           const std::string& currentStructName,
                           std::string& serializationOps,
                           std::string& deserializationOps,
                           std::string& outputLog) noexcept;

        std::unordered_map<std::string, std::vector<std::string>> m_structsBaseTypes;
        std::unordered_map<std::string, std::vector<std::string>> m_structsDerivedTypes;
    };
}

#endif //SUNGEARENGINE_SERDESPECSGENERATOR_H
