//
// Created by ilya on 15.02.24.
//

#include "AtmosphereUtils.h"
#include <cmath>
#include <glm/geometric.hpp>

glm::vec3
SGCore::AtmosphereUtils::rayleighMieAtmosphere(glm::vec3 r, const glm::vec3& ro, glm::vec3 pSun,
                                                float iSun, float rPlanet, float rAtmos, const glm::vec3& kRlh,
                                                float kMie, float shRlh, float shMie, float g, int iSteps,
                                                int jSteps) noexcept
{
    // Normalize the sun and view directions.
    pSun = glm::normalize(pSun);
    r = glm::normalize(r);
    
    // Calculate the step size of the primary ray.
    glm::vec2 p = MathUtils::raySphereIntersection(ro, r, rAtmos);
    if (p.x > p.y) return { 0, 0, 0 };
    p.y = std::min(p.y, MathUtils::raySphereIntersection(ro, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);
    
    // Initialize the primary ray time.
    float iTime = 0.0;
    
    // Initialize accumulators for Rayleigh and Mie scattering.
    glm::vec3 totalRlh = { 0, 0, 0 };
    glm::vec3 totalMie = { 0, 0, 0 };
    
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
    for (int i = 0; i < iSteps; ++i) {
        
        // Calculate the primary ray sample position.
        glm::vec3 iPos = ro + r * (iTime + iStepSize * 0.5f);
        
        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;
        
        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = std::exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = std::exp(-iHeight / shMie) * iStepSize;
        
        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;
        
        // Calculate the step size of the secondary ray.
        float jStepSize = MathUtils::raySphereIntersection(iPos, pSun, rAtmos).y / float(jSteps);
        
        // Initialize the secondary ray time.
        float jTime = 0.0;
        
        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;
        
        // Sample the secondary ray.
        for (int j = 0; j < jSteps; ++j) {
            
            // Calculate the secondary ray sample position.
            glm::vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5f);
            
            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;
            
            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;
            
            // Increment the secondary ray time.
            jTime += jStepSize;
        }
        
        // Calculate attenuation.
        glm::vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));
        
        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;
        
        // Increment the primary ray time.
        iTime += iStepSize;
    }
    
    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}
