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
    sg_struct(fullName="TestNamespace::TestStruct")
    sg_component()
    struct TestStruct
    {
        sg_serializable(key = "i")
        int i = 3;
        
        sg_serializable(key = "str")
        std::string str;
        
        // will transform follow parent entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        sg_serializable(key = "vec")
        std::vector<float> vec;
        
        bool m_transformChanged = false;
    };
    
    // sg_struct(fullName = "TestNamespace::TestStruct2")
}

#endif //SUNGEARENGINE_TESTSTRUCT_H
