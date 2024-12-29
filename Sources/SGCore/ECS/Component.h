//
// Created by stuka on 28.12.2024.
//

#ifndef SUNGEARENGINE_COMPONENT_H
#define SUNGEARENGINE_COMPONENT_H

namespace SGCore::ECS
{
    template<typename ComponentRegT, typename ConstComponentRegT>
    struct Component
    {
        /**
         * Type of component is registry.
         */
        using reg_t = ComponentRegT;
        /**
         * Cons type of component is registry.
         */
        using const_reg_t = ConstComponentRegT;
    };
}

#endif //SUNGEARENGINE_COMPONENT_H
