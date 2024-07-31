//
// Created by Ilya on 31.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_SERIALIZERS_H
#define SUNGEARENGINEEDITOR_SERIALIZERS_H

#include <SGCore/Scene/Serializer.h>
#include "Toolchains/Toolchain.h"
#include "Toolchains/VisualStudioToolchain.h"

template<>
struct SGCore::SerializerSpec<SGE::Toolchain>
{
    static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                          const std::string& varName, const SGE::Toolchain& value) noexcept;

    static void serializeOnlyValueType(rapidjson::Document& toDocument, rapidjson::Value& parent, const SGE::Toolchain& value) noexcept;
};

#endif //SUNGEARENGINEEDITOR_SERIALIZERS_H
