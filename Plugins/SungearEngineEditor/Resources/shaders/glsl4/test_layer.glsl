#sg_pragma once

#sg_include "sg_shaders/glsl4/postprocessing/layered/base.glsl"
#sg_include "sg_shaders/glsl4/disks.glsl"
#sg_include "sg_shaders/glsl4/math.glsl"
#sg_include "sg_shaders/glsl4/random.glsl"
#sg_include "sg_shaders/glsl4/postprocessing/layered/utils.glsl"

SGSubPass(SGPPLayerFXPass)
{
    SGSubShader(Fragment)
    {
        in vec2 vs_UVAttribute;

        const int samplesCount = 6;

        uniform sampler2D SGPP_LayersVolumes;
        uniform sampler2D SGPP_LayersColors;
        uniform int SGPP_CurrentLayerIndex;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            float rand = random(finalUV);
            float rotAngle = rand * PI;
            vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

            vec3 layerCol = texture(SGPP_LayersColors, finalUV).rgb;
            vec3 bloomCol = vec3(0.0);

            int successfulSamplesCount = 1;
            for(int i = 0; i < samplesCount; i++)
            {
                vec2 uvOffset = finalUV + rotate(poissonDisk[i], rotTrig) / 50.0;
                vec4 layerVolume = texture(SGPP_LayersVolumes, uvOffset);
                if(layerVolume == calculatePPLayerVolume(SGPP_CurrentLayerIndex))
                {
                    bloomCol += texture(SGPP_LayersColors, uvOffset).rgb * 5.0;
                    ++successfulSamplesCount;
                }
            }

            if(successfulSamplesCount > 2)
            {
                --successfulSamplesCount;
            }

            bloomCol.rgb /= float(successfulSamplesCount);

            layerCol += bloomCol;

            // just do nothing
            if(texture(SGPP_LayersVolumes, finalUV).rgb == calculatePPLayerVolume(SGPP_CurrentLayerIndex).rgb)
            // if()
            {
                gl_FragColor = texture(SGPP_LayersColors, finalUV);
            }
            // gl_FragColor = vec4(layerCol, 1.0);
        }
    }
}