#sg_pragma once

#define SG_MAKE_NOT_THIS_LAYER_COLOR(layerIndex) vec4((float(layerIndex) * 10.0) / 255.0, 0, 0, 1)

SGSubPass(PostProcessLayerDepthPass, PostProcessLayerFXPass, PostProcessAttachmentsCombiningPass, PostProcessFinalFXPass)
{
    SGSubShader(Vertex)
    {
        #sg_include "../../primitives.glsl"

        out vec2 vs_UVAttribute;

        void main()
        {
            vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

            vs_UVAttribute = quad2DUVs[gl_VertexID];

            gl_Position = vec4(pos, 0.0, 1.0);
        }
    }
}
