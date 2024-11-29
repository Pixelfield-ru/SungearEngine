//
// Created by stuka on 23.10.2024.
//

#ifndef SUNGEARENGINE_STATICTYPEID_H
#define SUNGEARENGINE_STATICTYPEID_H

#include <set>
#include "SGCore/Logger/Logger.h"

/// Pass current class type as first argument and its type ID as second argument.\n
/// Use this macro in derived types to implement function \p getTypeID() that overrides virtual function in base type\n
/// \p getTypeID() is needed to get real static type ID of object.\n
/// Implementation of this macro must has public access.
#define sg_implement_type_id(current_class, class_type_id)                          \
static inline size_t type_id = SGCore::StaticTypeID<current_class>::setID(class_type_id);   \
const size_t& getTypeID() const noexcept final { return type_id; }

/// Pass current class type as first argument and its type ID as second argument.\n
/// Use this macro in base types to implement virtual function \p getTypeID() .\n
/// \p getTypeID() is needed to get real static type ID of object.\n
/// Implementation of this macro must has public access.
#define sg_implement_type_id_base(current_class, class_type_id)                          \
static inline size_t type_id = SGCore::StaticTypeID<current_class>::setID(class_type_id);        \
virtual const size_t& getTypeID() const noexcept { return type_id; }

/// Pass current class type as first argument and its type ID as second argument.\n
/// Creates only static inline \p typeID without virtual function to get type ID of object
/// Implementation of this macro must has public access.
#define sg_implement_nonvirtual_type_id(current_class, class_type_id)                    \
static inline size_t type_id = SGCore::StaticTypeID<current_class>::setID(class_type_id);

namespace SGCore
{
    struct StaticTypeIDsContainer
    {
        template<typename>
        friend struct StaticTypeID;

        SG_NOINLINE static bool isTypeIDExists(const size_t& typeID) noexcept
        {
            return m_existingTypeIDs.contains(typeID);
        }

    private:
        SG_NOINLINE static void addTypeID(size_t typeID) noexcept
        {
            m_existingTypeIDs.insert(typeID);
        }

        static inline std::set<size_t> m_existingTypeIDs;
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
                //LOG_C(SGCORE_TAG, "Can not set type ID '{}' for type '{}': some other type with this ID is already exists.", typeID, typeid(T).name());
                std::exit(0);
            }

            StaticTypeIDsContainer::addTypeID(m_typeID);

            return m_typeID;
        }

        static inline size_t m_typeID = 0;
    };
}

#endif //SUNGEARENGINE_STATICTYPEID_H
