#include "sg_shaders/impl/glsl4/random.glsl"
#include "sg_shaders/impl/glsl4/pbr_base.glsl"
#include "sg_shaders/impl/glsl4/pbr/dir_lights_shadows_calc.glsl"

vec3 calculateLight(vec3 albedo, vec3 fragPos, vec3 viewDir, vec3 normal, float roughness, float specularCoeff, float metalness, float ao)
{
    vec3 light = vec3(0.0);

    // для формулы Шлика-Френеля
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metalness);

    for(int i = 0; i < sg_spotLightsCount; ++i)
    {
        SpotLight spotLight = sg_spotLights[i];

        vec3 lightDir = normalize(spotLight.position - fragPos);

        float theta = dot(lightDir, normalize(-spotLight.direction));

        if(theta < spotLight.outerCutoff)
        {
            continue;
        }

        float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
        float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);

        vec3 halfWayDir = normalize(lightDir + viewDir);

        float distance = length(spotLight.position - fragPos);
        // float attenuation = (1.0 / (distance * distance)) * intensity * spotLight.intensity;
        float attenuation = min((1.0 / (distance * distance)) * intensity * spotLight.intensity, 10.0);
        vec3 radiance = spotLight.color.rgb * attenuation;

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = max(dot(normal, lightDir), 0.0);
        float NdotVD = max(dot(normal, viewDir), 0.0);

        vec3 finalRadiance = NdotL * radiance + radiance * 0.04;

        // NDF (normal distribution func)
        float D = GGXTR(
            normal,
            halfWayDir,
            roughness * (1.0 - specularCoeff)
        );

        float cosTheta = max(dot(halfWayDir, viewDir), 0.0);

        // это по сути зеркальная часть (kS)
        vec3 F = SchlickFresnel(cosTheta, F0);// kS
        // geometry function
        float G = GeometrySmith(NdotVD, NdotL, roughness * (1.0 - specularCoeff));

        vec3 diffuse = vec3(1.0) - F;
        diffuse *= (1.0 - metalness);// check diffuse color higher

        vec3 ctNumerator = D * F * G;
        float ctDenominator = 4.0 * NdotVD * NdotL;
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.rgb;

        light += (diffuse * albedo / PI + specular) * radiance * NdotL;

        // WOW THATS NOT BAD
        // ambient = 0.25 * albedo.rgb * ao * attenuation;
        // lo = vec3(1.0);
    }

    // calculating sun =======================================

    float shadow = getCSMShadow(atmosphere.sunPosition, fragPos);

    {
        // ambient += atmosphere.sunAmbient;

        vec3 lightDir = normalize(atmosphere.sunPosition);// TRUE
        vec3 halfWayDir = normalize(lightDir + viewDir);// TRUE

        // energy brightness coeff (коэфф. энергетической яркости)
        float NdotL = saturate(dot(normal, lightDir));
        float NdotVD = abs(dot(normal, viewDir)) + 1e-5f;

        // NDF (normal distribution func)
        float D = GGXTR(
            normal,
            halfWayDir,
            roughness * (1.0 - specularCoeff)
        );// TRUE

        float cosTheta = saturate(dot(halfWayDir, viewDir));

        // это по сути зеркальная часть (kS)
        vec3 F = SchlickFresnel(cosTheta, F0);// kS
        // geometry function
        float G = GeometrySmith(NdotVD, NdotL, roughness * (1.0 - specularCoeff));// TRUE

        vec3 diffuse = vec3(1.0) - F;
        diffuse *= (1.0 - metalness);// check diffuse color higher

        vec3 ctNumerator = D * F * G;
        float ctDenominator = 1.0 * NdotVD * NdotL;
        vec3 specular = (ctNumerator / max(ctDenominator, 0.001)) * u_materialSpecularCol.r;

        light += (diffuse * albedo / PI + specular) * max(atmosphere.sunColor.rgb, vec3(0, 0, 0)) * NdotL * shadow * 1.0;
    }

    vec3 ambient = albedo * ao * max(dot(atmosphere.sunPosition, vec3(0, 1, 0)), 0.0);
    return ambient * vec3(1.0) * materialAmbientFactor + light;
}