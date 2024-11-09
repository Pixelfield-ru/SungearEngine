//
// Created by stuka on 23.10.2024.
//

#ifndef SUNGEARENGINE_STATICTYPEID_H
#define SUNGEARENGINE_STATICTYPEID_H

#include <set>
#include "SGCore/Logger/Logger.h"

/// Pass current class type as first argument and its type ID as second argument.\n
/// Use this macro in derived types to implement function \p getTypeID() that overrides virtual function in base type\n
/// \p getTypeID() is needed to get real static type ID of object.
#define sg_implement_type_id(current_class, class_type_id)                          \
static inline size_t type_id = StaticTypeID<current_class>::setID(class_type_id);   \
const size_t& getTypeID() const noexcept override { return class_type_id; }

/// Pass current class type as first argument and its type ID as second argument.\n
/// Use this macro in base types to implement virtual function \p getTypeID() .\n
/// \p getTypeID() is needed to get real static type ID of object.
#define sg_implement_type_id_base(current_class, class_type_id)                          \
static inline size_t type_id = StaticTypeID<current_class>::setID(class_type_id);        \
virtual const size_t& getTypeID() const noexcept { return class_type_id; }

namespace SGCore
{
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
            if(m_existingTypeIDs.contains(typeID))
            {
                LOG_C(SGCORE_TAG, "Can not set type ID '{}' for type '{}': some other type with this ID is already exists.", typeID, typeid(T).name());
                std::exit(0);
            }

            m_existingTypeIDs.insert(m_typeID);

            return m_typeID;
        }

        static inline size_t m_typeID = 0;

        static inline std::set<size_t> m_existingTypeIDs;
    };
}

#endif //SUNGEARENGINE_STATICTYPEID_H
