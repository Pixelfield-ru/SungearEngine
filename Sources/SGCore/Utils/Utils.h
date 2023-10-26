#ifndef SUNGEARENGINE_UTILS_H
#define SUNGEARENGINE_UTILS_H

#define SG_INSTANCEOF(data, type) Core::Utils::Utils::instanceof<type>(data)

#define SG_MAY_NORETURN __declspec(noreturn)

#define SG_DECL_CONFIRMATION_BOOL(confirmationsReq, boolDefVal, boolName)                                               \
                public:                                                                                                 \
                    static inline size_t s_ ## boolName ## ConfirmationsRequires = confirmationsReq;                    \
                    bool m_ ## boolName ## NeedsConfirmation = false;                                                   \
                                                                                                                        \
                    void confirm ## boolName()                                                                          \
                    {                                                                                                   \
                        if(m_ ## boolName ## NeedsConfirmation &&                                                       \
                           m_ ## boolName ## CurrentConfirmations < s_ ## boolName ## ConfirmationsRequires)            \
                        {                                                                                               \
                            m_ ## boolName ## CurrentConfirmations++;                                                   \
                            if(m_ ## boolName ## CurrentConfirmations >= s_ ## boolName ## ConfirmationsRequires)       \
                            {                                                                                           \
                                  m_ ## boolName = !boolDefVal;                                                         \
                            }                                                                                           \
                        }                                                                                               \
                    }                                                                                                   \
                                                                                                                        \
                    size_t get ## boolName() { return m_ ## boolName ## CurrentConfirmations; }                         \
                    bool is ## boolName ## Confirmed() { return m_ ## boolName != boolDefVal; }                         \
                    void reset ## boolName ## ForConfirmation()                                                         \
                    {                                                                                                   \
                        m_ ## boolName = boolDefVal;                                                                    \
                        m_ ## boolName ## CurrentConfirmations = 0;                                                     \
                    }                                                                                                   \
                                                                                                                        \
                private:                                                                                                \
                    size_t m_ ## boolName ## CurrentConfirmations = 0;                                                  \
                    bool m_ ## boolName = boolDefVal;                                                                   \

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
