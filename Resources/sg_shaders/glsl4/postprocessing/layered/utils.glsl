#sg_pragma once

vec3 calculatePPLayerVolume(int layerIndex)
{
    return vec3(float(layerIndex) * 10.0 / 255.0, 0.0, 0.0);
}