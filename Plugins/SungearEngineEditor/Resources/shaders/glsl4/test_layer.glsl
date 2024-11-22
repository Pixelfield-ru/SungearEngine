#sg_pragma once

// #sg_include "D:\Pixelfield\NativeSungearEngine\NativeSungearEngine\Resources\shaders\glsl4\postprocessing\layered\base.glsl"

SGSubPass(SGLPPLayerDepthPass, SGLPPLayerFXPass, SGLPPAttachmentsCombiningPass, SGLPPFinalFXPass)
{
    SGSubShader(Vertex)
    {
        const vec2 quad2DVerticesPositions[] = vec2[]
        (
        vec2(-1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, -1.0)
        );

        const vec2 quad2DUVs[] = vec2[]
        (
        vec2(0.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, 0.0)
        );


        out vec2 vs_UVAttribute;

        void main()
        {
            vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

            vs_UVAttribute = quad2DUVs[gl_VertexID];

            gl_Position = vec4(pos, 0.0, 1.0);
        }
    }
}


SGSubPass(SGLPPLayerFXPass)
{
    SGSubShader(Fragment)
    {
        uniform sampler2D currentLayer;

        // ===================================================
        // !! - TO GET INDICES OF ALL LAYERS USE UNIFORM ARRAY 'SGLPP_LayersIndices'
        // !! - TO GET LAYERS COUNT USE UNIFORM INT 'SGLPP_LayersCount'
        // !! - TO GET CURRENT LAYER INDEX USE UNIFORM INT 'SGLPP_CurrentLayerIndex'. !! WARNING - THE INDEXES OF THE LAYERS ARE NOT NECESSARILY SEQUENTIAL
        // !! - TO GET CURRENT LAYER SEQUENTIAL INDEX USE UNIFORM INT 'SGLPP_CurrentLayerSeqIndex'
        // !! - TO GET CURRENT LAYER CURRENT SUB PASS INDEX USE UNIFORM INT 'SGLPP_CurrentSubPassIndex'
        // ===================================================

        in vec2 vs_UVAttribute;

        void main()
        {
            vec2 finalUV = vs_UVAttribute.xy;

            #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
            #endif

            // just do nothing

            // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            gl_FragColor = vec4(texture(currentLayer, finalUV).rgb, 1.0);
        }
    }
}