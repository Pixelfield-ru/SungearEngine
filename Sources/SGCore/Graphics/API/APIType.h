//
// Created by stuka on 06.07.2023.
//

#ifndef SUNGEARENGINE_APITYPE_H
#define SUNGEARENGINE_APITYPE_H

namespace Core::Graphics
{
    enum APIType
    {
        OPENGL,
        OPENGLES,
        VULKAN,
        DIRECTX,
        METAL,
        UNKNOWN
    };
}

#endif //SUNGEARENGINE_APITYPE_H
