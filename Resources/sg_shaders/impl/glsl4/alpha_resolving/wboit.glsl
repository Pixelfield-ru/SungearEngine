void calculateWBOITComponents(vec3 inputCol,
                              float inputAlpha,
                              float fragCoordZ,
                              out vec4 outputAccumColor,
                              out vec4 outputLayerColor,
                              out float outputReveal,
                              int isTransparentPass)
{
    if(isTransparentPass == 1)
    {
        float weight = clamp(pow(min(1.0, inputAlpha * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - fragCoordZ * 0.9, 3.0), 1e-2, 3e3);

        outputAccumColor = vec4(inputCol.rgb * inputAlpha, inputAlpha) * weight;

        outputReveal = inputAlpha;
    }
    else
    {
        outputLayerColor = vec4(inputCol.rgb, 1.0);
    }
}