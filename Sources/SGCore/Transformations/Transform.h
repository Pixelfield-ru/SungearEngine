//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_TRANSFORM_H
#define SUNGEARENGINE_TRANSFORM_H

#include <SGCore/pch.h>

#include "TransformBase.h"
#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Scene/Serializer.h"
#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    // sizeof(Transform) = 767
    sg_struct(type = "component")
    struct Transform
    {
        sg_member()
        TransformBase m_finalTransform;
        
        sg_member()
        TransformBase m_ownTransform;

        // will transform follow parent entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        sg_member()
        glm::vec<3, bool, glm::highp> m_followParentTRS { true, true, true };
        // glm::bvec3 m_lastFollowParentTRS = glm::vec3 { false };
        
        bool m_transformChanged = false;
    };

    /*template<>
    struct SerializerSpec<Transform>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const Transform& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length());

            Serializer::serialize(toDocument, v, "finalTransform", value.m_finalTransform);
            Serializer::serialize(toDocument, v, "ownTransform", value.m_ownTransform);

            Serializer::serialize(toDocument, v, "followParentTRS", value.m_followParentTRS);
            Serializer::serialize(toDocument, v, "transformChanged", value.m_transformChanged);

            parent.AddMember(k, v, toDocument.GetAllocator());
        }
    };*/
}

#endif //SUNGEARENGINE_TRANSFORM_H
