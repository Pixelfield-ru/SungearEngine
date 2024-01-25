//
// Created by stuka on 04.01.2024.
//

#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <memory_resource>

#include "TypeInfo.h"

namespace SGECS
{
    struct Rotation
    {
        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_z = 0.0f;

        float m_arr[10] {};

        [[nodiscard]] std::string toString() const noexcept
        {
            return "x: " + std::to_string(m_x) + ", y: " + std::to_string(m_y) + ", z: " + std::to_string(m_z);
        }
    };

    struct OtherRotation : Rotation
    {
    SGECS_DECLARE_BASE(OtherRotation, Rotation)
    };

    struct AnotherRotation : OtherRotation
    {
    SGECS_DECLARE_BASE(AnotherRotation, OtherRotation)
    };

    struct AnotherRotation1 : AnotherRotation
    {
    SGECS_DECLARE_BASE(AnotherRotation1, AnotherRotation)
    };

    struct Position
    {
        std::pmr::string m_str;

        using allocator_type = std::pmr::polymorphic_allocator<>;

        float m_x = 0.0f;
        float m_y = 0.0f;

        Position() = default;

        Position(const Position&) = default;

        Position(Position&&) = default;

        explicit Position(const std::pmr::polymorphic_allocator<>& allocator) : m_str(allocator)
        {}

        explicit Position(const Position& other, const std::pmr::polymorphic_allocator<>& allocator) : m_str(
                other.m_str, allocator
        )
        {}

        explicit Position(Position&& other, const std::pmr::polymorphic_allocator<>& allocator) : m_str(
                std::move(other.m_str), allocator
        )
        {}

        [[nodiscard]] std::string toString() const noexcept
        {
            return "x: " + std::to_string(m_x) + ", y: " + std::to_string(m_y);
        }
    };

    struct Velocity
    {
        float m_dx = 0.0f;
        float m_dy = 0.0f;

        [[nodiscard]] std::string toString() const noexcept
        {
            return "dx: " + std::to_string(m_dx) + ", dy: " + std::to_string(m_dy);
        }
    };

    struct SomeComponent
    {
        void* m_array = malloc(10240);
    };
}

#endif //ECS_COMPONENTS_H
