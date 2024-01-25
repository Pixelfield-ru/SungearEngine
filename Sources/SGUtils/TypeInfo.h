//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_TYPEMETA_H
#define ECS_TYPEMETA_H

#include <cstddef>
#include <set>
#include <map>

#define SGECS_DECLARE_BASE(DerivedType, BaseType) \
private: \
    static inline const bool sgTypeInfoInit_base_##BaseType = []() { \
                const auto& baseTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<BaseType>(); \
                const auto& derivedTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<DerivedType>(); \
                SGUtils::TypeInfoUtils::inheritAllBases(baseTypeInfo.getID(), derivedTypeInfo.getID()); \
                return true; \
            }(); \

namespace SGUtils
{
    class TypesCounter
    {//
    public:
        static void increase() noexcept
        {
            ++s_typesCount;
        }

        static size_t getComponentsTypesCount() noexcept
        {
            return s_typesCount;
        }

    private:
        static inline size_t s_typesCount = 0;
    };

    template<typename>
    struct TypeID
    {
        TypeID()
        {
            m_id = TypesCounter::getComponentsTypesCount();
            TypesCounter::increase();
        }

        [[nodiscard]] size_t getID() const noexcept
        {
            return m_id;
        }

    private:
        size_t m_id = 0;
    };

    class TypeInfo
    {
        friend struct TypeInfoUtils;

    public:
        TypeInfo() = default;
        TypeInfo(const TypeInfo&) = delete;
        TypeInfo(TypeInfo&&) = delete;

        [[nodiscard]] size_t getID() const noexcept
        {
            return m_id;
        }

        [[nodiscard]] const std::set<size_t>& getDerivedTypes() const noexcept
        {
            return m_derivedTypes;
        }

        [[nodiscard]] const std::set<size_t>& getBaseTypes() const noexcept
        {
            return m_baseTypes;
        }

    private:
        size_t m_id = 0;

        std::set<size_t> m_derivedTypes;
        std::set<size_t> m_baseTypes;
    };

    struct TypeInfoUtils
    {
        template<typename ComponentT>
        static TypeInfo& getTypeInfo()
        {
            static TypeID<ComponentT> s_typeID;

            TypeInfo& typeInfo = m_typeInfos[s_typeID.getID()];
            typeInfo.m_id = s_typeID.getID();

            return typeInfo;
        }

        static TypeInfo& getTypeInfo(const size_t& typeID)
        {
            return m_typeInfos[typeID];
        }

        template<typename BaseT, typename DerivedT>
        static void addBaseAndDerived()
        {
            auto& baseTypeInfo = getTypeInfo<BaseT>();
            auto& derivedTypeInfo = getTypeInfo<DerivedT>();

            baseTypeInfo.m_derivedTypes.insert(derivedTypeInfo.getID());
            derivedTypeInfo.m_baseTypes.insert(baseTypeInfo.getID());
        }

        static void inheritAllBases(const size_t& baseType, const size_t& derivedType)
        {
            auto& baseTypeInfo = getTypeInfo(baseType);
            auto& derivedTypeInfo = getTypeInfo(derivedType);

            baseTypeInfo.m_derivedTypes.insert(derivedType);
            derivedTypeInfo.m_baseTypes.insert(baseType);

            for(auto baseBaseType : baseTypeInfo.getBaseTypes())
            {
                inheritAllBases(baseBaseType, derivedType);
            }
        }

        template<typename BaseT, typename DerivedT>
        static bool isBaseOf()
        {
            return getTypeInfo<BaseT>().m_derivedTypes.contains(getTypeInfo<DerivedT>().getID());
        }

    private:
        static inline std::map<size_t, TypeInfo> m_typeInfos;
    };
}

#endif //ECS_TYPEMETA_H
