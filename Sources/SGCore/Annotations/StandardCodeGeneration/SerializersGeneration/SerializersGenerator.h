//
// Created by ilya on 03.07.24.
//

#ifndef SUNGEARENGINE_SERIALIZERSGENERATOR_H
#define SUNGEARENGINE_SERIALIZERSGENERATOR_H

#include "SGCore/pch.h"

namespace SGCore
{
    struct AnnotationsProcessor;
}

namespace SGCore::CodeGen
{
    struct SerializersGenerator
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

#endif //SUNGEARENGINE_SERIALIZERSGENERATOR_H
