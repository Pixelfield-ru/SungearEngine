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
        float weight = pow(inputAlpha, 1.0) * clamp(0.3 / (1e-5 + pow(fragCoordZ / 200, 4.0)), 1e-2, 3e3);


        outputAccumColor = vec4(inputCol.rgb * inputAlpha * weight, inputAlpha * weight);

        outputReveal = inputAlpha;
    }
    else
    {
        outputLayerColor = vec4(inputCol.rgb, 1.0);
        outputReveal = 1.0;
    }
}