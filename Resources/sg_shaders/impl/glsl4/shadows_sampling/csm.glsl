uniform mat4 CSMLightSpaceMatrix[6];
uniform float CSMCascadesPlanesDistances[6];
uniform float CSMCascadesBiases[6];
uniform sampler2D CSMShadowMaps[6];
uniform int CSMCascadesCount;

#include "pcf.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"

float getCSMShadow(vec3 lightDir, vec3 fragPos)
{
    if(CSMCascadesCount == 0) return 1.0;

    // select cascade layer
    vec4 fragPosViewSpace = camera.viewMatrix * vec4(fragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for(int i = 0; i < CSMCascadesCount; ++i)
    {
        if (depthValue < CSMCascadesPlanesDistances[i])
        {
            layer = i;
            break;
        }
    }

    if(layer == -1)
    {
        layer = CSMCascadesCount;
    }

    vec4 fragPosLightSpace = CSMLightSpaceMatrix[layer] * vec4(fragPos, 1.0);

    // ===========================================

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    // const float biases[6] = float[6] ( 0.0012, 0.0012, 0.0012, 0.0012, 0.0012, 0.0012 );
    float bias = CSMCascadesBiases[layer];
    // float bias = biases[layer];
    if (layer == CSMCascadesCount)
    {
        bias *= 1 / (camera.zFar * 0.5f);
    }
    else
    {
        bias *= 1 / (CSMCascadesPlanesDistances[layer] * 0.5f);
    }

    // ====================================== filtering algo

    // float shadow = PCSS(CSMShadowMaps[layer], projCoords, bias);
    vec2 shadowMapSize = textureSize(CSMShadowMaps[layer], 0).xy;
    float shadow = ShadowMapPCF(CSMShadowMaps[layer], projCoords, shadowMapSize, 1.0 / shadowMapSize, 1, bias);

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}