#subpass [InstancingPass]

#vertex

#include "../uniform_bufs_decl.glsl"

layout (location = 0) in mat4 instanceModelMatrix;
layout (location = 4) in vec3 instanceVertexUV;
layout (location = 5) in vec3 instanceVertexPosition;

out vec3 vs_UVAttribute;
flat out int vs_instanceID;

void main()
{
    vs_UVAttribute = instanceVertexUV;
    vs_instanceID = gl_InstanceID;

    gl_Position = camera.projectionSpaceMatrix * instanceModelMatrix * vec4(instanceVertexPosition, 1.0);
}

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

layout(location = 0) out vec4 fragColor;

in vec3 vs_UVAttribute;
flat in int vs_instanceID;

void main()
{
    vec4 charCol = vec4(0, 0, 0, 1);

    vec2 finalUV = vs_UVAttribute.xy;
    #ifdef FLIP_TEXTURES_Y
        finalUV.y = 1.0 - vsIn.UV.y;
    #endif

    // fragColor = charCol;
    // fragColor = vec4(vec3(charCol.r) * vs_characterColor.rgb, charCol.r * vs_characterColor.a);
    fragColor = vec4(vec3(sin(float(vs_instanceID))), 1.0);
}