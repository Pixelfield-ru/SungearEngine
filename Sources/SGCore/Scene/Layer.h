//
// Created by stuka on 24.10.2023.
//

#pragma once

#include <string>

#include "SGCore/Serde/Defines.h"

// STANDARD LAYERS DEFINES

#define SG_LAYER_OPAQUE_NAME        "Opaque"
#define SG_LAYER_TRANSPARENT_NAME   "Transparent"

sg_predeclare_serde()

namespace SGCore
{
    // todo: del
    struct Layer
    {
        sg_serde_as_friend()

        friend class Scene;
        friend class ISystem;

        std::string m_name;

        bool m_isOpaque = true;

    private:
        size_t m_index = 0;
    };
}
