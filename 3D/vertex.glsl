#version 330 core
layout(location = 0) in vec3 inPos;   // VBO에서 위치 입력
layout(location = 1) in vec4 inColor; // VBO에서 색상 입력
out vec4 out_Color;                   // 프래그먼트 셰이더로 색상 전달

uniform mat4 u;   // 최종 행렬


void main()
{
    gl_Position =  u  * vec4(inPos, 1.0);
    out_Color = inColor;
}