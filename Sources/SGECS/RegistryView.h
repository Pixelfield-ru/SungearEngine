//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_REGISTRYVIEW_H
#define ECS_REGISTRYVIEW_H

#include <string>
#include <functional>
#include <tuple>
#include <iostream>
#include <tsl/sparse_map.h>
#include <map>

#include "SGUtils/Utils.h"
#include "TypeInfo.h"
#include "ComponentsAllocator.h"
#include "SGUtils/EventListenerHolder.h"
#include "ComponentWrapper.h"
#include "SGUtils/TypeInfo.h"

namespace SGECS
{
    template<typename... ComponentsT>
    class RegistryView
    {
        friend class Registry;

    public:
        RegistryView(const std::initializer_list<size_t>& entitiesToCollect) noexcept
        {
            m_entitiesToCollect = entitiesToCollect;
            m_isCollectingCertainEntities = true;
        }
        RegistryView() = default;
        RegistryView(const RegistryView&) = default;
        RegistryView(RegistryView&&) noexcept = default;

        template<typename Func>
        requires(std::is_invocable_v<Func, const size_t&, ComponentsT* ...>)
        void each(const Func& func) noexcept
        {
            for(auto& entityTuplesIter: m_formedTuplesToApply)
            {
                const auto& entity = entityTuplesIter.first;
                auto& tuples = entityTuplesIter.second;

                for(auto& tuple: tuples)
                {
                    std::apply([&entity, &func](ComponentsT* ... components)
                               { func(entity, components...); }, tuple.m_tuple
                    );
                }
            }
        }

        template<typename Func>
        requires(std::is_invocable_v<Func, ComponentsT* ...>)
        void each(const Func& func) noexcept
        {
            for(auto& entityTuplesIter: m_formedTuplesToApply)
            {
                auto& tuples = entityTuplesIter.second;
                for(auto& tuple: tuples)
                {
                    std::apply(func, tuple.m_tuple);
                }
            }
        }

        template<typename Func>
        requires(std::is_invocable_v<Func, ComponentsT* ...>)
        void forEntity(const size_t& entity, const Func& func) noexcept
        {
            auto foundTuples = m_formedTuplesToApply.find(entity);

            if(foundTuples != m_formedTuplesToApply.end())
            {
                for(auto& tuple : foundTuples->second)
                {
                    std::apply(func, tuple.m_tuple);
                }
            }
        }

        RegistryView& operator=(const RegistryView&) noexcept = default;
        RegistryView& operator=(RegistryView&&) noexcept = default;

    private:
        template<typename... Types>
        struct TupleToApply
        {
            std::tuple<Types* ...> m_tuple;
            size_t m_elementsFilled = 0;
            bool m_isFormed = true;
        };

        template<std::size_t I = 0>
        inline typename std::enable_if<I == sizeof...(ComponentsT), void>::type
        checkTupleForFullFormed(TupleToApply<ComponentsT...>& tuple) const noexcept
        {
            tuple.m_isFormed = tuple.m_elementsFilled == sizeof...(ComponentsT);
        }

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), void>::type
        checkTupleForFullFormed(TupleToApply<ComponentsT...>& tuple) noexcept
        {
            if(I == 0)
            {
                tuple.m_elementsFilled = 0;
            }

            auto& tupleElem = std::get<I>(tuple.m_tuple);
            using t = std::remove_reference<decltype(*tupleElem)>::type;

            if(tupleElem)
            {
                ++tuple.m_elementsFilled;
            }

            checkTupleForFullFormed<I + 1>(tuple);
        }

