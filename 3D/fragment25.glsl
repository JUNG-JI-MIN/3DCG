#version 330 core
in vec4 out_Color;
in vec3 out_Normal;
in vec3 out_FragPos;
out vec4 FragColor;

uniform vec3 lightPos;     // 빛 위치
uniform vec3 viewPos;      // 카메라 위치
uniform vec3 lightColor;
uniform int turn_off;

void main()
{
    // 1. Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse (확산광)
    vec3 norm = normalize(out_Normal);
    vec3 lightDir = normalize(lightPos - out_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse;
    if (turn_off == 1){
        diffuse = vec3(0.0);
    }
    else diffuse = diff * lightColor;

    // 3. 최종 색상 계산 (objectColor 대신 out_Color 사용 가능)
    vec3 result = (ambient + diffuse) * out_Color.rgb;

    FragColor = vec4(result, out_Color.a);
}
