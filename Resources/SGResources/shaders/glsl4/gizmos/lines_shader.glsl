#sg_pragma once

#sg_include "../uniform_bufs_decl.glsl"

SGSubPass(GeometryPass)
{
    SGSubShader(Vertex)
    {
        uniform vec3 u_offset;
        uniform vec3 u_verticesPositions[2];

        uniform mat4 u_primitiveModelMatrix;

        void main()
        {
            gl_Position = camera.spaceMatrix * u_primitiveModelMatrix * vec4(u_offset + u_verticesPositions[gl_VertexID], 1.0);
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

            fragColor = u_color;
        }
    }
}
