#sg_pragma once

#sg_include "sg_shaders/glsl4/defines.glsl"
#sg_include "sg_shaders/glsl4/uniform_bufs_decl.glsl"

SGSubPass(GeometryPass)
{
    SGSubShader(Vertex)
    {
        #extension GL_KHR_vulkan_glsl : enable

        layout (location = 0) in vec3 positionsAttribute;
        layout (location = 1) in vec3 UVAttribute;

        out vec3 nearPoint;
        out vec3 farPoint;

        out vec3 vs_UVAttribute;

        // Grid position are in clipped space
        vec3 gridPlane[6] = vec3[] (
            vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
            vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
        );

        vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection)
        {
            mat4 viewInv = inverse(view);
            mat4 projInv = inverse(projection);
            vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
            return unprojectedPoint.xyz / unprojectedPoint.w;
        }

        void main()
        {
            vs_UVAttribute = UVAttribute;

            vec3 p = gridPlane[gl_VertexIndex].xyz;
            // vec3 p = positionsAttribute;
            nearPoint = UnprojectPoint(p.x, p.y, 0.0, camera.viewMatrix, camera.projectionMatrix).xyz;
            farPoint = UnprojectPoint(p.x, p.y, 1.0, camera.viewMatrix, camera.projectionMatrix).xyz;
            // gl_Position = vec4(p, 1.0);

            gl_Position = camera.projectionMatrix * camera.viewMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        in vec3 nearPoint;
        in vec3 farPoint;

        in vec3 vs_UVAttribute;

        const float linesStep = 0.1;
        const float linesWidth = 0.05;

        void main()
        {
            float t = -nearPoint.y / (farPoint.y - nearPoint.y);
            // fragColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0));

            // fragColor = vec4(1.0);
            // discard;

            float xFract = fract(vs_UVAttribute.x * 100.0 / linesStep);
            float yFract = fract(vs_UVAttribute.y * 100.0 / linesStep);

            if(xFract < linesWidth || yFract < linesWidth)
            {
                fragColor = vec4(1.0);
            }
            else
            {
                discard;
            }
        }
    }
}
