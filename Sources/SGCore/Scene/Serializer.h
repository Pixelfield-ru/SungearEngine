//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_SERIALIZER_H
#define SUNGEARENGINE_SERIALIZER_H

#include <SGCore/pch.h>

#include "SGCore/Utils/TypeTraits.h"

#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Annotations/AnnotationsProcessor.h"

// HERE IS SPECIALIZATIONS OF SerializerSpec FOR BUILT-IN TYPES OF C++ AND LIBRARIES TYPES

namespace SGCore
{
    template<typename T>
    struct SerializerSpec
    {
        static_assert(always_false<T>::value, "This type is not serializable. Please, implement specialization of SerializerSpec for this type.");

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const T& value) noexcept
        {
        }

        static T deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
        }
    };

    struct Serializer
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const auto& value) noexcept
        {
            SerializerSpec<std::remove_cvref_t<decltype(value)>>::serialize(toDocument, parent, varName, value);
        }

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, auto& value) noexcept
        {
            SerializerSpec<std::remove_cvref_t<decltype(value)>>::serialize(toDocument, parent, varName, value);
        }

        template<typename T>
        static T deserialize(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
        {
            if (!parent.HasMember(varName.c_str()))
            {
                Serializer::formNotExistingMemberError(parent, varName, outputLog);

                return {};
            }

            auto& self = parent[varName.c_str()];

            return SerializerSpec<T>::deserialize(parent, self, outputLog);
        }

        template<typename T>
        static T deserialize(const rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog) noexcept
        {
            if (parent.Size() <= index)
            {
                Serializer::formNotExistingMemberError(parent, index, outputLog);

                return {};
            }

            auto& self = parent[index];

            return SerializerSpec<T>::deserialize(parent, self, outputLog);
        }

        static std::string formNotExistingMemberError(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
        {
            // TODO: MAYBE PARENT NAME
            outputLog += "Error: member '" + varName + "' does not exist";
        }

        static std::string formNotExistingMemberError(const rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog) noexcept
        {
            // TODO: MAYBE PARENT NAME
            outputLog += "Error: member with index '" + std::to_string(index) + "' does not exist";
        }
    };

    // ===============================================================================================================================
    // ===============================================================================================================================
    // ===============================================================================================================================
    // STANDARD SERIALIZABLES FOR TYPES

    template<typename T>
    requires(std::is_enum_v<T>)
    struct SerializerSpec<T>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, T value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            switch (parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, static_cast<std::int64_t>(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(static_cast<std::int64_t>(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static T deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return static_cast<T>(value.GetInt64());
        }
    };

    template<typename T>
    struct SerializerSpec<T*>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, T* value) noexcept
        {
            if(value)
            {
                Serializer::serialize(toDocument, parent, varName, *value);
            }
            else
            {
                rapidjson::Value k(rapidjson::kStringType);
                k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

                rapidjson::Value v(rapidjson::kNullType);

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
        }

        static T* deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            T* outputValue = nullptr;

            if(value.GetType() != rapidjson::kNullType)
            {
                outputValue = new T(SerializerSpec<T>::deserialize(parent, value, outputLog));
            }

            return outputValue;
        }
    };

    template<typename T>
    struct SerializerSpec<std::shared_ptr<T>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, std::shared_ptr<T> value) noexcept
        {
            if(value)
            {
                Serializer::serialize(toDocument, parent, varName, *value);
            }
            else
            {
                rapidjson::Value k(rapidjson::kStringType);
                k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

                rapidjson::Value v(rapidjson::kNullType);

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
        }

        static std::shared_ptr<T> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::shared_ptr<T> outputValue;

            if(value.GetType() != rapidjson::kNullType)
            {
                outputValue = std::shared_ptr<T>(SerializerSpec<T*>::deserialize(parent, value, outputLog));
            }

            return outputValue;
        }
    };

    template<typename T>
    struct SerializerSpec<std::unique_ptr<T>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, std::unique_ptr<T> value) noexcept
        {
            if(value)
            {
                Serializer::serialize(toDocument, parent, varName, *value);
            }
            else
            {
                rapidjson::Value k(rapidjson::kStringType);
                k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

                rapidjson::Value v(rapidjson::kNullType);

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
        }

        static std::unique_ptr<T> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::unique_ptr<T> outputValue;

            if(value.GetType() != rapidjson::kNullType)
            {
                outputValue = std::shared_ptr<T>(SerializerSpec<T*>::deserialize(parent, value, outputLog));
            }

            return outputValue;
        }
    };

    /*template<typename T>
    struct SerializerSpec<std::weak_ptr<T>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, std::weak_ptr<T> value) noexcept
        {
            auto lockedValue = value.lock();

            if(lockedValue)
            {
                Serializer::serialize(toDocument, parent, varName, *lockedValue);
            }
            else
            {
                rapidjson::Value k(rapidjson::kStringType);
                k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

                rapidjson::Value v(rapidjson::kNullType);

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
        }

        static bool deserialize(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
        {
            if (!parent.HasMember(varName.c_str()))
            {
                Serializer::formNotExistingMemberError(parent, varName, outputLog);

                return {};
            }

            auto& self = parent[varName.c_str()];

            return self.GetBool();
        }
    };*/

    template<>
    struct SerializerSpec<char>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const char& value) noexcept
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

        static char deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetStringLength() > 0 ? value.GetString()[0] : ' ';
        }
    };
    
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

        static std::int8_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetInt();
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

        static std::int16_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetInt();
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

        static std::int32_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetInt();
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

        static std::int64_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetInt64();
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

        static std::uint8_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
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

        static std::uint16_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
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

        static std::uint32_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
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

        static std::uint64_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint64();
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

        static double deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetDouble();
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

        static float deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetFloat();
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

        static bool deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetBool();
        }
    };
    
    template<std::int32_t ScalarsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::vec<ScalarsCnt, ScalarT, Qualifier>>
    {
        using vec_t = glm::vec<ScalarsCnt, ScalarT, Qualifier>;

        static_assert(ScalarsCnt >= 1 && ScalarsCnt <= 4 && "Scalars count in glm::vec must be in range of 1-4.");

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const vec_t& value) noexcept
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

        static vec_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            vec_t outputValue;

            outputValue.x = value["x"];
            if constexpr(ScalarsCnt >= 2)
            {
                outputValue.y = value["y"];
            }
            if constexpr(ScalarsCnt >= 3)
            {
                outputValue.z = value["z"];
            }
            if constexpr(ScalarsCnt >= 4)
            {
                outputValue.w = value["w"];
            }

            return outputValue;
        }
    };
    
    template<std::int32_t ColumnsCnt, std::int32_t RowsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>>
    {
        using mat_t = glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>;

        static_assert(ColumnsCnt >= 1 && ColumnsCnt <= 4 && "Columns count in glm::mat must be in range of 1-4.");
        static_assert(RowsCnt >= 1 && RowsCnt <= 4 && "Rows count in glm::mat must be in range of 1-4.");
        
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const mat_t& value) noexcept
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

        static mat_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            mat_t outputValue;

            for(std::int32_t c = 0; c < ColumnsCnt; ++c)
            {
                for(std::int32_t r = 0; r < RowsCnt; ++r)
                {
                    outputValue[c][r] = value[c * ColumnsCnt + r];
                }
            }

            return outputValue;
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

        static std::vector<T> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::vector<T> outputValue;

            for(std::uint64_t i = 0; i < value.Size(); ++i)
            {
                outputValue.push_back(SerializerSpec<T>::deserialize(value, value[i], outputLog));
            }

            return outputValue;
        }
    };

    /// KeyT REQUIRES AN IMPLICIT CONVERSION OPERATOR TO std::string OR OTHER TYPES FROM
    /// WHICH std::string CAN BE CONSTRUCTED OR WHICH CAN BE IMPLICITLY CONVERTED TO std::string
    template<typename KeyT, typename ValueT>
    struct SerializerSpec<std::unordered_map<KeyT, ValueT>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::unordered_map<KeyT, ValueT>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            for(const auto& n : value)
            {
                Serializer::serialize(toDocument, v, n.first, n.second);
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
            };
        }

        static std::unordered_map<KeyT, ValueT> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::unordered_map<KeyT, ValueT> outputValue;

            for (rapidjson::Value::ConstMemberIterator iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
            {
                outputValue[iter->name.GetString()] = SerializerSpec<ValueT>::deserialize(value, iter->value, outputLog);
            }

            return outputValue;
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

        static std::string deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
        }
    };

    template<>
    struct SerializerSpec<std::filesystem::path>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::filesystem::path& value) noexcept
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
                    parent.AddMember(k, rapidjson::StringRef(Utils::toUTF8<char16_t>(value.u16string()).c_str()), toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(rapidjson::StringRef(Utils::toUTF8<char16_t>(value.u16string()).c_str()), toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::string deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
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

        static const char* deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
        }
    };

    template<typename T>
    concept CustomSerializationStruct =
    requires(T obj,
            rapidjson::Document& document,
            rapidjson::Value& parent,
            const std::string& varName,
            const rapidjson::Value& jsonValue,
            std::string& outputLog) {
        obj.serializeData(document, parent, varName);
        obj.serializeMeta(document, parent, varName);
        obj.deserializeData(parent, jsonValue, outputLog);
        obj.deserializeMeta(parent, jsonValue, outputLog);
        obj.m_serializationType;
    };

    template<CustomSerializationStruct T>
    struct SerializerSpec<T>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, T& value) noexcept
        {
            switch(value.m_serializationType)
            {
                case SerializationType::SERIALIZE_META:
                    value.serializeMeta(toDocument, parent, varName);
                    break;
                case SerializationType::SERIALIZE_DATA:
                    value.serializeData(toDocument, parent, varName);
                    break;
            }
        }

        static T deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            T outputValue;

            switch(outputValue.m_serializationType)
            {
                case SerializationType::SERIALIZE_META:
                    outputValue.deserializeMeta(parent, value, outputLog);
                    break;
                case SerializationType::SERIALIZE_DATA:
                    outputValue.deserializeData(parent, value, outputLog);
                    break;
            }

            return outputValue;
        }
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::AnnotationArg>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::AnnotationArg& value) noexcept;

        static AnnotationsProcessor::AnnotationArg deserialize(const rapidjson::Value& parent,
                                                               const rapidjson::Value& value,
                                                               std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::Annotation>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::Annotation& value) noexcept;

        static AnnotationsProcessor::Annotation deserialize(const rapidjson::Value& parent,
                                                            const rapidjson::Value& value,
                                                            std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::Member>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::Member& value) noexcept;

        static AnnotationsProcessor::Member deserialize(const rapidjson::Value& parent,
                                                        const rapidjson::Value& value,
                                                        std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor& value) noexcept;

        static AnnotationsProcessor deserialize(const rapidjson::Value& parent,
                                                const rapidjson::Value& value,
                                                std::string& outputLog) noexcept;
    };
}

#endif //SUNGEARENGINE_SERIALIZER_H
