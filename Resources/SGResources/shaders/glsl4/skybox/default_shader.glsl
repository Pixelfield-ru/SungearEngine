#sg_include "../defines.glsl"
#sg_include "../uniform_bufs_decl.glsl"

SGSubPass(PBRFRPGeometryPass)
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

            gl_Position = camera.spaceMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
            // gl_Position = camera.projectionMatrix * mat4(mat3(camera.viewMatrix)) * objectModelMatrix * vec4(positionsAttribute, 1.0);
            //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
            //gl_Position = vec4(pos.xy, pos.w, pos.w);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        const vec3 sunPos = vec3(0.0, 1.0, -1.0);

        /*SGUniformsDeclaration
        {
            SGUSamplerCube skyboxSamplers[1] = SGGetTexturesFromMaterial("SGTT_SKYBOX");
        }*/

        in vec3 vs_UVAttribute;

        void main()
        {




            fragColor = vec4(1.0);
        }
    }
}
