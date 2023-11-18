//
// Created by stuka on 06.07.2023.
//

#ifndef SUNGEARENGINE_APITYPE_H
#define SUNGEARENGINE_APITYPE_H

namespace Core::Graphics
{
    typedef char GAPIType;

    static const GAPIType SG_API_TYPE_UNKNOWN = 0;

    static const GAPIType SG_API_TYPE_GL4 = 1;
    static const GAPIType SG_API_TYPE_GL46 = 2;

    static const GAPIType SG_API_TYPE_GLES2 = 3;
    static const GAPIType SG_API_TYPE_GLES3 = 4;

    static const GAPIType SG_API_TYPE_VULKAN = 5;
}

#endif //SUNGEARENGINE_APITYPE_H
