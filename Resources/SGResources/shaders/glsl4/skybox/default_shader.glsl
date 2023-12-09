#define SG_NOT_INCLUDE_LIGHTS

#include "../ray_intersections.glsl"
#include "../defines.glsl"
#include "../uniform_bufs_decl.glsl"

#define PI 3.141592
#define iSteps 16
#define jSteps 8

vec3 atmosphere(vec3 r, vec3 ro,
                vec3 pSun, float iSun,
                float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    vec2 p = raySphereIntersection(ro, r, rAtmos);
    if (p.x > p.y) return vec3(0,0,0);
    p.y = min(p.y, raySphereIntersection(ro, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = ro + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = raySphereIntersection(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    out vec3 vs_UVAttribute;

    void main()
    {
        vs_UVAttribute = positionsAttribute;

        gl_Position = camera.projectionMatrix * mat4(mat3(camera.viewMatrix)) * objectModelMatrix * vec4(positionsAttribute, 1.0);
        //vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(positionsAttribute, 1.0);
        //gl_Position = vec4(pos.xy, pos.w, pos.w);
    }
#endif

#ifdef FRAGMENT_SHADER
    layout(location = 0) out vec4 fragColor;
    // layout(location = 1) out vec3 gFragPos;
    // layout(location = 2) out vec3 gNormal;
    // layout(location = 3) out vec4 gAlbedoSpec;
    // layout(location = 4) out vec3 unknownAttachment0;
    // layout(location = 5) out vec3 unknownAttachment1;

    const vec3 sunPos = vec3(0.0, 1.0, -1.0);

    uniform int sgmat_skyboxSamplers_COUNT;
    uniform samplerCube sgmat_skyboxSamplers[SGMAT_SAMPLERS_OF_TYPE_MAX];

    in vec3 vs_UVAttribute;

    void main()
    {
        // gFragPos = vec3(1.0);
        // gNormal = vec3(1.0);
        // gAlbedoSpec = vec4(1.0);
        // unknownAttachment0 = vec3(1.0);
        // unknownAttachment1 = vec3(1.0);

        vec3 atmosphereCol = atmosphere(
            vs_UVAttribute,           // normalized ray direction
            vec3(0, 6372e3, 0),               // ray origin
            sunPos,                        // position of the sun
            30.0,                           // intensity of the sun
            6371e3,                         // radius of the planet in meters
            6471e3,                         // radius of the atmosphere in meters
            vec3(14.5e-6, 15.0e-6, 17.4e-6), // Rayleigh scattering coefficient
            20e-6,                          // Mie scattering coefficient
            8e3,                            // Rayleigh scale height
            1.0e3,                          // Mie scale height
            0.958                          // Mie preferred scattering direction
        );

        // vec3 atmosphereCol = vec3(1.0);
        if(sgmat_skyboxSamplers_COUNT > 0)
        {
            float mixCoeff = 1.0 / sgmat_skyboxSamplers_COUNT;
            vec4 skyboxCol = vec4(0.0);
            for (int i = 0; i < sgmat_skyboxSamplers_COUNT; i++)
            {
                skyboxCol += texture(sgmat_skyboxSamplers[i], vs_UVAttribute.xyz) * mixCoeff;
            }

            fragColor = vec4(atmosphereCol * skyboxCol.rgb, skyboxCol.a);
        }
        else
        {
            fragColor = vec4(atmosphereCol, 1.0);
        }
    }
#endif
