#sg_pragma once

#sg_include "sg_shaders/glsl4/postprocessing/layered/base.glsl"
#sg_include "sg_shaders/glsl4/disks.glsl"
#sg_include "sg_shaders/glsl4/math.glsl"
#sg_include "sg_shaders/glsl4/random.glsl"

SGSubPass(SGLPPLayerFXPass)
{
    SGSubShader(Fragment)
    {
        uniform sampler2D currentLayer;

        in vec2 vs_UVAttribute;

        const int samplesCount = 6;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            float rand = random(finalUV);
            float rotAngle = rand * PI;
            vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

            vec3 finalCol = vec3(0.0);

            for(int i = 0; i < samplesCount; i++)
            {
                finalCol += texture(currentLayer, finalUV + rotate(poissonDisk[i], rotTrig) / 150.0).rgb;
            }

            finalCol /= float(samplesCount);

            // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            gl_FragColor = vec4(texture(currentLayer, finalUV).rgba);
            // gl_FragColor = vec4(finalCol, 1.0);
        }
    }
}