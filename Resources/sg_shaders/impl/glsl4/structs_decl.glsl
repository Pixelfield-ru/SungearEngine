struct ProgramData
{
    vec2 windowSize;
    vec2 primaryMonitorSize;
    float p0;
    float currentTime;
};

struct ObjectTransform
{
    mat4 modelMatrix;
    vec3 position;
    float p0;
    vec3 rotation;
    float p1;
    vec3 scale;
    float p2;
};

struct IRenderingComponent
{
    mat4 projectionSpaceMatrix;
    mat4 orthographicSpaceMatrix;
    mat4 orthographicMatrix;
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 position;
    float zFar;
    vec3 rotation;
    float p1;
    vec3 scale;
    float p2;
};

struct ILight
{
    mat4 projectionSpaceMatrix;
    mat4 orthographicSpaceMatrix;
    mat4 orthographicMatrix;
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 position;
    float p0;
    vec3 rotation;
    float p1;
    vec3 scale;
    float p2;
    vec4 color;
    float intensity;
    int shadowSamplesCount;
};

struct SpotLight
{
    mat4 projectionSpaceMatrix;
    mat4 modelMatrix;
    vec3 position;
    float p0;
    vec3 rotation;
    float p1;
    vec4 color;
    float intensity;
    int shadowSamplesCount;
    int cutoffSamplerIndex; // -1 means sampler is not used
    float cutoffRadius;
};

struct Atmosphere
{
    vec3 sunAmbient;
    float p0;

    vec3 sunPosition;
    float p1;

    vec3 sunColor;
    float rayleighScaleHeight;

    vec3 rayleighScatteringCoeff;
    float mieScatteringCoeff;

    float mieScaleHeight;
    float sunIntensity;

    float planetRadius;
    float atmosphereRadius;

    vec3 rayOrigin;
    float miePreferredScatteringDirection;
};
