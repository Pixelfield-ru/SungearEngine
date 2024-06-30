//
// Created by ilya on 30.06.24.
//

#ifndef SUNGEARENGINE_TESTSTRUCT_H
#define SUNGEARENGINE_TESTSTRUCT_H

#include "SGCore/Annotations/Annotations.h"
#include <string>
#include <vector>

namespace TestNamespace
{
    sg_member()
    int k = 0;
    
    sg_struct(fullName = "TestNamespace::TestStruct")
    sg_component()
    struct TestStruct
    {
        
        sg_struct()
        struct TestStruct2
        {
        
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
        std::vector<float> vec { };
        
        bool m_transformChanged = { { false } };
    };
    
    // sg_struct(fullName = "TestNamespace::TestStruct2")
}

#endif //SUNGEARENGINE_TESTSTRUCT_H
