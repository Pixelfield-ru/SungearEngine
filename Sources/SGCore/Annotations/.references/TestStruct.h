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
    
    sg_struct(fullName = ["TestNamespace::TestStruct", "struct"], type = "component")
    struct TestStruct
    {
        sg_struct()
        struct TestStruct2
        {
            friend struct TestStruct;
            
            sg_member()
            int a = 32;
        };
        
        sg_struct()
        struct TestStruct3
        {
            sg_member()
            int b = 0;
        };
        
        sg_member()
        int i=3;
        
        sg_member(serializableName = "myName")
        std::string str { "sdfsdf" };
        
        sg_member()
        std::vector<float> m_stdvec { 0.5, 5 };
        
        sg_member()
        glm::vec3 m_glmvec { 0, 1, 0 };
        
        sg_member()
        TestStruct3 m_testStruct3 = { };
        
        sg_member()
        bool m_bool = { false };
    };
}

#endif //SUNGEARENGINE_TESTSTRUCT_H
