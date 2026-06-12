//
// Created by stuka on 28.12.2024.
//

#pragma once

#include <entt/entity/registry.hpp>
#include <iostream>

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Utils/Slot.h"
#include "SGCore/Utils/Signal.h"

namespace SGCore::ECS
{
    // struct (instead of direct usage of entt::exclude_t) to avoiding compile-time error
    template<typename... Exclude>
    struct ExcludeTypes
    {
        using exclude_t = entt::exclude_t<typename Exclude::reg_t...>;
    };

    // Signal<void(&)>

    /**
     *
     * @tparam EntityT
     */
    template<typename EntityT>
    struct Registry
    {
        using entity_t = EntityT;
        using entt_reg_t = typename entt::basic_registry<EntityT>;
        using version_type = typename entt_reg_t::version_type;
        template<typename T>
        using storage_for_type = typename entt_reg_t::template storage_for_type<T>;

        template<typename>
        friend struct SingletonComponent;

        // todo: сделать конструктор, в котором будет автоматическая подписка на ивент создания и удаления синглтон компонентов
        // todo: сделать также copy, move операторы и copy, move конструкторы

        Registry()
        {
            subscribeToAllSingletonsEvents();

            getNewSingletonTypeSignal() += onNewSingletonType;
        }

        Registry(const Registry& other) noexcept = delete;
        Registry(Registry&& other) noexcept
        {
            m_registry = std::move(other.m_registry);
            m_singletonsStorage = std::move(other.m_singletonsStorage);

            subscribeToAllSingletonsEvents();

            getNewSingletonTypeSignal() += onNewSingletonType;
        }

        [[nodiscard]] bool valid(const EntityT& entt) const noexcept
        {
            return m_registry.valid(entt);
        }

        /**
         * Creates entity with standard component EntityBaseInfo.
         * @return New entity.
         */
        [[nodiscard]] EntityT create()
        {
            auto entity = m_registry.create();
            emplace<EntityBaseInfo>(entity, entity, *this);

            return entity;
        }

        /**
         * Creates entity with standard component EntityBaseInfo.
         * @return New entity.
         */
        [[nodiscard]] EntityT create(const EntityT& hint)
        {
            auto entity = m_registry.create(hint);
            emplace<EntityBaseInfo>(entity, entity);

            return entity;
        }

        /*template<typename It>
        void create(It first, It last)
        {
            m_registry.create(first, last);
        }*/

        version_type destroy(const EntityT& entt) noexcept
        {
            return m_registry.destroy(entt);
        }

        version_type destroy(const EntityT& entt, const version_type& version) noexcept
        {
            return m_registry.destroy(entt, version);
        }

        template<typename It>
        void destroy(It first, It last) noexcept
        {
            m_registry.destroy(first, last);
        }

        template<typename Type, typename... Args>
        decltype(auto) emplace(const EntityT& entt, Args&&...args) noexcept
        {
            // msvc workaround
            using accessor_t = Type::accessor_t;

            auto&& component = m_registry.template emplace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
            accessor_t{}(component).m_thisEntity = entt;

            return component;
        }

        template<typename Type, typename It>
        void insert(It first, It last, const Type& value = {}) noexcept
        {
            m_registry.insert(first, last, value);
        }

        template<typename Type, typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<CIt>::value_type, Type>>>
        void insert(EIt first, EIt last, CIt from) noexcept
        {
            m_registry.template insert<typename Type::reg_t>(first, last, from);
        }

        template<typename Type, typename... Args>
        decltype(auto) emplaceOrReplace(const EntityT& entt, Args&&...args) noexcept
        {
            // msvc workaround
            using accessor_t = Type::accessor_t;

            auto&& component = m_registry.template emplace_or_replace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
            accessor_t{}(component).m_thisEntity = entt;

            return component;
        }

        template<typename Type, typename... Func>
        decltype(auto) patch(const EntityT& entt, Func&&...func) noexcept
        {
            return m_registry.template patch<typename Type::reg_t>(entt, std::forward<Func>(func)...);
        }

        template<typename Type, typename... Args>
        decltype(auto) replace(const EntityT& entt, Args&&...args) noexcept
        {
            // msvc workaround
            using accessor_t = Type::accessor_t;

            auto&& component = m_registry.template replace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
            accessor_t{}(component).m_thisEntity = entt;

            return component;
        }

