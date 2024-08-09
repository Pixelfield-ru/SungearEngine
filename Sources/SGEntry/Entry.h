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
struct SGCore::SerializerSpec<Derived> : SGCore::BaseClasses<Base>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Derived";

    static void serialize(rapidjson::Document& toDocument,
                          rapidjson::Value& parentKey,
                          rapidjson::Value& parent,
                          rapidjson::Value& valueTypeName,
                          const Derived& value) noexcept;

    static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                   const std::string& typeName,
                                   std::string& outputLog,
                                   Derived*& outputValue) noexcept;

    static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                  const std::string& typeName,
                                  std::string& outputLog,
                                  Derived& outputValue) noexcept;
};

template<>
struct SGCore::SerializerSpec<Base> : SGCore::DerivedClasses<Derived>
{
    static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
    static inline const std::string type_name = "Base";

    static void serialize(rapidjson::Document& toDocument,
                          rapidjson::Value& parentKey,
                          rapidjson::Value& parent,
                          rapidjson::Value& valueTypeName,
                          const Base& value) noexcept;

    static void deserializeDynamic(const rapidjson::Value& parent,
                                   const rapidjson::Value& value,
                                   const std::string& typeName,
                                   std::string& outputLog,
                                   Base*& outputValue) noexcept;

    static void deserializeStatic(const rapidjson::Value& parent,
                                  const rapidjson::Value& value,
                                  const std::string& typeName,
                                  std::string& outputLog,
                                  Base& outputValue) noexcept;
};

void coreInit();

void onUpdate(const double& dt, const double& fixedDt);

int main();

#endif //SUNGEARENGINE_ENTRY_H
