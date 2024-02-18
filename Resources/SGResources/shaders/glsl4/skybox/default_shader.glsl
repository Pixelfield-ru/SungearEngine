#sg_include "../defines.glsl"
#sg_include "../uniform_bufs_decl.glsl"

SGSubPass(GeometryPass)
{
    SGSubShader(Vertex)
    {
        layout (location = 0) in vec3 positionsAttribute;
        layout (location = 1) in vec3 UVAttribute;
        layout (location = 2) in vec3 normalsAttribute;

        out vec3 vs_UVAttribute;

        void main()
        {
            vs_UVAttribute = positionsAttribute;

            // gl_Position = camera.spaceMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
            gl_Position = camera.projectionMatrix * mat4(mat3(camera.viewMatrix)) * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
            //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
            //gl_Position = vec4(pos.xy, pos.w, pos.w);
        }
    }

    SGSubShader(Fragment)
    {
        #sg_include "atmosphere_scattering.glsl"

        layout(location = 0) out vec4 fragColor;

        /*SGSamplerCube skyboxSamplers[1];
        skyboxSamplers[0] = SGGetTextures("GeniusTexture");*/
        /*SGSamplerCube skyboxSamplers[1];
        skyboxSamplers[0] = SGGetTextures("standard_skybox0");*/
        SGSamplerCube mat_skyboxSamplers[1];
        // skyboxSamplers[0] = SGGetTexturesFromMaterial("SGTT_SKYBOX");

        in vec3 vs_UVAttribute;

        void main()
        {
            vec3 atmosphereCol = atmosphereScattering(
                        vs_UVAttribute,                                     // normalized ray direction
                        atmosphere.rayOrigin,                               // ray origin
                        atmosphere.sunPosition,                             // position of the sun
                        atmosphere.sunIntensity,                            // intensity of the sun
                        atmosphere.planetRadius,                            // radius of the planet in meters
                        atmosphere.atmosphereRadius,                        // radius of the atmosphere in meters
                        atmosphere.rayleighScatteringCoeff,                 // Rayleigh scattering coefficient
                        atmosphere.mieScatteringCoeff,                      // Mie scattering coefficient
                        atmosphere.rayleighScaleHeight,                     // Rayleigh scale height
                        atmosphere.mieScaleHeight,                          // Mie scale height
                        atmosphere.miePreferredScatteringDirection          // Mie preferred scattering direction
            );

            if(mat_skyboxSamplers.sg_length() > 0)
            {
                float mixCoeff = 1.0 / mat_skyboxSamplers.sg_length();
                vec4 skyboxCol = vec4(0.0);
                for (int i = 0; i < mat_skyboxSamplers.sg_length(); i++)
                {
                    skyboxCol += texture(mat_skyboxSamplers[i], vs_UVAttribute.xyz) * mixCoeff;
                }

                fragColor = vec4(atmosphereCol, skyboxCol.a);
            }
            else
            {
                fragColor = vec4(atmosphereCol, 1.0);
            }
        }
    }
}
