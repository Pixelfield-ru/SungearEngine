#sg_pragma once

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

            gl_Position = camera.spaceMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        in vec3 vs_UVAttribute;

        SGSampler2D mat_diffuseSamplers[1];

        void main()
        {
            vec4 diffuseColor = vec4(0, 0, 0, 1);

            vec2 finalUV = vs_UVAttribute.xy;
            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vsIn.UV.y;
            #endif

            if(mat_diffuseSamplers.sg_length() > 0)
            {
                float mixCoeff = 1.0 / mat_diffuseSamplers.sg_length();

                diffuseColor.rgba = vec4(0);

                for (int i = 0; i < mat_diffuseSamplers.sg_length(); ++i)
                {
                    diffuseColor += texture(mat_diffuseSamplers[i], finalUV) * mixCoeff;
                }
            }

            fragColor = vec4(diffuseColor.r, diffuseColor.r, diffuseColor.r, 1.0);
        }
    }
}