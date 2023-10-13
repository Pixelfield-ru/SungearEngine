#ifndef SUNGEARENGINE_UTILS_H
#define SUNGEARENGINE_UTILS_H

#define SG_INSTANCEOF(data, type) Core::Utils::Utils::instanceof<type>(data)

namespace Core::Utils
{
    class Utils
    {
    public:
        template<typename Base, typename T>
        static bool instanceof(const T* ptr)
        {
            return dynamic_cast<const Base*>(ptr) != nullptr;
        }
    };
}

#endif //SUNGEARENGINE_UTILS_H
