//
// Created by stuka on 23.10.2024.
//

#pragma once

#include <set>
#include <iostream>
#include <fmt/format.h>
#include <sgcore_export.h>

#include "SGCore/Utils/TypeTraits.h"

/// Pass current class type as first argument. PLEASE, QUALIFY YOUR TYPE BY NAMESPACE IN WHICH THE TYPE IS LOCATED.\n
/// Use this macro in derived types to implement function \p getTypeID() that overrides virtual function in base type\n
/// \p getTypeID() is needed to get real static type ID of object.\n
/// Implementation of this macro must has public access.
#define sg_implement_type_id(current_class)                          \
static size_t getTypeIDStatic() { static size_t typeID = SGCore::StaticTypeID<current_class>::setID(SGCore::constexprHash(#current_class)); return typeID; }   \
size_t getTypeID() const noexcept final { return current_class::getTypeIDStatic(); }

/// Pass current class type as first argument. PLEASE, QUALIFY YOUR TYPE BY NAMESPACE IN WHICH THE TYPE IS LOCATED. \n
/// Use this macro in base types to implement virtual function \p getTypeID() .\n
/// \p getTypeID() is needed to get real static type ID of object.\n
/// Implementation of this macro must has public access.
#define sg_implement_type_id_base(current_class)                          \
static size_t getTypeIDStatic() { static size_t typeID = SGCore::StaticTypeID<current_class>::setID(SGCore::constexprHash(#current_class)); return typeID; }        \
virtual size_t getTypeID() const noexcept { return current_class::getTypeIDStatic(); }

/// Pass current class type as first argument. PLEASE, QUALIFY YOUR TYPE BY NAMESPACE IN WHICH THE TYPE IS LOCATED.\n
/// Creates only static function to get type ID without virtual function to get type ID of object.
/// Implementation of this macro must has public access.
#define sg_implement_nonvirtual_type_id(current_class)                    \
static size_t getTypeIDStatic() { static size_t typeID = SGCore::StaticTypeID<current_class>::setID(SGCore::constexprHash(#current_class)); return typeID; }

namespace SGCore
{
    struct SGCORE_EXPORT StaticTypeIDsContainer
    {
        template<typename>
        friend struct StaticTypeID;

        static bool isTypeIDExists(const size_t& typeID) noexcept;

    private:
        static void addTypeID(size_t typeID) noexcept;

        static std::set<size_t>& getExistingTypeIDs() noexcept;
    };

    template<typename T>
    struct StaticTypeID
    {
        friend T;

        static size_t getID()
        {
            return m_typeID;
        }

    private:
        static size_t setID(size_t typeID)
        {
            m_typeID = typeID;
            if(StaticTypeIDsContainer::isTypeIDExists(typeID))
            {
                const std::string message = fmt::format("Can not set type ID '{}' for type '{}': some other type with this ID is already exists.", typeID, typeid(T).name());
                std::cerr << message;
                std::exit(0);
            }

            StaticTypeIDsContainer::addTypeID(m_typeID);

            return m_typeID;
        }

        static inline size_t m_typeID = 0;
    };
}
