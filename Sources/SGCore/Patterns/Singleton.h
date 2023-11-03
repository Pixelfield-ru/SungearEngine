//
// Created by stuka on 21.07.2023.
//

#ifndef SUNGEARENGINE_SINGLETON_H
#define SUNGEARENGINE_SINGLETON_H

#include <memory>

#define SG_DECLARE_SINGLETON(cls)                   \
        friend struct Core::Patterns::Singleton;    \
    protected:                                      \
        cls() = default;                            \
        cls(const cls&) = default;                  \
        cls(cls&&) = default;

#define SG_DECLARE_COPY_MOVE_SINGLETON(cls)             \
        friend struct Core::Patterns::Singleton;        \
    protected:                                          \
        cls(const cls&) = default;                      \
        cls(cls&&) = default;

namespace Core::Patterns
{
    struct Singleton
    {
        template<class T>
        static std::shared_ptr<T> getInstance()
        {
            static std::shared_ptr<T> s_instance(new T);
            return s_instance;
        }
    };
}

#endif //SUNGEARENGINE_SINGLETON_H
