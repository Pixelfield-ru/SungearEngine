#ifndef SUNGEARENGINE_ENTRY_H
#define SUNGEARENGINE_ENTRY_H

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Annotations/Annotations.h>
#include <SGCore/Scene/Serializer.h>
#include <SGCore/Utils/TypeTraits.h>

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

template<>
struct SGCore::Serde::SerializerSpec<Derived> : SGCore::Serde::BaseClasses<Base>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Derived";

    static void serialize(SGCore::Serde::ValueDataView<Derived>& dataView) noexcept;

    static void deserialize(SGCore::Serde::ValueDataView<Derived>& dataView) noexcept;
};

template<>
struct SGCore::Serde::SerializerSpec<Base> : SGCore::Serde::DerivedClasses<Derived>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Base";

    static void serialize(SGCore::Serde::ValueDataView<Base>& dataView) noexcept;

    static void deserialize(SGCore::Serde::ValueDataView<Base>& dataView) noexcept;
};

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
