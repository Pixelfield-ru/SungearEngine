#sg_pragma once

SGSubPass(SGLPPLayerDepthPass, SGLPPLayerFXPass, SGLPPAttachmentsCombiningPass, SGLPPFinalFXPass)
{
    SGSubShader(Vertex)
    {
        // #sg_include "../../primitives.glsl"
        #sg_include "sg_shaders/glsl4/primitives.glsl"

        out vec2 vs_UVAttribute;

        void main()
        {
            vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

            vs_UVAttribute = quad2DUVs[gl_VertexID];

            gl_Position = vec4(pos, 0.0, 1.0);
        }
    }
}
