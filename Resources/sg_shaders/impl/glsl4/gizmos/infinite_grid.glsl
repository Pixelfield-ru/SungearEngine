#include "sg_shaders/impl/glsl4/defines.glsl"
#include "sg_shaders/impl/glsl4/uniform_bufs_decl.glsl"
#include "sg_shaders/impl/glsl4/alpha_resolving/wboit.glsl"

#subpass [GeometryPass]

#vertex

#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec3 positionsAttribute;
layout (location = 1) in vec3 UVAttribute;

out vec3 nearPoint;
out vec3 farPoint;
/*out vec3 fragView;
out vec3 fragProjection;*/

out vec3 vs_UVAttribute;

vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection)
{
    mat4 view_inverse = inverse (view);
    mat4 projection_inverse = inverse (projection);
    vec4 unprojected_point = view_inverse * projection_inverse * vec4 (x, y, z, 1.0);

    return unprojected_point.xyz / unprojected_point.w;
}

void main()
{
    vs_UVAttribute = UVAttribute;

    // vec3 p = gridPlane[gl_VertexIndex].xyz;
    // vec3 p = positionsAttribute;
    /*nearPoint = UnprojectPoint(p.x, p.y, 0.0, camera.viewMatrix, camera.projectionMatrix).xyz;
    farPoint = UnprojectPoint(p.x, p.y, 1.0, camera.viewMatrix, camera.projectionMatrix).xyz;*/
    // gl_Position = vec4(p, 1.0);

    nearPoint = unprojectPoint(positionsAttribute.x, positionsAttribute.y, 0.0, camera.viewMatrix, camera.projectionMatrix).xyz;
    farPoint = unprojectPoint(positionsAttribute.x, positionsAttribute.y, 1.0, camera.viewMatrix, camera.projectionMatrix).xyz;
    /*frag_view = view;
    frag_projection = projection;*/

    gl_Position = vec4(positionsAttribute, 1.0);
}

#end

// =========================================================================
// =========================================================================
// =========================================================================

#fragment

#include "sg_shaders/impl/glsl4/postprocessing/layered/utils.glsl"

layout(location = 0) out vec4 layerVolume;
layout(location = 1) out vec4 layerColor;
layout(location = 3) out vec4 layerWBOITColorAccum;
layout(location = 4) out float layerWBOITReveal;

in vec3 nearPoint;
in vec3 farPoint;

in vec3 vs_UVAttribute;

const float gridHeight = 1.0;

uniform int SGPP_CurrentLayerIndex;

vec4 grid(vec3 fragPosition3D, float scale)
{
    // dont want the grid to be infinite?
    // 	uncomment this bit, set your boundaries to whatever you want
    //if (frag_position_3d.x > 10
    //	|| frag_position_3d.x < -10
    //	|| frag_position_3d.z > 10
    //	|| frag_position_3d.z < -10)
    //{
    //	return vec4 (0, 0, 0, 0);
    //}

    vec2 coord = fragPosition3D.xz * scale;
    vec2 derivative = fwidth (coord);
    vec2 grid = abs (fract (coord - 0.5) - 0.5) / derivative / 15.0;
    float line = min (grid.x, grid.y);
    float minimum_z = min (derivative.y, 1);
    float minimum_x = min (derivative.x, 1);
    vec4 color = vec4 (0.2, 0.2, 0.2, 1.0 - min (line, 1.0));

    // z axis color
    //if (fragPosition3D.x > -0.1 * minimum_x
    //	&& fragPosition3D.x < 0.1 * minimum_x)
    //{
    //	color.z = 1.0;
    //}

    //// x axis color
    //if (fragPosition3D.z > -0.1 * minimum_z
    //	&& fragPosition3D.z < 0.1 * minimum_z)
    //{
    //	color.x = 1.0;
    //}

    return color;
}

float computeDepth(vec3 position)
{
    vec4 clipSpacePosition = camera.projectionMatrix * camera.viewMatrix * vec4(position.xyz, 1.0);

    // the depth calculation in the original article is for vulkan
    // the depth calculation for opengl is:
    // 	(far - near) * 0.5 * ndc_depth + (far + near) * 0.5
    // 	far = 1.0  (opengl max depth)
    // 	near = 0.0  (opengl min depth)
    //		ndc_depth = clip_space_position.z / clip_space_position.w
    //	since our near and far are fixed, we can reduce the above formula to the following
    return 0.5 + 0.5 * (clipSpacePosition.z / clipSpacePosition.w);
    // this could also be (ndc_depth + 1.0) * 0.5
}

float computeLinearDepth(vec3 position)
{
    float near = 0.5;
    float far = 250;
    vec4 clipSpacePosition = camera.projectionMatrix * camera.viewMatrix * vec4(position.xyz, 1.0);
    float clipSpaceDepth = (clipSpacePosition.z / clipSpacePosition.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));

    return linearDepth / far;
}

void main()
{
    float t = (gridHeight - nearPoint.y) / (farPoint.y - nearPoint.y);
    vec3 fragPosition3D= nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPosition3D);

    float linearDepth = computeLinearDepth(fragPosition3D);
    float fading = max(0, (0.5 - linearDepth));

    vec4 gridColor = (grid(fragPosition3D, 1) + grid(fragPosition3D, 1)) * float(t > 0);
    gridColor.a *= fading;

    /*layerColor = gridColor;
    layerColor.a *= fading;*/

    {
        /*calculateWBOITComponents(gridColor.rgb, gridColor.a, gl_FragCoord.z, layerColor, layerWBOITAccumAlpha.r);
        layerWBOITAccumAlpha.a = 1.0;*/

        layerColor = gridColor;
        layerWBOITReveal = 1.0;
    }

    layerVolume = calculatePPLayerVolume(SGPP_CurrentLayerIndex);

    // fragColor.a = 0.0;
    // if(fading < 0.2) discard;
}

#end
