#sg_pragma once

SGSubPass(TextRenderPass)
{
    SGSubShader(Vertex)
    {
        layout (location = 0) in mat4 characterModelMatrix;
        layout (location = 4) in vec4 characterColor;
        layout (location = 5) in vec2 characterUV[4];
        layout (location = 9) in vec3 characterVertexPosition;

        out vec2 vs_UVAttribute;
        out vec4 vs_characterColor;

        void main()
        {
            vs_UVAttribute = characterUV[gl_VertexID];
            vs_characterColor = characterColor;

            gl_Position = characterModelMatrix * vec4(characterVertexPosition, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        layout(location = 0) out vec4 fragColor;

        in vec2 vs_UVAttribute;
        in vec4 vs_characterColor;

        uniform sampler2D u_fontSpecializationAtlas;

        void main()
        {
            vec4 charCol = vec4(0, 0, 0, 1);

            vec2 finalUV = vs_UVAttribute.xy;
            #ifdef FLIP_TEXTURES_Y
                finalUV.y = 1.0 - vsIn.UV.y;
            #endif

            charCol = texture(u_fontSpecializationAtlas, finalUV);

            // fragColor = charCol;
            fragColor = vec4(vec3(charCol.r) * vs_characterColor.rgb, charCol.r * vs_characterColor.a);
            // fragColor = vec4(1.0) * vec4(vs_UVAttribute, 1.0, 1.0);
        }
    }
}