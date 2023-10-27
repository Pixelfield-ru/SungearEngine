#ifndef SUNGEARENGINE_UTILS_H
#define SUNGEARENGINE_UTILS_H

#define SG_INSTANCEOF(data, type) Core::Utils::Utils::instanceof<type>(data)

#define SG_MAY_NORETURN __declspec(noreturn)

namespace Core::Utils
{
    class Utils
    {
    public:
        // stores type of T
        template<typename T>
        struct TypeWrapper { using type = T; };

        /**
         * Example
         * forTypes<InTypes...>([](auto t) { using type = typename decltype(t)::type; });
         * @tparam InTypes Types through which iteration will occur.
         * @tparam Func Type of function.
         * @param f Function.
         */
        template <typename... InTypes, typename Func>
        static void forTypes(Func&& f)
        {
            (f(TypeWrapper<InTypes>()), ...);
        }

        template<typename Base, typename T>
        static bool instanceof(const T* ptr)
        {
            return dynamic_cast<const Base*>(ptr) != nullptr;
        }
    };
}

#endif //SUNGEARENGINE_UTILS_H
