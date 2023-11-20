//
// Created by stuka on 06.07.2023.
//

#ifndef SUNGEARENGINE_APITYPE_H
#define SUNGEARENGINE_APITYPE_H

namespace Core::Graphics
{
    enum GAPIType
    {
        SG_API_TYPE_UNKNOWN,

        SG_API_TYPE_GL4,
        SG_API_TYPE_GL46,

        SG_API_TYPE_GLES2,
        SG_API_TYPE_GLES3,

        SG_API_TYPE_VULKAN
    };
}

#endif //SUNGEARENGINE_APITYPE_H
