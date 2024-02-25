#sg_pragma once

#sg_include "../uniform_bufs_decl.glsl"

SGSubPass(PhysicsLinesDebugPass)
{
    SGSubShader(Vertex)
    {
        layout (location = 0) in vec3 linesPositionsAttribute;
        layout (location = 1) in vec3 linesColorsAttribute;

        out vec4 fs_lineCol;

        void main()
        {
            fs_lineCol = vec4(linesColorsAttribute, 1.0);

            gl_Position = camera.projectionSpaceMatrix * vec4(linesPositionsAttribute, 1.0);
        }
    }

    SGSubShader(Fragment)
    {
        out vec4 fragColor;

        in vec4 fs_lineCol;

        void main()
        {
            fragColor = fs_lineCol;
        }
    }
}