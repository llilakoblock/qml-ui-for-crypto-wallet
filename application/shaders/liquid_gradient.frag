#version 450

layout(location = 0) in vec2 coord;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 resolution;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
} ubufFrag;

void main()
{
    vec3 resultColor = color;
    vec2 st = coord.xy / resolution.xy;
    resultColor.g -= pow(st.y + sin(-12.0) * st.x, 5.0) * 0.4;

    fragColor = vec4(resultColor.rgb, ubufFrag.qt_Opacity);
}