        template<typename Type, typename... Other>
        size_t remove(const EntityT& entt) noexcept
        {
            return m_registry.template remove<typename Type::reg_t, typename Other::reg_t...>(entt);
        }

        template<typename Type, typename... Other, typename It>
        size_t remove(It first, It last) noexcept
        {
            return m_registry.template remove<typename Type::reg_t, typename Other::reg_t...>(first, last);
        }

        template<typename Type, typename... Other>
        void erase(const EntityT& entt) noexcept
        {
            m_registry.template erase<typename Type::reg_t, typename Other::reg_t...>(entt);
        }

        template<typename Type, typename... Other, typename It>
        void erase(It first, It last) noexcept
        {
            m_registry.template erase<typename Type::reg_t, typename Other::reg_t...>(first, last);
        }

        template<typename Func>
        void eraseIf(const EntityT& entt, Func func)
        {
            m_registry.erase_if(entt, func);
        }

        template<typename... Type>
        void compact() noexcept
        {
            m_registry.template compact<typename Type::reg_t...>();
        }

        template<typename... Type>
        [[nodiscard]] bool allOf([[maybe_unused]] const EntityT& entt) const noexcept
        {
            return m_registry.template all_of<typename Type::reg_t...>(entt);
        }

        template<typename... Type>
        [[nodiscard]] bool anyOf([[maybe_unused]] const EntityT& entt) const noexcept
        {
            return m_registry.template any_of<typename Type::reg_t...>(entt);
        }

        template<typename... Type>
        [[nodiscard]] decltype(auto) get([[maybe_unused]] const EntityT& entt) const noexcept
        {
            return m_registry.template get<typename Type::reg_t...>(entt);
        }

        template<typename... Type>
        [[nodiscard]] decltype(auto) get([[maybe_unused]] const EntityT& entt) noexcept
        {
            return m_registry.template get<typename Type::reg_t...>(entt);
        }

        template<typename Type, typename... Args>
        [[nodiscard]] decltype(auto) getOrEmplace(const EntityT& entt, Args&&...args) noexcept
        {
            // msvc workaround
            using accessor_t = Type::accessor_t;

            auto&& component = m_registry.template get_or_emplace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
            accessor_t{}(component).m_thisEntity = entt;

            return component;
        }

        template<typename... Type>
        [[nodiscard]] auto tryGet([[maybe_unused]] const EntityT& entt) const noexcept
        {
            return m_registry.template try_get<typename Type::reg_t...>(entt);
        }

        template<typename... Type>
        [[nodiscard]] auto tryGet([[maybe_unused]] const EntityT& entt) noexcept
        {
            return m_registry.template try_get<typename Type::reg_t...>(entt);
        }

        template<typename... Type>
        void clear() noexcept
        {
            m_registry.template clear<typename Type::reg_t...>();
        }

        template<typename Type>
        [[nodiscard]] auto onConstruct(const entt::id_type id = entt::type_hash<typename Type::reg_t>::value()) noexcept
        {
            return m_registry.template on_construct<typename Type::reg_t>(id);
        }

        template<typename Type>
        [[nodiscard]] auto onUpdate(const entt::id_type id = entt::type_hash<typename Type::reg_t>::value()) noexcept
        {
            return m_registry.template on_update<typename Type::reg_t>(id);
        }

        template<typename Type>
        [[nodiscard]] auto onDestroy(const entt::id_type id = entt::type_hash<typename Type::reg_t>::value()) noexcept
        {
            return m_registry.template on_destroy<typename Type::reg_t>(id);
        }

        template<typename Type, typename... Other, typename... Exclude>
        [[nodiscard]] entt::basic_view<entt::get_t<storage_for_type<const typename Type::reg_t>,
                storage_for_type<const typename Other::reg_t>...>,
                entt::exclude_t<storage_for_type<const typename Exclude::reg_t>...>>
        view(ExcludeTypes<Exclude...> exc = ExcludeTypes {}) const noexcept
        {
            return m_registry.template view<typename Type::reg_t, typename Other::reg_t...>(typename decltype(exc)::exclude_t { });
        }

