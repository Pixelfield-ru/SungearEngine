//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_SERIALIZABLES_H
#define SUNGEARENGINE_SERIALIZABLES_H

#include <SGCore/pch.h>

namespace SGCore
{
    template<typename T>
    struct Serializable
    {
        static_assert(always_false<T>::value, "This type is not serializable. Please, implement specialization of Serializable for this type.");
        
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const T& value) noexcept
        {
        }
    };
    
    struct Serializer
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const auto& value) noexcept
        {
            Serializable<std::remove_cvref_t<std::remove_pointer_t<decltype(value)>>>::serialize(toDocument, varName, value);
        }
    };
    
    // ===============================================================================================================================
    // ===============================================================================================================================
    // ===============================================================================================================================
    // STANDARD SERIALIZABLES FOR TYPES
    
    template<>
    struct Serializable<std::int8_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int8_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::int16_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int16_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::int32_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int32_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::int64_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int64_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::uint8_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::uint8_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::uint16_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::int16_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::uint32_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::uint32_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::uint64_t>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::uint64_t& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<double>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const double& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<float>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const float& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
        }
    };
    
    template<typename ScalarT, glm::qualifier Qualifier>
    struct Serializable<glm::vec<2, ScalarT, Qualifier>>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const glm::vec<2, ScalarT, Qualifier>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kObjectType);
            rapidjson::Value v(rapidjson::kObjectType);
            
            k.SetString(varName.c_str(), varName.length());
            v.AddMember("x", value.x, toDocument.GetAllocator());
            v.AddMember("y", value.y, toDocument.GetAllocator());
            
            toDocument.AddMember(k, v, toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<std::string>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const std::string& value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), rapidjson::StringRef(value.c_str()), toDocument.GetAllocator());
        }
    };
    
    template<>
    struct Serializable<const char*>
    {
        static void serialize(rapidjson::Document& toDocument, const std::string& varName, const char* value) noexcept
        {
            toDocument.AddMember(rapidjson::StringRef(varName.c_str()), rapidjson::StringRef(value), toDocument.GetAllocator());
        }
    };
}

#endif //SUNGEARENGINE_SERIALIZABLES_H
