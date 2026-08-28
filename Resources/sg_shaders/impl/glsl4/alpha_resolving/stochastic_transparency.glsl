#include "sg_shaders/impl/glsl4/random.glsl"

// returns is fragment needs to be discarded
bool calculateStochasticTransparencyComponents(vec3 inputCol,
                                               float inputAlpha,
                                               out vec4 outputSTColor,
                                               out vec4 outputLayerColor,
                                               vec2 UV,
                                               int isTransparentPass)
{
    // Both outputs must be assigned on every path. The callers do not discard (their discard is
    // commented out), and a fragment output left unassigned is undefined: GL happened to leave the
    // attachment alone, while Vulkan wrote the fragment colour into it — the transparent objects
    // leaked into the opaque layer, and the whole stochastic layer was filled with opaque geometry,
    // which the resolve then turned into noise. Neither backend was wrong; the shader was.
    outputSTColor = vec4(0.0);
    outputLayerColor = vec4(inputCol.rgb, 1.0);

    if(isTransparentPass == 1)
    {
        float a = inputAlpha;

        float weight = random(UV);
        // float weight = fract(sin(dot(UV, vec2(127.1, 311.7))) * 43758.5453);
        // float weight = random(gl_FragCoord.xy) / 2.0;

        // the colour of a transparent fragment belongs to the stochastic layer only: alpha 0 leaves
        // the opaque layer untouched under this pass's SrcAlpha blending
        outputLayerColor = vec4(inputCol.rgb, 0.0);

        if(weight > a)
        {
            return true;
        }

        outputSTColor = vec4(inputCol.rgb, a);
    }

    return false;
}