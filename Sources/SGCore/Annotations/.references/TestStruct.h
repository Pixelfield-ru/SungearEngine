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
    sg_struct(template = [(type = "typename", name = "T")])
    struct MyStruct
    {
        sg_function(name = "getMyMember", getterFor = "m_myMember")
        const T& getMyMember() const noexcept
        {
            return m_myMember;
        }

        sg_function(name = "setMyMember", setterFor = "m_myMember")
        void setMyMember(const T& val) noexcept
        {
            m_myMember = val;
        }

    private:
        sg_member()
        T m_myMember;
    };
    
    // sg_member()
    int k = 0;
    
    sg_struct(type = "component")
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
