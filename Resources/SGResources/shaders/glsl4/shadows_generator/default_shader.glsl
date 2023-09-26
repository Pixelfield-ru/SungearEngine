#include "../uniform_bufs_decl.glsl"

#ifdef VERTEX_SHADER
    layout (location = 0) in vec3 positionsAttribute;
    layout (location = 1) in vec3 UVAttribute;
    layout (location = 2) in vec3 normalsAttribute;

    out vec3 vs_UVAttribute;

    void main()
    {
        vs_UVAttribute = UVAttribute;

        gl_Position = projectionMatrix * viewMatrix * objectModelMatrix * vec4(positionsAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    uniform sampler2D sgmat_diffuse4;
    uniform sampler2D sgmat_baseColor8;

    in vec3 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;
        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        #ifdef sgmat_diffuse4_DEFINED
            float a = texture(sgmat_diffuse4, finalUV).a;
            if(a < 0.2)
            {
                discard;
            }
            else
            {
                gl_FragColor = vec4(gl_FragCoord.z);
            }
        #elif sgmat_baseColor8_DEFINED
            float a = texture(sgmat_baseColor8, finalUV).a;
            // todo: make 0.2 flexible value
            if(a < 0.2)
            {
                discard;
            }
            else
            {
                gl_FragColor = vec4(gl_FragCoord.z);
            }
            //gl_FragColor = vec4(gl_FragCoord.zzz * texture(sgmat_baseColor8, finalUV).a, gl_FragCoord.z);
        #else
            gl_FragColor = vec4(1.0);
        #endif
        //gl_FragColor = vec4(0.0);
    }
#endif
