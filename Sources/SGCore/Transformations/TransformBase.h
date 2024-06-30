//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMCOMPONENT_H
#define NATIVECORE_TRANSFORMCOMPONENT_H

#include <SGCore/pch.h>

#include "SGCore/Math/MathUtils.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/Event.h"
#include "SGCore/Math/AABB.h"
#include "SGCore/Scene/Serializer.h"
#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    // sizeof(TransformBase) == 382
    // todo: make quaternion transformations
    sg_struct(fullName = "SGCore::TransformBase")
    struct TransformBase
    {
        friend struct TransformationsUpdater;

        sg_serializable(key = "blockTranslation")
        bool m_blockTranslation = false;
        sg_serializable(key = "blockRotation")
        bool m_blockRotation = false;
        sg_serializable(key = "blockScale")
        bool m_blockScale = false;

        sg_serializable(key = "aabb")
        AABB<> m_aabb;

        sg_serializable(key = "position")
        glm::vec3 m_position { 0.0 };
        sg_serializable(key = "rotation")
        glm::vec3 m_rotation { 0.0 };
        sg_serializable(key = "scale")
        glm::vec3 m_scale { 1.0 };

        sg_serializable(key = "left")
        glm::vec3 m_left = MathUtils::left3;
        sg_serializable(key = "forward")
        glm::vec3 m_forward = MathUtils::forward3;
        sg_serializable(key = "up")
        glm::vec3 m_up = MathUtils::up3;

        glm::mat4 m_translationMatrix = glm::mat4(1);
        glm::mat4 m_rotationMatrix = glm::mat4(1);
        glm::mat4 m_scaleMatrix = glm::mat4(1);

        bool m_positionChanged = false;
        bool m_rotationChanged = false;
        bool m_scaleChanged = false;

        glm::mat4 m_modelMatrix = glm::mat4(1);

    // private:
        glm::vec3 m_lastPosition { 0.0 };
        glm::vec3 m_lastRotation { 0.0 };
        glm::vec3 m_lastScale = glm::vec3(0);
    };

    /*template<>
    struct SerializerSpec<TransformBase>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const TransformBase& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            Serializer::serialize(toDocument, v, "blockTranslation", value.m_blockTranslation);
            Serializer::serialize(toDocument, v, "blockRotation", value.m_blockRotation);
            Serializer::serialize(toDocument, v, "blockScale", value.m_blockScale);

            Serializer::serialize(toDocument, v, "aabb", value.m_aabb);

            Serializer::serialize(toDocument, v, "position", value.m_position);
            Serializer::serialize(toDocument, v, "rotation", value.m_rotation);
            Serializer::serialize(toDocument, v, "scale", value.m_scale);

            Serializer::serialize(toDocument, v, "left", value.m_left);
            Serializer::serialize(toDocument, v, "forward", value.m_forward);
            Serializer::serialize(toDocument, v, "up", value.m_up);

            parent.AddMember(k, v, toDocument.GetAllocator());
        }
    };*/
}

#endif //NATIVECORE_TRANSFORMCOMPONENT_H
