vec4 calculatePPLayerVolume(int layerIndex)
{
    return vec4(float(layerIndex) * 10.0 / 255.0, 0.0, 0.0, 1.0);
}