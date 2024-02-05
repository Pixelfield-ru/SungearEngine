#sg_include "../defines.glsl"
#sg_include "../uniform_bufs_decl.glsl"
#sg_include "atmosphere_scattering.glsl"

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
            gl_Position = camera.projectionMatrix * mat4(mat3(camera.viewMatrix)) * objectModelMatrix * vec4(positionsAttribute, 1.0);
            //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
            //gl_Position = vec4(pos.xy, pos.w, pos.w);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        const vec3 sunPos = vec3(0.0, 1.0, -1.0);

        SGUniformsDeclaration
        {
            // SGUSamplerCube skyboxSamplers[1] = SGGetTextures("GeniusTexture");
            // SGUSamplerCube skyboxSamplers[1] = SGGetTextures("standard_skybox0");
            SGUSamplerCube skyboxSamplers[1] = SGGetTexturesFromMaterial("SGTT_SKYBOX");
        }

        in vec3 vs_UVAttribute;

        void main()
        {
            vec3 atmosphereCol = atmosphere(
                        vs_UVAttribute,           // normalized ray direction
                        vec3(0, 6372e3, 0),               // ray origin
                        sunPos,                        // position of the sun
                        30.0,                           // intensity of the sun
                        6371e3,                         // radius of the planet in meters
                        6471e3,                         // radius of the atmosphere in meters
                        vec3(14.5e-6, 15.0e-6, 17.4e-6), // Rayleigh scattering coefficient
                        20e-6,                          // Mie scattering coefficient
                        8e3,                            // Rayleigh scale height
                        1.0e3,                          // Mie scale height
                        0.958                          // Mie preferred scattering direction
            );

            if(skyboxSamplers.length() > 0)
            {
                float mixCoeff = 1.0 / skyboxSamplers.length();
                vec4 skyboxCol = vec4(0.0);
                for (int i = 0; i < skyboxSamplers.length(); i++)
                {
                    skyboxCol += texture(skyboxSamplers[i], vs_UVAttribute.xyz) * mixCoeff;
                }

                fragColor = vec4(atmosphereCol * skyboxCol.rgb, skyboxCol.a);
            }
            else
            {
                fragColor = vec4(atmosphereCol, 1.0);
            }
        }
    }
}
