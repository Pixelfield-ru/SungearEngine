#sg_pragma once

#sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"

SGSubPass(GeometryPass)
{
    SGSubShader(Vertex)
    {
        layout (location = 0) in vec3 positionsAttribute;

        uniform vec3 u_offset;

        uniform mat4 u_primitiveModelMatrix;

        void main()
        {
            gl_Position = camera.projectionSpaceMatrix * u_primitiveModelMatrix * vec4(u_offset + positionsAttribute, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;
        layout(location = 1) out vec3 gFragPos;
        layout(location = 2) out vec3 gNormal;
        layout(location = 3) out vec4 gAlbedoSpec;
        layout(location = 4) out vec3 unknownAttachment0;
        layout(location = 5) out vec3 unknownAttachment1;

        uniform vec4 u_color;

        void main()
        {
            gFragPos = vec3(1.0);
            gNormal = vec3(1.0);
            gAlbedoSpec = vec4(1.0);
            unknownAttachment0 = vec3(1.0);
            unknownAttachment1 = vec3(1.0);

            fragColor = vec4(u_color.rgb, u_color.a);
        }
    }
}