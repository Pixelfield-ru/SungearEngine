//
// Created by ilya on 03.07.24.
//

#ifndef SUNGEARENGINE_SERIALIZERSGENERATOR_H
#define SUNGEARENGINE_SERIALIZERSGENERATOR_H

#include "SGCore/pch.h"
#include "SGCore/Annotations/AnnotationsProcessor.h"

namespace SGCore::CodeGen
{
    struct SerializersGenerator
    {
        [[nodiscard]] std::string generateSerializers(const AnnotationsProcessor& annotationsProcessor, const std::filesystem::path& toPath) const noexcept;
    };
}

#endif //SUNGEARENGINE_SERIALIZERSGENERATOR_H
