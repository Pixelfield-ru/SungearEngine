//
// Created by stuka on 28.12.2024.
//

#ifndef SUNGEARENGINE_COMPONENTTRAITS_H
#define SUNGEARENGINE_COMPONENTTRAITS_H

namespace SGCore
{
    template<typename ComponentRegT>
    struct ComponentTraits
    {
        /**
         * Type of component is registry.
         */
        using reg_t = ComponentRegT;
    };
}

#endif //SUNGEARENGINE_COMPONENTTRAITS_H
