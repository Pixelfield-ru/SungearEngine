//
// Created by stuka on 21.07.2023.
//

#ifndef SUNGEARENGINE_SINGLETON_H
#define SUNGEARENGINE_SINGLETON_H

namespace Core::Patterns
{
    template<typename T>
    class Singleton
    {
    private:

    public:
        static T* getInstance()
        {
            static T* s_instance = new T;
            return s_instance;
        }
    };
}

#endif //SUNGEARENGINE_SINGLETON_H
