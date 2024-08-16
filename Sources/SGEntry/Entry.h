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

template<SGCore::NewSerde::FormatType TFormatType>
struct SGCore::NewSerde::SerdeSpec<Derived, TFormatType> : SGCore::NewSerde::BaseTypes<Base>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Derived";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::NewSerde::ISerializableValueView<Derived>& valueView) noexcept
    {
        std::printf("derived serializing\n");
    }

    static void deserialize(SGCore::NewSerde::IDeserializableValueView<Derived>& valueView) noexcept
    {

    }
};

template<SGCore::NewSerde::FormatType TFormatType>
struct SGCore::NewSerde::SerdeSpec<Base, TFormatType> : SGCore::NewSerde::DerivedTypes<Derived>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Base";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::NewSerde::ISerializableValueView<Base>& valueView) noexcept
    {
        std::printf("base serializing\n");
    }

    static void deserialize(SGCore::NewSerde::IDeserializableValueView<Base>& valueView) noexcept
    {

    }
};

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
