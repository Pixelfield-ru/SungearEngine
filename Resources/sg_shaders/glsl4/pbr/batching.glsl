#sg_pragma once

SGSubPass(BatchingPass)
{
    SGSubShader(Vertex)
    {
        #sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"

        layout (location = 0) in int instanceID;
        layout (location = 1) in vec3 vertexUV;
        layout (location = 2) in vec3 vertexPosition;

        out vec3 vs_UVAttribute;
        flat out int vs_InstanceID;

        uniform samplerBuffer u_matricesTextureBuffer;

        void main()
        {
            vs_UVAttribute = vertexUV;
            vs_InstanceID = instanceID;

            mat4 instanceModelMatrix = mat4(1.0);

            instanceModelMatrix[0] = texelFetch(u_matricesTextureBuffer, instanceID * 4);
            instanceModelMatrix[1] = texelFetch(u_matricesTextureBuffer, instanceID * 4 + 1);
            instanceModelMatrix[2] = texelFetch(u_matricesTextureBuffer, instanceID * 4 + 2);
            instanceModelMatrix[3] = texelFetch(u_matricesTextureBuffer, instanceID * 4 + 3);

            gl_Position = camera.projectionSpaceMatrix * instanceModelMatrix * vec4(vertexPosition, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        in vec3 vs_UVAttribute;
        flat in int vs_InstanceID;

        void main()
        {
            vec4 charCol = vec4(0, 0, 0, 1);

            vec2 finalUV = vs_UVAttribute.xy;
            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vsIn.UV.y;
            #endif

            // fragColor = charCol;
            // fragColor = vec4(vec3(charCol.r) * vs_characterColor.rgb, charCol.r * vs_characterColor.a);
            // fragColor = vec4(vec3(1.0), 1.0);
            fragColor = vec4(vs_UVAttribute, 1.0);
            // fragColor = vec4(vec3(1.0) / (1.0 + float(vs_InstanceID)), 1.0);
        }
    }
}