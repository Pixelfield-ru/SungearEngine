//
// Created by ilya on 02.07.24.
//

#include "Serializer.h"

// ===============================================================================================================================
// ===============================================================================================================================
// ===============================================================================================================================
// STANDARD SERIALIZABLES FOR TYPES


/*template<> void SGCore::SerializerSpec<std::int8_t>::serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                                                    const std::string& varName, const std::int8_t& value) noexcept
{
    parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
}*/
