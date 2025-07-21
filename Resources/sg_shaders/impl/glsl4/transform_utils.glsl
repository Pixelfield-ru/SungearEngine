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