#ifdef VERTEX_SHADER
    // id аттрибута = 0. позиции вершин. входной параметр
    layout (location = 0) in vec3 positionAttribute;
    // id аттрибута = 2. текстурная координата вершины. входной параметр
    layout (location = 1) in vec3 textureCoordsAttribute;
    layout (location = 2) in vec3 normalPositionAttribute;

    uniform mat4 mvpMatrix;

    out vec3 vs_textureCoords;

    void main()
    {
        vs_textureCoords = textureCoordsAttribute;

        gl_Position = mvpMatrix * vec4(positionAttribute, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    out vec4 fragColor;

    uniform sampler2D tex;

    uniform vec4 color;

    in vec3 vs_textureCoords;

    void main()
    {
        #if defined(FLIP_TEXTURES_Y) && FLIP_TEXTURES_Y == 1
                vec4 textureColor = texture(tex, vec2(vs_textureCoords.x, 1.0 - vs_textureCoords.y));
        #else
                vec4 textureColor = texture(tex, vec2(vs_textureCoords.x, vs_textureCoords.y));
        #endif

        fragColor = color * textureColor;
    }
#endif
