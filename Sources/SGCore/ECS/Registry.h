//
// Created by stuka on 28.12.2024.
//

#ifndef SUNGEARENGINE_ECSREGISTRY_H
#define SUNGEARENGINE_ECSREGISTRY_H

#include <entt/entity/registry.hpp>
#include <entt/fwd.hpp>

#include "SGCore/Scene/EntityBaseInfo.h"

namespace SGCore::ECS
{
    // struct (instead of direct usage of entt::exclude_t) to avoiding compile-time error
    template<typename... Exclude>
    struct ExcludeTypes
    {
        using exclude_t = entt::exclude_t<typename Exclude::reg_t...>;
    };

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
            emplace<EntityBaseInfo>(entity, entity);

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
            return m_registry.template emplace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
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
            return m_registry.template emplace_or_replace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
        }

        template<typename Type, typename... Func>
        decltype(auto) patch(const EntityT& entt, Func&&...func) noexcept
        {
            return m_registry.template patch<typename Type::reg_t>(entt, std::forward<Func>(func)...);
        }

        template<typename Type, typename... Args>
        decltype(auto) replace(const EntityT& entt, Args&&...args) noexcept
        {
            return m_registry.template replace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
        }

        template<typename Type, typename... Other>
        size_type remove(const EntityT& entt) noexcept
        {
            return m_registry.template remove<typename Type::reg_t, typename Other::reg_t...>(entt);
        }

        template<typename Type, typename... Other, typename It>
        size_type remove(It first, It last) noexcept
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
            return m_registry.template get_or_emplace<typename Type::reg_t>(entt, std::forward<Args>(args)...);
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

    private:
        entt_reg_t m_registry;
    };
}

#endif //SUNGEARENGINE_ECSREGISTRY_H
