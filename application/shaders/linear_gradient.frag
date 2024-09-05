#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
 
    float angle;
    vec2 sourceSize;
};

layout(binding = 1) uniform sampler2D source;
layout(binding = 2) uniform sampler2D gradientSource;

void main()
{
    vec4 sourceTexel = texture(source, texCoord);

    float sinFactor = sin(radians(angle));
    float cosFactor = cos(radians(angle));

    vec2 coord = texCoord.xy;
    coord -= vec2(0.5);
    coord *= mat2(cosFactor, sinFactor, -sinFactor, cosFactor);
    coord += vec2(0.5);

    vec4 gradientTexel = texture(gradientSource, coord);

    fragColor = gradientTexel * sourceTexel.a * qt_Opacity;
}
