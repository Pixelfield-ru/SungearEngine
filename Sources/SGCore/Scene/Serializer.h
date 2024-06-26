//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_SERIALIZER_H
#define SUNGEARENGINE_SERIALIZER_H

#include <SGCore/pch.h>

#include "SGCore/Utils/TypeTraits.h"

// HERE IS SPECIALIZATIONS OF SerializerSpec FOR BUILT-IN TYPES OF C++ AND LIBRARIES TYPES

namespace SGCore
{
    template<typename T>
    struct SerializerSpec
    {
        static_assert(always_false<T>::value, "This type is not serializable. Please, implement specialization of Serializable for this type.");
        
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const T& value) noexcept
        {
        }
    };
    
    struct Serializer
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const auto& value) noexcept
        {
            SerializerSpec<std::remove_cvref_t<std::remove_pointer_t<decltype(value)>>>::serialize(toDocument, parent, varName, value);
        }
    };
    
    // ===============================================================================================================================
    // ===============================================================================================================================
    // ===============================================================================================================================
    // STANDARD SERIALIZABLES FOR TYPES
    
    template<>
    struct SerializerSpec<std::int8_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int8_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::int16_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int16_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::int32_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int32_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::int64_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int64_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::uint8_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint8_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::uint16_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int16_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::uint32_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint32_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::uint64_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint64_t& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<double>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const double& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<float>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const float& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };

    template<>
    struct SerializerSpec<bool>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const bool& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<std::int32_t ScalarsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::vec<ScalarsCnt, ScalarT, Qualifier>>
    {
        static_assert(ScalarsCnt >= 1 && ScalarsCnt <= 4 && "Scalars count in glm::vec must be in range of 1-4.");

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const glm::vec<ScalarsCnt, ScalarT, Qualifier>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kObjectType);
            rapidjson::Value v(rapidjson::kObjectType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.AddMember("x", value.x, toDocument.GetAllocator());
            if constexpr(ScalarsCnt >= 2)
            {
                v.AddMember("y", value.y, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 3)
            {
                v.AddMember("z", value.z, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 4)
            {
                v.AddMember("w", value.w, toDocument.GetAllocator());
            }

            parent.AddMember(k, v, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<std::string>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::string& value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), rapidjson::StringRef(value.c_str()), toDocument.GetAllocator());
        }
    };
    
    template<>
    struct SerializerSpec<const char*>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const char* value) noexcept
        {
            parent.AddMember(rapidjson::StringRef(varName.c_str()), rapidjson::StringRef(value), toDocument.GetAllocator());
        }
    };
}

#endif //SUNGEARENGINE_SERIALIZER_H
