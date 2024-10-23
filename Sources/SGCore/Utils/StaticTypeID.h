//
// Created by stuka on 23.10.2024.
//

#ifndef SUNGEARENGINE_STATICTYPEID_H
#define SUNGEARENGINE_STATICTYPEID_H

#include <set>
#include "SGCore/Logger/Logger.h"

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
