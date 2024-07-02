//
// Created by ilya on 30.06.24.
//

#ifndef SUNGEARENGINE_TESTSTRUCT_H
#define SUNGEARENGINE_TESTSTRUCT_H

#include "SGCore/Annotations/Annotations.h"
#include "SGCore/Scene/Serializer.h"
#include <string>
#include <vector>

namespace TestNamespace
{
    template<typename T>
    struct MyStruct
    {
    
    };
    
    template<>
    sg_struct()
    struct MyStruct<std::int8_t>
    {
    
    };
    
    sg_member()
    int k = 0;
    
    sg_struct(type="component")
    struct TestStruct
    {
        sg_struct()
        struct TestStruct2
        {
            friend struct TestStruct;
            
            sg_member()
            int a = 32;
        };
        
        struct TestStruct3
        {
            sg_member()
            int b = 0;
        };
        
        sg_member()
        int i=3;
        
        sg_member()
        std::string str { "sdfsdf" };
        
        // will transform follow parent entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        sg_member()
        std::vector<float> vec { 0.5, 5 };
        
        sg_member()
        bool m_bool = { { false } };
    };
    
    // sg_struct(fullName = "TestNamespace::TestStruct2")
}

template<>
sg_struct(type="serializer")
struct SGCore::SerializerSpec<TestNamespace::TestStruct>
{
    sg_member()
    int m_amem = 4;
    
    static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                          const std::string& varName, const TestNamespace::TestStruct& value) noexcept
    {
    }
};

// здесь используется проверка на то, что такая специализация SGCore::SerializerSpec уже определена
/*template<>
struct SGCore::SerializerSpec<std::conditional_t<requires { SGCore::SerializerSpec<decltype(TestNamespace::TestStruct::str)>::serialize; },
        struct TestNamespace_TestStruct_str_, SGCore::SerializerSpec<decltype(TestNamespace::TestStruct::str)>>>
{

};*/

#endif //SUNGEARENGINE_TESTSTRUCT_H
