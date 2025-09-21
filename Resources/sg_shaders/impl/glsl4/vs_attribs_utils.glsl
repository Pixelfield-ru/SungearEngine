mat4 getCurrentInstanceModelMatrix()
{
    #ifdef SG_INSTANCED_RENDERING
    return SG_INSTANCED_instanceModelMatrix;
    #else
    return objectTransform.modelMatrix;
    #endif
}

vec3 getCurrentInstancePosition()
{
    #ifdef SG_INSTANCED_RENDERING
    return SG_INSTANCED_instancePosition.xyz;
    #else
    return objectTransform.position;
    #endif
}

vec3 getCurrentInstanceRotation()
{
    #ifdef SG_INSTANCED_RENDERING
    return SG_INSTANCED_instanceRotation.xyz;
    #else
    return objectTransform.rotation;
    #endif
}

vec3 getCurrentInstanceScale()
{
    #ifdef SG_INSTANCED_RENDERING
    return SG_INSTANCED_instanceScale.xyz;
    #else
    return objectTransform.scale;
    #endif
}

vec4 getVertexColor0()
{
    #if defined(SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC) && defined(SG_VS_MAX_ATTRIBS_COUNT) && SG_VS_VERTEX_COLOR_ATTRIBUTE0_LOC < SG_VS_MAX_ATTRIBS_COUNT
    return vertexColor0Attribute;
    #else
    return vec4(1.0);
    #endif
}

vec4 getVertexColor1()
{
    #if defined(SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC) && defined(SG_VS_MAX_ATTRIBS_COUNT) && SG_VS_VERTEX_COLOR_ATTRIBUTE1_LOC < SG_VS_MAX_ATTRIBS_COUNT
    return vertexColor1Attribute;
    #else
    return vec4(1.0);
    #endif
}