        template<typename Type, typename... Other, typename... Exclude>
        [[nodiscard]] entt::basic_view<entt::get_t<storage_for_type<typename Type::reg_t>,
                storage_for_type<typename Other::reg_t>...>,
                entt::exclude_t<storage_for_type<typename Exclude::reg_t>...>>
        view(ExcludeTypes<Exclude...> exc = ExcludeTypes {}) noexcept
        {
            return m_registry.template view<typename Type::reg_t, typename Other::reg_t...>(typename decltype(exc)::exclude_t { });
        }

        template<typename SingletonT>
        EntityT getEntityOfSingleton() const noexcept
        {
            const auto entitiesIt = m_singletonsStorage.find(SingletonT::getTypeIDStatic());
            if(entitiesIt == m_singletonsStorage.end()) return entt::null;

            return entitiesIt->second;
        }

        template<typename SingletonT>
        const SingletonT* tryGetSingleton() const noexcept
        {
            const auto singletonEntity = getEntityOfSingleton<SingletonT>();
            if(singletonEntity == entt::null) return {};

            return m_registry.template get<SingletonT>(singletonEntity);
        }

        template<typename SingletonT>
        SingletonT* tryGetSingleton() noexcept
        {
            const auto singletonEntity = getEntityOfSingleton<SingletonT>();
            if(singletonEntity == entt::null) return {};

            return m_registry.template get<SingletonT>(singletonEntity);
        }

        Registry& operator=(const Registry& other) noexcept = delete;
        Registry& operator=(Registry&& other) noexcept
        {
            if(std::addressof(other) == this) return *this;

            m_registry = std::move(other.m_registry);
            m_singletonsStorage = std::move(other.m_singletonsStorage);

            subscribeToAllSingletonsEvents();

            // getNewSingletonTypeSignal() += onNewSingletonType;

            return *this;
        }

    private:
        entt_reg_t m_registry;

        std::unordered_map<size_t, EntityT> m_singletonsStorage;

        Slot<void(const std::function<void(Registry&)>&)> onNewSingletonType = [this](const std::function<void(Registry&)>& subscribeFunc) {
            subscribeToSingletonEvents(subscribeFunc);
        };

        template<typename SingletonT>
        void singletonConstructObserver(entt_reg_t& enttRegistry, EntityT entity) noexcept
        {
            if(!valid(entity)) return;

            // removing component from old valid entity
            const auto entitiesIt = m_singletonsStorage.find(SingletonT::getTypeIDStatic());
            if(entitiesIt != m_singletonsStorage.end())
            {
                if(valid(entitiesIt->second))
                {
                    remove<SingletonT>(entitiesIt->second);
                }
            }

            m_singletonsStorage[SingletonT::getTypeIDStatic()] = entity;
        }

        template<typename SingletonT>
        void singletonDestroyObserver(entt_reg_t& enttRegistry, EntityT entity) noexcept
        {
            if(!valid(entity)) return;

            m_singletonsStorage[SingletonT::getTypeIDStatic()] = entt::null;
        }

        void subscribeToSingletonEvents(const std::function<void(Registry&)>& subscribeFunc) noexcept
        {
            // subscribing to construct & destroy event of singleton component of type
            subscribeFunc(*this);
        }

        void subscribeToAllSingletonsEvents() noexcept
        {
            // subscribing to construct & destroy event of all singletons components
            for(const auto& subscribeFunc : getSingletonLifecycleSubscribers())
            {
                subscribeFunc(*this);
            }
        }

        static SGCORE_EXPORT Signal<void(const std::function<void(Registry&)>&)>& getNewSingletonTypeSignal() noexcept
        {
            static Signal<void(const std::function<void(Registry&)>&)> newSingletonTypeSignal;
            return newSingletonTypeSignal;
        }

        static SGCORE_EXPORT std::vector<std::function<void(Registry&)>>& getSingletonLifecycleSubscribers() noexcept
        {
            static std::vector<std::function<void(Registry&)>> singletonLifecycleCallbacks;
            return singletonLifecycleCallbacks;
        }

        template<typename SingletonT>
        static void registerSingleton() noexcept
        {
            const auto subscribeFunc = [](Registry& registry) {
                registry.onConstruct<SingletonT>().template connect<&Registry::singletonConstructObserver<SingletonT>>(registry);
                registry.onDestroy<SingletonT>().template connect<&Registry::singletonDestroyObserver<SingletonT>>(registry);
            };

            getSingletonLifecycleSubscribers().push_back(subscribeFunc);

            getNewSingletonTypeSignal()(subscribeFunc);
        }
    };
}
