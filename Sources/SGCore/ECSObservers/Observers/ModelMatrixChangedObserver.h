//
// Created by stuka on 04.02.2024.
//

#ifndef SUNGEARENGINE_MODELMATRIXCHANGEDOBSERVER_H
#define SUNGEARENGINE_MODELMATRIXCHANGEDOBSERVER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct ModelMatrixChangedObserver : public ISystem
    {
        void onFlagChanged(const Ref<Scene>& scene, const entt::entity& flagOwner, const size_t& flagTypeID) noexcept final;
    };
}

#endif //SUNGEARENGINE_MODELMATRIXCHANGEDOBSERVER_H
