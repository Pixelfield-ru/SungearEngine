void calculateWBOITComponents(vec3 inputCol, float inputAlpha, float fragCoordZ, out vec4 outputColor, out float outputAlphaAccum)
{
    // float weight = inputAlpha * (1.0 - fragCoordZ);
    float weight = clamp(pow(min(1.0, inputAlpha * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - fragCoordZ * 0.9, 3.0), 1e-2, 3e3);

    // outputColor = vec4(inputCol.rgb * inputAlpha, inputAlpha) * weight;

    outputColor.rgb = inputCol.rgb;
    outputColor.a = 1.0;

    outputAlphaAccum = inputAlpha;
}