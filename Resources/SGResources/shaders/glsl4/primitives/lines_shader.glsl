#define PI 3.14

#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    uniform vec3 offset;
    uniform vec3 verticesPositions[2];

    void main()
    {
        #if defined(SG_TRANSLATE_WITH_OBJECT) && defined(SG_ROTATE_WITH_OBJECT) && defined(SG_SCALE_WITH_OBJECT)
            gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(offset + verticesPositions[gl_VertexID], 1.0);
        #else
            mat4 newObjModelMatrix;

            #ifdef SG_TRANSLATE_WITH_OBJECT
                newObjModelMatrix[0] = vec4(1.0, 0.0, 0.0, 0.0);
                newObjModelMatrix[1] = vec4(0.0, 1.0, 0.0, 0.0);
                newObjModelMatrix[2] = vec4(0.0, 0.0, 1.0, 0.0);
                newObjModelMatrix[3] = vec4(objectPosition, 1.0);
            #endif

            #ifdef SG_ROTATE_WITH_OBJECT

            #endif

            #ifdef SG_SCALE_WITH_OBJECT

            #endif

            gl_Position = projectionMatrix * viewMatrix * newObjModelMatrix * vec4(offset + verticesPositions[gl_VertexID], 1.0);
        #endif
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform vec4 color;

    void main()
    {
        fragColor = vec4(color.rgb, color.a);
    }
#endif
