#ifndef SUNGEARENGINE_ENTRY_H
#define SUNGEARENGINE_ENTRY_H

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Annotations/Annotations.h>
#include <SGCore/Scene/Serializer.h>
#include <SGCore/Utils/TypeTraits.h>
#include "SGCore/Scene/NewSerializer.h"

struct Base
{
    int a = 3;

    virtual void v()
    {

    }
};

struct Derived : Base
{
    float b = 3.14f;
};

struct Derived0 : Derived
{
    float c = 5.1f;
};

template<SGCore::NewSerde::FormatType TFormatType>
struct SGCore::NewSerde::SerdeSpec<Derived0, TFormatType> : SGCore::NewSerde::BaseTypes<Derived>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Derived0";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::NewSerde::SerializableValueView<Derived0, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("c", valueView.m_data->c);
        std::printf("derived0 serializing\n");
    }

    static void deserialize(SGCore::NewSerde::DeserializableValueView<Derived0, TFormatType>& valueView) noexcept
    {
        const auto c = valueView.getValueContainer().template getMember<float>("c");
        if(c)
        {
            valueView.m_data->c = *c;
        }

        std::printf("derived0 deserializing\n");
    }
};

template<SGCore::NewSerde::FormatType TFormatType>
struct SGCore::NewSerde::SerdeSpec<Derived, TFormatType> : SGCore::NewSerde::BaseTypes<Base>, SGCore::NewSerde::DerivedTypes<Derived0>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Derived";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::NewSerde::SerializableValueView<Derived, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("b", valueView.m_data->b);
        std::printf("derived serializing\n");
    }

    static void deserialize(SGCore::NewSerde::DeserializableValueView<Derived, TFormatType>& valueView) noexcept
    {
        const auto b = valueView.getValueContainer().template getMember<float>("b");
        if(b)
        {
            valueView.m_data->b = *b;
        }

        std::printf("derived deserializing\n");
    }
};

template<SGCore::NewSerde::FormatType TFormatType>
struct SGCore::NewSerde::SerdeSpec<Base, TFormatType> : SGCore::NewSerde::DerivedTypes<Derived>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Base";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::NewSerde::SerializableValueView<Base, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("a", valueView.m_data->a);
        std::printf("base serializing\n");
    }

    static void deserialize(SGCore::NewSerde::DeserializableValueView<Base, TFormatType>& valueView) noexcept
    {
        const auto a = valueView.getValueContainer().template getMember<std::int32_t>("a");
        if(a)
        {
            valueView.m_data->a = *a;
        }
        std::printf("base deserializing\n");
    }
};

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
