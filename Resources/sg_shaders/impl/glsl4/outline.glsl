#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/math.glsl"
#include "sg_shaders/impl/glsl4/defines.glsl"

#subpass [OutlinePass]

#vertex

#include "sg_shaders/impl/glsl4/primitives.glsl"

layout (location = 0) in vec3 positionsAttribute;
layout (location = 5) in ivec4 bonesIDsAttribute0;
layout (location = 6) in ivec4 bonesIDsAttribute1;
layout (location = 7) in vec4 bonesWeightsAttribute0;
layout (location = 8) in vec4 bonesWeightsAttribute1;
/*layout (location = 1) in vec3 UVAttribute;
layout (location = 2) in vec3 normalsAttribute;*/

#include "sg_shaders/impl/glsl4/animation/bones_calculation.glsl"

uniform int u_pass;

flat out int vs_pass;
out vec2 vs_quadUV;
out vec3 vs_fragPos;

void main()
{
    vs_pass = u_pass;
    /*vec4 cameraProjCords = objectTransform.modelMatrix * vec4(positionsAttribute, 1.0);
    vs_fragPos = (cameraProjCords.xyz / cameraProjCords.w) * 0.5 + 0.5;*/

    vs_fragPos = vec3(objectTransform.modelMatrix * vec4(positionsAttribute, 1.0));

    // firstly drawing model
    if(u_pass == 1)
    {
        vec4 totalPosition = vec4(0.0);
        vec3 totalNormal = vec3(0.0);

        calculateVertexPosAndNormal(positionsAttribute, vec3(0.0), totalPosition, totalNormal);

        gl_Position = camera.projectionSpaceMatrix * objectTransform.modelMatrix * totalPosition;
    }
    else if(u_pass >= 2) // then drawing postprocess quads
    {
        vs_quadUV = quad2DUVs[gl_VertexID];

        gl_Position = vec4(positionsAttribute.xy, 0.0, 1.0);
    }

}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

// Two variants of one program, chosen by SG_OUTLINE_COMBINE (see OutlinePass::create).
// Passes 1 and 2 draw into attachments 0 and 1, pass 3 into attachment 7 alone. A single program
// declaring both outputs is legal, but its pipeline for pass 3 then declares a write to a location
// with no attachment behind it, and validation warns on every draw — nine times per frame here.
// The branch on vs_pass is invisible to that analysis, so the outputs have to differ per variant.
// In the combine variant location 0 IS colour attachment 7.
// Two rules the SGSL translator imposes here, both learned the hard way:
//  - no #ifndef: it comes out of the translator as a bare "#" and the guarded block disappears;
//  - no trailing // comment on the line before a directive: comments are stripped and the lines are
//    joined, so the directive ends up glued to the end of the previous statement.
#ifdef SG_OUTLINE_COMBINE
layout(location = 0) out vec4 outColor;
#else
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outScaledColor;
#endif

uniform float u_outlineThickness;
uniform vec4 u_outlineColor;
uniform sampler2D u_outlineBuffer;
uniform sampler2D u_scaledOutlineBuffer;

flat in int vs_pass;
in vec2 vs_quadUV;
in vec3 vs_fragPos;

// Every output is assigned on every path: a fragment output left unassigned is undefined, and the
// backends differ exactly there — GL happened to leave the attachment alone, Vulkan wrote whatever
// was in the register. Pass 3 relied on that for the whole frame: wherever there is no outline it
// assigned nothing and expected attachment 7 to keep the scene colour, which on Vulkan turned the
// final image into noise. "Leave the attachment alone" is spelled discard (no blending, no depth).
void main()
{
#ifdef SG_OUTLINE_COMBINE
    // pass 3: combine the scaled outline with the scene colour already in attachment 7
    vec3 outlineCol = texture(u_scaledOutlineBuffer, vs_quadUV).rgb;

    // no outline here: attachment 7 keeps the scene colour
    if(outlineCol == vec3(0.0))
    {
        discard;
    }

    outColor = vec4(outlineCol, 1.0);
#else
    if(vs_pass == 1) // firstly drawing object with outline color
    {
        outColor = u_outlineColor;
        outScaledColor = vec4(0.0);
    }
    else if(vs_pass == 2) // then scaling this object and taking only outline (taking fragments that are not inside the object)
    {
        vec4 col = vec4(0.0, 0.0, 0.0, 1.0);

        int count = 0;

        // todo: idk why this pass only with calculating pixelSize.
        // if we dont calculate pixelSize then ouline will be incorrect for different distance between fragment and camera
        vec2 pixelSize = 1.0 / vec2(textureSize(u_outlineBuffer, 0).xy);

        const float boxSize = 1.0;

        for(float x = -boxSize; x <= boxSize; x += 1.0)
        {
            for(float y = -boxSize; y <= boxSize; y += 1.0)
            {
                vec3 tmpCol = texture(u_outlineBuffer, vs_quadUV + vec2(x, y) * pixelSize * u_outlineThickness).rgb;

                if(tmpCol.rgb != vec3(0.0))
                {
                    col.rgb += texture(u_outlineBuffer, vs_quadUV + vec2(x, y) * pixelSize * u_outlineThickness).rgb;
                    ++count;
                }
            }
        }

        col.rgb /= float(count);

        vec3 curCol = texture(u_outlineBuffer, vs_quadUV).rgb;

        // attachment 0 is still a draw target in this pass and neighbouring fragments sample it:
        // write back what is there so nothing else lands in it
        outColor = vec4(curCol, 1.0);

        // writing outline only if fragment is not inside the object in vs_pass == 1
        if(curCol == vec3(0.0, 0.0, 0.0))
        {
            outScaledColor = col;
        }
        else
        {
            outScaledColor = vec4(0.0);
        }
    }
    else
    {
        discard;
    }
#endif
}

#end