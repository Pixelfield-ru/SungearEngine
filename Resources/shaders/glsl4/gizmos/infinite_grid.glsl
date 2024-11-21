#sg_pragma once

#sg_include "../defines.glsl"
#sg_include "../uniform_bufs_decl.glsl"

SGSubPass(GeometryPass)
{
    SGSubShader(Vertex)
    {
        #extension GL_KHR_vulkan_glsl : enable

        layout (location = 0) in vec3 positionsAttribute;

        out vec3 nearPoint;
        out vec3 farPoint;

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
            vec3 p = gridPlane[gl_VertexIndex].xyz;
            // vec3 p = positionsAttribute;
            nearPoint = UnprojectPoint(p.x, p.y, 0.0, camera.viewMatrix, camera.projectionMatrix).xyz;
            farPoint = UnprojectPoint(p.x, p.y, 1.0, camera.viewMatrix, camera.projectionMatrix).xyz;
            gl_Position = vec4(p, 1.0);

            // gl_Position = camera.projectionMatrix * camera.viewMatrix * objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        in vec3 nearPoint;
        in vec3 farPoint;

        void main()
        {
            float t = -nearPoint.y / (farPoint.y - nearPoint.y);
            fragColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0));
        }
    }
}
