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
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const T& value) noexcept
        {
            static_assert(always_false<T>::value, "This type is not serializable. Please, implement specialization of Serializable for this type.");
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
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::int16_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int16_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::int32_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int32_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::int64_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::int64_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::uint8_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint8_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::uint16_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const std::int16_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::uint32_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint32_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<std::uint64_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint64_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<double>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const double& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<float>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const float& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };

    template<>
    struct SerializerSpec<bool>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const bool& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<std::int32_t ScalarsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::vec<ScalarsCnt, ScalarT, Qualifier>>
    {
        static_assert(ScalarsCnt >= 1 && ScalarsCnt <= 4 && "Scalars count in glm::vec must be in range of 1-4.");

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const glm::vec<ScalarsCnt, ScalarT, Qualifier>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.PushBack(value.x, toDocument.GetAllocator());
            if constexpr(ScalarsCnt >= 2)
            {
                v.PushBack(value.y, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 3)
            {
                v.PushBack(value.z, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 4)
            {
                v.PushBack(value.w, toDocument.GetAllocator());
            }
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<std::int32_t ColumnsCnt, std::int32_t RowsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>>
    {
        static_assert(ColumnsCnt >= 1 && ColumnsCnt <= 4 && "Columns count in glm::mat must be in range of 1-4.");
        static_assert(RowsCnt >= 1 && RowsCnt <= 4 && "Rows count in glm::mat must be in range of 1-4.");
        
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            for(std::int32_t c = 0; c < ColumnsCnt; ++c)
            {
                for(std::int32_t r = 0; r < RowsCnt; ++r)
                {
                    v.PushBack(value[c][r], toDocument.GetAllocator());
                }
            }
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<typename T>
    struct SerializerSpec<std::vector<T>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::vector<T>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            for(const auto& n : value)
            {
                Serializer::serialize(toDocument, v, "", n);
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
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, rapidjson::StringRef(value.c_str()), toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(rapidjson::StringRef(value.c_str()), toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
    
    template<>
    struct SerializerSpec<const char*>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const char* value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, rapidjson::StringRef(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(rapidjson::StringRef(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }
    };
}

#endif //SUNGEARENGINE_SERIALIZER_H
