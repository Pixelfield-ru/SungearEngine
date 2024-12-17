//
// Created by stuka on 17.12.2024.
//

#ifndef SUNGEARENGINE_PICKABLE_H
#define SUNGEARENGINE_PICKABLE_H

namespace SGCore
{
    // TAG COMPONENT FORM PICKABLE ENTITIES
    struct Pickable
    {
    private:
        bool m_dummy = false;
    };
}

/*
 struct EntityRef
 {
    Ref<entity_t> m_referencesEntity;
 };

 struct EntityBaseInfo
 {
    std::vector<Ref<entity_t>> m_entitiesRefsToResolve;
 };

 */

#endif //SUNGEARENGINE_PICKABLE_H
