#include "sg_shaders/impl/glsl4/random.glsl"

// returns is fragment needs to be discarded
bool calculateStochasticTransparencyComponents(vec3 inputCol,
                                               float inputAlpha,
                                               out vec4 outputSTColor,
                                               out vec4 outputLayerColor,
                                               vec2 UV,
                                               int isTransparentPass)
{
    if(isTransparentPass == 1)
    {
        float a = inputAlpha;

        float weight = random(UV);
        // float weight = fract(sin(dot(UV, vec2(127.1, 311.7))) * 43758.5453);
        // float weight = random(gl_FragCoord.xy) / 2.0;

        if(weight > a)
        {
            return true;
        }

        outputSTColor = vec4(inputCol.rgb, 1.0);
    }
    else
    {
        outputLayerColor = vec4(inputCol.rgb, 1.0);
    }

    return false;
}