        template<typename ComponentBaseT, std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), void>::type
        collectAllComponentsOfType(RegistryAllocator& registryAllocator, const size_t& componentIdx, const size_t& typeID) noexcept
        {
            Pool* pool = registryAllocator.getPoolByTypeID(typeID);
            if(pool && pool->getFarthestIndex() > componentIdx)
            {
                ComponentWrapper<ComponentBaseT>* wrapper = nullptr;
                wrapper = pool->template getObjectByIndex<ComponentWrapper<ComponentBaseT>>(
                        componentIdx
                );
                if(!wrapper || (wrapper->m_entityID == (size_t) (-1) ||
                                (m_isCollectingCertainEntities && !m_entitiesToCollect.contains(wrapper->m_entityID))))
                {
                    return;
                }

                std::vector<TupleToApply<ComponentsT...>>& tuplesVec = m_notFormedTuplesToApply[wrapper->m_entityID];

                bool tupleFound = false;
                for(auto& tuple: tuplesVec)
                {
                    auto& tupleVal = std::get<I>(tuple.m_tuple);
                    if(!tupleVal)
                    {
                        tupleVal = &wrapper->m_component;
                        tupleFound = true;
                        break;
                    }
                }

                if(!tupleFound)
                {
                    tuplesVec.push_back({});

                    auto& tupleToUse = *tuplesVec.rbegin();
                    std::get<I>(tupleToUse.m_tuple) = &(wrapper->m_component);
                }
            }
        }

        template<std::size_t I = 0>
        inline typename std::enable_if<I == sizeof...(ComponentsT), void>::type
        collectAllComponents(RegistryAllocator& registryAllocator, const size_t& componentIdx) const noexcept
        {}

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), void>::type
        collectAllComponents(RegistryAllocator& registryAllocator, const size_t& componentIdx) noexcept
        {
            using baseType = std::remove_reference<decltype(*std::get<I>(m_technicalTuple))>::type;

            auto& baseTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<baseType>();

            collectAllComponentsOfType<baseType, I>(registryAllocator, componentIdx, baseTypeInfo.getID());

            for(size_t derivedType : baseTypeInfo.getDerivedTypes())
            {
                collectAllComponentsOfType<baseType, I>(registryAllocator, componentIdx, derivedType);
            }

            collectAllComponents<I + 1>(registryAllocator, componentIdx);
        }

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        TupleToApply<ComponentsT...>&
        createTupleAndPut(RegistryAllocator& registryAllocator, std::vector<TupleToApply<ComponentsT...>>& tuplesVec,
                          const size_t& entity, const void* componentWrapper,
                          const size_t& componentToPutTypeID,
                          bool& tupleCreated) noexcept
        {
            if constexpr(I < sizeof...(ComponentsT))
            {
                using type = std::remove_reference<decltype(*std::get<I>(m_technicalTuple))>::type;

                size_t componentTypeID = SGUtils::TypeInfoUtils::getTypeInfo<type>().getID();

                Pool* pool = registryAllocator.getPoolByType<type>();
                if(!pool || (componentToPutTypeID != componentTypeID &&
                   !SGUtils::TypeInfoUtils::getTypeInfo(componentTypeID).getDerivedTypes().contains(componentToPutTypeID)))
                    return createTupleAndPut<I + 1>(registryAllocator, tuplesVec, entity, componentWrapper,
                                                    componentToPutTypeID, tupleCreated
                    );

                tuplesVec.push_back({});

                auto& tupleToUse = *tuplesVec.rbegin();
                std::get<I>(tupleToUse.m_tuple) = &((ComponentWrapper<type>*) componentWrapper)->m_component;

                tupleCreated = true;

                return tupleToUse;
            }
            else
            {
                tupleCreated = false;

                return *tuplesVec.rbegin();
            }
        }

        template<std::size_t I = 0>
        inline typename std::enable_if<I == sizeof...(ComponentsT), bool>::type
        putInTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator,
                   const void* componentWrapper, const size_t& componentTypeID) const noexcept
        { return false; }

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), bool>::type
        putInTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator,
                   const void* componentWrapper, const size_t& componentToPutTypeID) noexcept
        {
            auto& val = std::get<I>(tuple);

            using type = std::remove_reference<decltype(*val)>::type;

            const size_t tupleComponentTypeID = SGUtils::TypeInfoUtils::getTypeInfo<type>().getID();

            if(tupleComponentTypeID == componentToPutTypeID || SGUtils::TypeInfoUtils::getTypeInfo(tupleComponentTypeID).getDerivedTypes().contains(componentToPutTypeID))
            {
                if(!val)
                {
                    val = &((ComponentWrapper<type>*) componentWrapper)->m_component;

                    return true;
                }
            }

            return putInTuple<I + 1>(tuple, registryAllocator, componentWrapper, componentToPutTypeID);
        }

        template<std::size_t I = 0>
        inline typename std::enable_if<I == sizeof...(ComponentsT), bool>::type
        removeFromTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator,
                        const void* component, const size_t& componentTypeID) const noexcept
        { return false; }

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), bool>::type
        removeFromTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator,
                        const void* component, const size_t& componentToPutTypeID) noexcept
        {
            auto& val = std::get<I>(tuple);

            using type = std::remove_reference<decltype(*val)>::type;

            const size_t tupleComponentTypeID = SGUtils::TypeInfoUtils::getTypeInfo<type>().getID();

            if(tupleComponentTypeID == componentToPutTypeID || SGUtils::TypeInfoUtils::getTypeInfo(tupleComponentTypeID).getDerivedTypes().contains(componentToPutTypeID))
            {
                if(val == component)
                {
                    val = nullptr;

                    return true;
                }
            }

            return removeFromTuple<I + 1>(tuple, registryAllocator, component, componentToPutTypeID);
        }

        template<std::size_t I = 0>
        inline typename std::enable_if<I == sizeof...(ComponentsT), void>::type
        destroyTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator) const noexcept
        {}

        // iterating through type to fill tuples
        template<std::size_t I = 0>
        inline typename std::enable_if<I < sizeof...(ComponentsT), void>::type
        destroyTuple(std::tuple<ComponentsT* ...>& tuple, RegistryAllocator& registryAllocator) noexcept
        {
            auto& val = std::get<I>(tuple);

            if(val)
            {
                registryAllocator.deallocate(val);
            }

            destroyTuple<I + 1>(tuple, registryAllocator);
        }

        // TODO: MAKE BETTER
        void onRegistryCreated(RegistryAllocator& registryAllocator)
        {
            m_notFormedTuplesToApply.clear();
            m_formedTuplesToApply.clear();

            // choosing max of components count in all pools
            size_t maxComponentsCountInPools = 0;
            bool anyComponentExists = false;
            SGUtils::Utils::forTypes<ComponentsT...>([&](auto tw)
                                     {
                                         using type = typename decltype(tw)::type;

                                         Pool* pool = registryAllocator.getPoolByType<type>();
                                         if(pool)
                                         {
                                             size_t farthestIndex = pool->getFarthestIndex();
                                             // std::cout << "farthestIndex: " << farthestIndex << std::endl;
                                             if(farthestIndex > maxComponentsCountInPools)
                                             {
                                                 maxComponentsCountInPools = farthestIndex;
                                                 anyComponentExists = true;
                                             }
                                         }
                                     }
            );

            if(anyComponentExists)
            {
                for(size_t i = 0; i < maxComponentsCountInPools; ++i)
                {
                    collectAllComponents(registryAllocator, i);
                }

                typename decltype(m_notFormedTuplesToApply)::iterator tuplesMapIter = m_notFormedTuplesToApply.begin();
                while(tuplesMapIter != m_notFormedTuplesToApply.end())
                {
                    auto& tuplesVec = tuplesMapIter.value();

                    auto tuplesVecIter = tuplesVec.begin();
                    while(tuplesVecIter != tuplesVec.end())
                    {
                        auto& tuple = *tuplesVecIter;

                        checkTupleForFullFormed(tuple);

                        if(tuple.m_isFormed)
                        {
                            m_formedTuplesToApply[tuplesMapIter->first].push_back(tuple);
                            tuplesVecIter = tuplesVec.erase(tuplesVecIter);

                            continue;
                        }

                        ++tuplesVecIter;
                    }

                    ++tuplesMapIter;
                }
            }
        }

        void onEntityRemoved(RegistryAllocator& registryAllocator, const size_t& entity)
        {
            auto& notFormedTuplesVec = m_notFormedTuplesToApply[entity];
            auto& formedTuplesVec = m_formedTuplesToApply[entity];

            auto tupleVecIter = notFormedTuplesVec.begin();
            while(tupleVecIter != notFormedTuplesVec.end())
            {
                auto& tuple = *tupleVecIter;
                destroyTuple(tuple.m_tuple, registryAllocator);

                ++tupleVecIter;
            }

            tupleVecIter = formedTuplesVec.begin();
            while(tupleVecIter != formedTuplesVec.end())
            {
                auto& tuple = *tupleVecIter;
                destroyTuple(tuple.m_tuple, registryAllocator);

                ++tupleVecIter;
            }

            m_notFormedTuplesToApply.erase(entity);
            m_formedTuplesToApply.erase(entity);
        }

        void onComponentCreated
        (RegistryAllocator& registryAllocator, const size_t& entity, const void* componentWrapper, const size_t& componentTypeID)
        {
            auto& notFormedTuplesVec = m_notFormedTuplesToApply[entity];

            auto tupleVecIter = notFormedTuplesVec.begin();
            while(tupleVecIter != notFormedTuplesVec.end())
            {
                auto& tuple = *tupleVecIter;
                if(putInTuple(tuple.m_tuple, registryAllocator, componentWrapper, componentTypeID))
                {
                    checkTupleForFullFormed(tuple);
                    if(tuple.m_isFormed)
                    {
                        m_formedTuplesToApply[entity].push_back(tuple);
                        notFormedTuplesVec.erase(tupleVecIter);
                    }
                    return;
                }

                ++tupleVecIter;
            }

            // if not put in existing tuple then componentWrapper is created in new place where no one componentWrapper was
            // then we just create new tuple

            bool newTupleCreated;

            auto& tupleToUse = createTupleAndPut(registryAllocator, notFormedTuplesVec, entity, componentWrapper,
                                                 componentTypeID, newTupleCreated
            );

            if(newTupleCreated)
            {
                checkTupleForFullFormed(tupleToUse);

                if(tupleToUse.m_isFormed)
                {
                    notFormedTuplesVec.erase(notFormedTuplesVec.end() - 1);
                    m_formedTuplesToApply[entity].push_back(tupleToUse);
                }
            }
        }

        void onComponentRemoved(RegistryAllocator& registryAllocator, const size_t& entity, const void* component,
                                const size_t& componentTypeID)
        {
            auto& notFormedTuplesVec = m_notFormedTuplesToApply[entity];

            auto tupleVecIter = notFormedTuplesVec.begin();
            while(tupleVecIter != notFormedTuplesVec.end())
            {
                auto& tuple = *tupleVecIter;
                if(removeFromTuple(tuple.m_tuple, registryAllocator, component, componentTypeID))
                {
                    checkTupleForFullFormed(tuple);
                    if(tuple.m_elementsFilled == 0)
                    {
                        notFormedTuplesVec.erase(tupleVecIter);

                        if(notFormedTuplesVec.empty())
                        {
                            m_notFormedTuplesToApply.erase(entity);
                        }
                    }

                    return;
                }

                ++tupleVecIter;
            }

            auto& formedTuplesVec = m_formedTuplesToApply[entity];

            tupleVecIter = formedTuplesVec.begin();
            while(tupleVecIter != formedTuplesVec.end())
            {
                auto& tuple = *tupleVecIter;
                if(removeFromTuple(tuple.m_tuple, registryAllocator, component, componentTypeID))
                {
                    checkTupleForFullFormed(tuple);

                    formedTuplesVec.erase(tupleVecIter);

                    if(formedTuplesVec.empty())
                    {
                        m_formedTuplesToApply.erase(entity);
                    }

                    notFormedTuplesVec.push_back(tuple);

                    return;
                }

                ++tupleVecIter;
            }
        }

        std::unique_ptr<SGUtils::EventListenerHolder<void(
                RegistryAllocator&)>> m_onRegistryCreated = std::make_unique<SGUtils::EventListenerHolder<void(
                RegistryAllocator&)>>([this](RegistryAllocator& a)
                                      {
                                          onRegistryCreated(a);
                                      }
        );

        std::unique_ptr<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&)>> m_onEntityRemoved =
                std::make_unique<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&)>>
                        ([this](RegistryAllocator& a, const size_t& entity)
                         {
                             if(m_isCollectingCertainEntities)
                             {
                                 if(m_entitiesToCollect.contains(entity))
                                 {
                                     onEntityRemoved(a, entity);
                                 }
                             }
                             else
                             {
                                 onEntityRemoved(a, entity);
                             }
                         }
                        );

        std::unique_ptr<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&, const void* component,
                                                         const size_t&)>> m_onComponentCreated =
                std::make_unique<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&,
                                                                  const void* component, const size_t&)>>
                        ([this](RegistryAllocator& a, const size_t& entity, const void* componentWrapper,
                                const size_t& componentTypeID)
                         {
                             if(m_isCollectingCertainEntities)
                             {
                                 if(m_entitiesToCollect.contains(entity))
                                 {
                                     onComponentCreated(a, entity, componentWrapper, componentTypeID);
                                 }
                             }
                             else
                             {
                                 onComponentCreated(a, entity, componentWrapper, componentTypeID);
                             }
                         }
                        );

        std::unique_ptr<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&, const void* component,
                                                         const size_t&)>> m_onComponentRemoved =
                std::make_unique<SGUtils::EventListenerHolder<void(RegistryAllocator&, const size_t&,
                                                                  const void* component, const size_t&)>>
                        ([this](RegistryAllocator& a, const size_t& entity, const void* component,
                                const size_t& componentTypeID)
                         {
                             if(m_isCollectingCertainEntities)
                             {
                                 if(m_entitiesToCollect.contains(entity))
                                 {
                                     onComponentRemoved(a, entity, component, componentTypeID);
                                 }
                             }
                             else
                             {
                                 onComponentRemoved(a, entity, component, componentTypeID);
                             }
                         }
                        );

        std::set<size_t> m_entitiesToCollect;
        bool m_isCollectingCertainEntities = false;

        tsl::sparse_map<size_t, std::vector<TupleToApply<ComponentsT...>>> m_formedTuplesToApply;
        tsl::sparse_map<size_t, std::vector<TupleToApply<ComponentsT...>>> m_notFormedTuplesToApply;
        std::tuple<ComponentsT* ...> m_technicalTuple;
    };
}

#endif //ECS_REGISTRYVIEW_H
