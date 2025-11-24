#version 330 core
uniform int isBorder;
in vec4 out_Color;
out vec4 FragColor;

void main()
{
    if(isBorder == 1)
        FragColor = vec4(0,0,0,1); // 
    else
        FragColor = vec4(out_Color);
}