#sg_ifndef DIR_LIGHTS_SHADOWS_CALC
#sg_define DIR_LIGHTS_SHADOWS_CALC

#sg_include "../rendering_structs_decl.glsl"
#sg_include "../math.glsl"

// todo: make PCSS, PCF

float calcPoissonShadow(
    const in DirectionalLight dirLight,
    const in vec3 projCoords,
    const in vec3 fragPos,
    const in vec3 normal,
    const in sampler2D shadowMap
)
{
    const float shadowsBias = 0.000025;

    ILight lightPart = dirLight.lightPart;

    const float shadowsMinCoeff = 0.0;

    float fragmentVisibility = 1.0;
    float downstep = (1.0 - shadowsMinCoeff) / lightPart.shadowSamplesCount;

    float rand = random(projCoords.xy);
    float rotAngle = rand * PI;
    vec2 rotTrig = vec2(cos(rotAngle), sin(rotAngle));

    for(int i = 0; i < lightPart.shadowSamplesCount; i++)
    {
        if(texture(shadowMap, projCoords.xy + rotate(poissonDisk[i], rotTrig) / 750.0).z < projCoords.z - shadowsBias)
        {
            fragmentVisibility -= downstep;
        }
    }

    return fragmentVisibility;
}

float calcDirLightShadow(
    const in DirectionalLight dirLight,
    const in vec3 fragPos,
    const in vec3 normal,
    const in sampler2D shadowMap
)
{
    IRenderingComponent renderingPart = dirLight.lightPart.renderingPart;
    vec4 lightSpaceFragPos = renderingPart.spaceMatrix * vec4(fragPos, 1.0);

    vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
    {
        return 1.0;
    }

    return calcPoissonShadow(dirLight, projCoords, fragPos, normal, shadowMap);
}

#sg_endif // DIR_LIGHTS_SHADOWS_CALC
