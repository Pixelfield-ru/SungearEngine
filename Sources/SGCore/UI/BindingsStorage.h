//
// Created by stuka on 28.11.2025.
//

#pragma once

#include <unordered_map>
#include <string>
#include <any>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI
{
    struct Binding
    {
        friend struct BindingsStorage;

        template<typename T>
        T* getAs() noexcept
        {
            if(m_binding->type() == typeid(T*))
            {
                return std::any_cast<T*>(*m_binding);
            }

            return nullptr;
        }

        template<typename T>
        const T* getAs() const noexcept
        {
            if(m_binding->type() == typeid(T*))
            {
                return std::any_cast<T*>(*m_binding);
            }

            return nullptr;
        }

    private:
        Ref<std::any> m_binding;
    };

    struct BindingsStorage
    {
        template<typename T>
        void bind(const std::string& key, T* object) noexcept
        {
            if(m_bindingsAccess.contains(key))
            {
                (*m_bindingsAccess[key]) = object;
                return;
            }

            m_bindingsAccess[key] = MakeRef<std::any>(object);
        }

        Binding getBinding(const std::string& key) noexcept
        {
            Binding binding;

            const auto it = m_bindingsAccess.find(key);
            if(it != m_bindingsAccess.end())
            {
                binding.m_binding = it->second;
                return binding;
            }

            const auto newBinding = MakeRef<std::any>();
            m_bindingsAccess[key] = newBinding;

            binding.m_binding = newBinding;

            return binding;
        }

        void clear() noexcept
        {
            m_bindingsAccess.clear();
        }

    private:
        std::unordered_map<std::string, Ref<std::any>> m_bindingsAccess;
    };
}
