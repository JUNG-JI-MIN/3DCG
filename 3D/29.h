#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION //--- 한 번만 선언
#include <stdlib.h>
#include <stdio.h>  
#include <iostream>
#include <vector>
#include <fstream>     // 파일 읽기 (ifstream)
#include <sstream>     // 문자열 스트림 (stringstream)
#include <string>      // 문자열 처리 (std::string)
#include <cstdlib>     // std::stoi
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtx/rotate_vector.hpp>
#include <stb_image.h>
using namespace std;
//--- 아래 5개 함수는 사용자 정의 함수 임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
//--- 필요한 변수 선언
GLuint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
// 정점에 대한 정의들 위치 색상 법선 텍스쳐좌표
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
    glm::vec2 texcoord;
};
glm::vec4 color{ 0,0,0,1 };
// 도형 생성 함수들 (버텍스와 인덱스 생성) 큐브, 원(이건 Line 클래스로 생성해야함 밑에 있으니 참고), 구
vector<Vertex> create_cube(float x = 1, float y = 1, float z = 1, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) {
    vector<Vertex> cube_vertices = {
        // 앞면 | Normal: +Z | TexCoords: { {0, 0}, {1, 0}, {1, 1}, {0, 1} }
        { {-x, -y,  z}, color, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // 0 (bottom-left)
        { { x, -y,  z}, color, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }, // 1 (bottom-right)
        { { x,  y,  z}, color, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // 2 (top-right)
        { {-x,  y,  z}, color, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // 3 (top-left)

        // 뒷면 | Normal: -Z
        { {-x, -y, -z}, color, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} }, // 4 
        { { x, -y, -z}, color, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} }, // 5 
        { { x,  y, -z}, color, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} }, // 6 
        { {-x,  y, -z}, color, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} }, // 7 

        // 왼쪽면 | Normal: -X
        { {-x, -y, -z}, color, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} }, // 8
        { {-x, -y,  z}, color, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} }, // 9
        { {-x,  y,  z}, color, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} }, // 10
        { {-x,  y, -z}, color, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }, // 11

        // 오른쪽면 | Normal: +X
        { { x, -y, -z}, color, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} }, // 12
        { { x, -y,  z}, color, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} }, // 13
        { { x,  y,  z}, color, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }, // 14
        { { x,  y, -z}, color, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} }, // 15

        // 위면 | Normal: +Y
        { {-x,  y, -z}, color, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }, // 16 
        { { x,  y, -z}, color, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} }, // 17
        { { x,  y,  z}, color, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} }, // 18
        { {-x,  y,  z}, color, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} }, // 19

        // 아래면 | Normal: -Y
        { {-x, -y, -z}, color, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} }, // 20
        { {-x, -y,  z}, color, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} }, // 21
        { { x, -y,  z}, color, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} }, // 22
        { { x, -y, -z}, color, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }  // 23
    };
    return cube_vertices;
}
vector<unsigned int> create_cube_index() {
    vector<unsigned int> cube_indices = {
        // 앞면
        0, 1, 2,
        0, 2, 3,
        // 뒷면
        4, 7, 6,
        4, 6, 5,

        // 왼쪽면
        8, 9, 10,
        8, 10, 11,
        // 오른쪽면
        12, 15, 14,
        12, 14, 13,
        // 위면
        16, 18, 17,
        16, 19, 18,
        // 아래면
        20, 23, 22,
        20, 22, 21
    };
    return cube_indices;
}

vector<Vertex> create_circle(float r = 1.0f) {
    vector<Vertex> circle_vertices;
    float angle = 0.0f, d_angle = 3.0f;

    while (angle <= 360) {
        float rad = glm::radians(angle);
        float x = r * cos(rad);
        float z = r * sin(rad);
        circle_vertices.push_back({ glm::vec3(x, 0.0f, z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
        angle += d_angle;
    }
    return circle_vertices;
}

vector<Vertex> create_sphere(float r = 1.0f, int kind = 0) {
    vector<Vertex> circle_vertices;
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    float uangle = 0.0f, vangle = 0.0f, d_angle = 3.0f;
    while (vangle <= 360) {
        uangle = 0.0f;
        while (uangle <= 360) {
            float u = glm::radians(uangle);
            float v = glm::radians(vangle);
            float x = r * cos(u) * sin(v);
            float y = r * cos(v);
            float z = r * sin(u) * sin(v);
            if (kind == 0) {
                color = { uangle / 360, 1.0f, 1.0f, 1.0f };
            }
            else if (kind == 1) {
                color = { 1.0f, uangle / 360, 1.0f, 1.0f };
            }
            else if (kind == 2) {
                color = { 1.0f, 1.0f,uangle / 360, 1.0f };
            }
            circle_vertices.push_back({ glm::vec3(x, y, z), glm::vec4(color) });
            uangle += d_angle;
        }
        vangle += d_angle;
    }
    return circle_vertices;
}
vector<unsigned int> create_sphere_index(int slices = 120) { // 여기서는 360 / 3으로 진행함 그러므로 120이다. 면의 개수는 level-1이다.
    vector<unsigned int> sphere_index;

    // slices: 경도 분할 수 (j의 최대치)
    // stacks: 위도 분할 수 (i의 최대치)
    const int SLICES = slices;
    const int STACKS = (int)(SLICES / 2.0f); // 360/3=120, 180/3=60. level이 120이면 stacks는 60.

    // 한 층(스택) 당 정점 개수
    const int VERTICES_PER_STACK = SLICES + 1;

    // i (스택 인덱스)는 0부터 STACKS-1까지 반복
    for (int i = 0; i < STACKS; ++i) {
        // j (슬라이스 인덱스)는 0부터 SLICES-1까지 반복
        for (int j = 0; j < SLICES; ++j) {

            // p1: i번째 스택, j번째 슬라이스
            unsigned int p1 = i * VERTICES_PER_STACK + j;
            // p2: i번째 스택, j+1번째 슬라이스
            unsigned int p2 = p1 + 1;
            // p3: i+1번째 스택, j번째 슬라이스
            unsigned int p3 = (i + 1) * VERTICES_PER_STACK + j;
            // p4: i+1번째 스택, j+1번째 슬라이스
            unsigned int p4 = p3 + 1;

            // 삼각형 1: (p1, p3, p2)
            sphere_index.push_back(p1);
            sphere_index.push_back(p3);
            sphere_index.push_back(p2);

            // 삼각형 2: (p2, p3, p4)
            sphere_index.push_back(p2);
            sphere_index.push_back(p3);
            sphere_index.push_back(p4);
        }
    }
    return sphere_index;
}

vector<unsigned int> create_pyramid_index() {
    return {
        // 면 1: 앞면
        0, 1, 2,

        // 면 2: 왼쪽면
        3, 4, 5,

        // 면 3: 뒷면
        6, 7, 8,

        // 면 4: 오른쪽면
        9, 10, 11,

        // 면 5 & 6: 밑면 (Base)
        12, 13, 14, // Base Triangle 1
        15, 16, 17  // Base Triangle 2
    };
}
vector<Vertex> create_pyramid() {
    // 사각뿔 꼭지점 및 밑면 정점 좌표
    const glm::vec3 P_APEX(0.0f, 1.0f, 0.0f);
    const glm::vec3 P_FL(-1.0f, -1.0f, 1.0f); // 앞 왼쪽
    const glm::vec3 P_FR(1.0f, -1.0f, 1.0f); // 앞 오른쪽
    const glm::vec3 P_BL(-1.0f, -1.0f, -1.0f); // 뒤 왼쪽
    const glm::vec3 P_BR(1.0f, -1.0f, -1.0f); // 뒤 오른쪽

    // 계산된 노멀 벡터 (정규화된 근사값)
    const glm::vec3 N_FRONT = { 0.0f, 0.7071f, 0.7071f };
    const glm::vec3 N_LEFT = { -0.7071f, 0.7071f, 0.0f };
    const glm::vec3 N_BACK = { 0.0f, 0.7071f, -0.7071f };
    const glm::vec3 N_RIGHT = { 0.7071f, 0.7071f, 0.0f };
    const glm::vec3 N_BASE = { 0.0f, -1.0f, 0.0f };

    // UV 경계값
    const float U0 = 0; // 맨 오른쪽
    const float V0 = 0; // 맨 아래
    const float U1 = 0.333333f; 
    const float U2 = 0.666667f;
    const float V1 = 0.333333f;
    const float V2 = 0.666667f;

    // UV 중앙값
    const float U_MID = 0.5f;
    const float V_MID = 0.5f;

    // 미드포인트 계산
    const float U_TOP_MID = (U1 + U2) * 0.5f;
    const float V_TOP_MID = (V2 + 1.0f) * 0.5f;

    const float U_LEFT_MID = (U0 + U1) * 0.5f;
    const float V_LEFT_MID = (V1 + V2) * 0.5f;

    const float U_RIGHT_MID = (U2 + 1.0f) * 0.5f;
    const float V_RIGHT_MID = (V1 + V2) * 0.5f;

    const float U_BOTTOM_MID = (U1 + U2) * 0.5f;
    const float V_BOTTOM_MID = (0.0f + V1) * 0.5f;

    vector<Vertex> pyramid_vertices = {

        // =========================================================================
        // 1. 앞면 (+Z) - Top 삼각형
        { {0.0f, 1.0f, 0.0f}, color, N_FRONT, {U_TOP_MID, 1.0f} }, // Apex
        { {1.0f, -1.0f, 1.0f},  color, N_FRONT, {U2, V_TOP_MID} }, // Right-mid
        { {-1.0f, -1.0f, 1.0f}, color, N_FRONT, {U1, V_TOP_MID} }, // Left-mid

        // =========================================================================
        // 2. 왼쪽면 (-X) - Left 삼각형
        { {0.0f, 1.0f, 0.0f}, color, N_LEFT, {0.0f, V_LEFT_MID} },  // Apex(left)
        { {-1.0f, -1.0f, -1.0f}, color, N_LEFT, {U_LEFT_MID, V2} }, // Top-mid
        { {-1.0f, -1.0f, 1.0f},  color, N_LEFT, {U_LEFT_MID, V1} }, // Bottom-mid

        // =========================================================================
        // 3. 뒷면 (-Z) - Bottom 삼각형
        { {0.0f, 1.0f, 0.0f}, color, N_BACK, {U_BOTTOM_MID, 0.0f} }, // Apex(bottom)
        { {1.0f, -1.0f, -1.0f},  color, N_BACK, {U2, V_BOTTOM_MID} }, // Right-mid
        { {-1.0f, -1.0f, -1.0f}, color, N_BACK, {U1, V_BOTTOM_MID} }, // Left-mid

        // =========================================================================
        // 4. 오른쪽면 (+X) - Right 삼각형
        { {0.0f, 1.0f, 0.0f}, color, N_RIGHT, {1.0f, V_RIGHT_MID} }, // Apex(right)
        { {1.0f, -1.0f, 1.0f},  color, N_RIGHT, {U2, V1} }, // Bottom-mid
        { {1.0f, -1.0f, -1.0f}, color, N_RIGHT, {U2, V2} }, // Top-mid

        // =========================================================================
        // 5. 밑면 1 (사각형의 절반)
        { {1.0f, -1.0f, 1.0f},  color, N_BASE, {U2, V1} },
        { {-1.0f, -1.0f, 1.0f}, color, N_BASE, {U1, V1} },
        { {-1.0f, -1.0f, -1.0f},color, N_BASE, {U1, V2} },

        // =========================================================================
        // 6. 밑면 2
        { {1.0f, -1.0f, 1.0f},  color, N_BASE, {U2, V1} },
        { {-1.0f, -1.0f, -1.0f},color, N_BASE, {U1, V2} },
        { {1.0f, -1.0f, -1.0f}, color, N_BASE, {U2, V2} }
    };

    return pyramid_vertices;
}
//랜덤 실수 만드는 건데 이걸 쓸지 모르겠다.
float random(float min = 0.0f, float max = 1.0f) {
    // 0.0 ~ 1.0 사이의 정규화된 난수 생성
    float randomValue = (float)rand() / (float)RAND_MAX;
    // min ~ max 범위로 스케일링
    float result = min + randomValue * (max - min);
    if (result == 0) result = 0.001;
    return result;
}

// 카메라 클래스 안에 투명 및 뷰 매트릭스 업데이트 함수 포함
class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fovy = 45.0f; // 시양각
    float aspect; // 종횡비 아직 w와 h가 없기 때문에 계산 불가
    float n = 0.1f; // near
    float f = 150.0f; // far
    float orthoScale = 10.0f; // 직각 투영 범위
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 u)
        : position(pos), target(tar), up(u) {

        cout << aspect << endl;
    }
    void reset() {
        position = { 0.0f,25.0f,30.0f };
        target = { 0.0f,0.0f,0.0f };
        up = { 0.0f,1.0f,0.0f };
    }
    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }
    glm::mat4 View_matrix_update() {
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(position, target, up);
        return viewMatrix;
    }
    glm::mat4 Projection_matrix_update() {
        aspect = (float)width / (float)height;
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        if (aspect <= 0.0f) {
            aspect = 1.0f; // 기본값으로 설정
        }
        projectionMatrix = glm::perspective(
            glm::radians(fovy), //시야각
            aspect,  // 종횡비
            n, f);// near, far
        return projectionMatrix;
    }
    glm::mat4 Orthographic_matrix_update() {
        aspect = (float)width / (float)height;
        if (aspect <= 0.0f) aspect = 1.0f;
        return glm::ortho(-orthoScale * aspect, orthoScale * aspect,
            -orthoScale, orthoScale, n, f);
    }
};

// 조명 클래스
class Light {
public:
    glm::vec3 position;
    glm::vec3 light_color;
    glm::vec3 viewpos;
    Light(glm::vec3 pos, glm::vec3 lc)
        : position(pos), light_color(lc) {
    }
    void Init() {
        aplly_position();
        apply_color();
    }
    void apply_color() {
        GLuint u = glGetUniformLocation(shaderProgramID, "lightColor");
        glUniform3fv(u, 1, glm::value_ptr(light_color));
    }
    void aplly_position() {
        GLuint u = glGetUniformLocation(shaderProgramID, "lightPos");
        glUniform3fv(u, 1, glm::value_ptr(position));
    }
};
Light light({ 0.0f,5.0f,5.0f }, { 1.0f,1.0f,1.0f });
Camera camera({ 0.0f,3.0f,3.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
float x = 1, y = 1, z = 1;

float u_1 = 0,u_2 = 0.333333f,u_3 = 0.666666f, u_4 = 1;
float v_1 = 0, v_2 = 0.333333f, v_3 = 0.666666f, v_4 = 1;
// 추상 클래스 - 기본적으로 렌더링만 당담하는 클래스임 아무 게임 로직도 없잉 렌더링 하는 것만 담당
class Model {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스

    Model(const vector<Vertex>& ver, const vector<unsigned int>& ind)
        :vertices(ver), indices(ind) {
    }

    void Init()
    {
        std::cout << "Init() called: vertices=" << vertices.size()
            << ", indices=" << indices.size() << std::endl;

        if (vertices.empty() || indices.empty()) {
            std::cerr << "ERROR: Empty vertex or index data!" << std::endl;
            return;
        }
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // 정점 속성 설정 (position, color, nomor)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }
    void Update() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
    void Draw()
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    void Delete()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
};
class Texture {
public:
    GLuint id;
    int width, height;
    int channels;

    void Load(const char* filePath) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "ERROR: Failed to load texture: " << filePath << std::endl;
            return;
        }
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // 채널 수에 따라 포맷 결정
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // 텍스처 파라미터 설정 (필수!)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 밉맵 생성
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0); // 언바인드
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }
};
Model public_cube({
    
    // 앞면 | Normal: +Z | TexCoords: { {0, 0}, {1, 0}, {1, 1}, {0, 1} }
    { {-x, -y,  z}, color, {0.0f, 0.0f, 1.0f}, {u_1, v_2} }, // 0 (bottom-left)
    { { x, -y,  z}, color, {0.0f, 0.0f, 1.0f}, {u_2, v_2} }, // 1 (bottom-right)
    { { x,  y,  z}, color, {0.0f, 0.0f, 1.0f}, {u_2, v_3} }, // 2 (top-right)
    { {-x,  y,  z}, color, {0.0f, 0.0f, 1.0f}, {u_1, v_3} }, // 3 (top-left)

    // 뒷면 | Normal: -Z
    { {-x, -y, -z}, color, {0.0f, 0.0f, -1.0f}, {u_4, v_2} }, // 4 
    { { x, -y, -z}, color, {0.0f, 0.0f, -1.0f}, {u_3, v_2} }, // 5 
    { { x,  y, -z}, color, {0.0f, 0.0f, -1.0f}, {u_3, v_3} }, // 6 
    { {-x,  y, -z}, color, {0.0f, 0.0f, -1.0f}, {u_4, v_3} }, // 7 

    // 왼쪽면 | Normal: -X
    { {-x, -y, -z}, color, {-1.0f, 0.0f, 0.0f}, {u_2, v_1} }, // 8
    { {-x, -y,  z}, color, {-1.0f, 0.0f, 0.0f}, {u_3, v_1} }, // 9
    { {-x,  y,  z}, color, {-1.0f, 0.0f, 0.0f}, {u_3, v_2} }, // 10
    { {-x,  y, -z}, color, {-1.0f, 0.0f, 0.0f}, {u_2, v_2} }, // 11

    // 오른쪽면 | Normal: +X
    { { x, -y, -z}, color, {1.0f, 0.0f, 0.0f}, {u_2, v_3} }, // 12
    { { x, -y,  z}, color, {1.0f, 0.0f, 0.0f}, {u_3, v_3} }, // 13
    { { x,  y,  z}, color, {1.0f, 0.0f, 0.0f}, {u_3, v_4} }, // 14
    { { x,  y, -z}, color, {1.0f, 0.0f, 0.0f}, {u_2, v_4} }, // 15

    // 위면 | Normal: +Y
    { {-x,  y, -z}, color, {0.0f, 1.0f, 0.0f}, {u_2, v_2} }, // 16 
    { { x,  y, -z}, color, {0.0f, 1.0f, 0.0f}, {u_3, v_2} }, // 17
    { { x,  y,  z}, color, {0.0f, 1.0f, 0.0f}, {u_3, v_3} }, // 18
    { {-x,  y,  z}, color, {0.0f, 1.0f, 0.0f}, {u_2, v_3} }, // 19

    // 아래면 | Normal: -Y
    { {-x, -y, -z}, color, {0.0f, -1.0f, 0.0f}, {u_2, v_2} }, // 20
    { {-x, -y,  z}, color, {0.0f, -1.0f, 0.0f}, {u_2, v_3} }, // 21
    { { x, -y,  z}, color, {0.0f, -1.0f, 0.0f}, {u_3, v_3} }, // 22
    { { x, -y, -z}, color, {0.0f, -1.0f, 0.0f}, {u_3, v_2} }  // 23
    },
    create_cube_index()
); // 전역 변수로 큐브 모델 생성
Model pyramidse(create_pyramid(), create_pyramid_index());
Texture player_cube_texture; // 플레이어 1
Texture pyramid;


// 게임 오브젝트는 게임 로직을 포함하는 객체임 여기서 다른 자식들로 파생되면서 게임 로직이 추가될 수 있음
class GameObject { // 게임 오브젝트는 게임 로직을 포함하는 객체임 여기서 다른 자식들로 파생되면서 게임 로직이 추가될 수 있음
public:
    glm::vec3 position = glm::vec3(0.0f); // 위치
    glm::vec3 velocity = glm::vec3(0.0f); // 속도
    glm::vec3 multy = glm::vec3(1.0f, 1.0f, 1.0f); // 크기 
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // 회전 (쿼터니언)
    Model* model = nullptr; // 모델 포인터
    Texture* texture = nullptr; // 텍스처 포인터

    GameObject(glm::vec3 pos)
        :position(pos) {
    }

    void model_init(const vector<Vertex>& v, const vector<unsigned int>& i) {
        model->vertices = v;
        model->indices = i;
    }

    void addRotation(float dAngleX, float dAngleY, float dAngleZ) // 오일러 각도로 회전 추가 예를 들어 (30, 0, 0) 은 x축으로 30도 회전
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));
        glm::quat qz = glm::angleAxis(glm::radians(dAngleZ), glm::vec3(0, 0, 1));
        rotation = glm::normalize(qz * qy * qx * rotation);
    }

    void add_position(float dx, float dy, float dz) { // 위치 변경 예를 들어 (1, 0, 0) 은 x축으로 1만큼 이동
        position.x += dx;
        position.y += dy;
        position.z += dz;
    }

    // 텍스처와 모델을 로드하고 초기화하는 함수
    void InitializeRendering(Model* sharedModel, Texture* tex) {
        // 1. 모델 설정
        model = sharedModel;
        // 2. 텍스처 설정
        texture = tex;
    }

    void Draw() {
        if (!model) return;
        // 텍스처가 있다면 바인딩
        if (texture) {
            texture->Bind();
            // 텍스처 유니폼 위치 설정 (셰이더의 sampler2D 이름: "textureSampler" 가정)
            GLuint uTex = glGetUniformLocation(shaderProgramID, "textureSampler");
            glUniform1i(uTex, 0); // GL_TEXTURE0 유닛을 사용하도록 설정
        }
        // 모델 렌더링
        model->Draw();
    }

    virtual void Update(float dt) {}
    virtual void OnCollision(GameObject* other) {}

    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model *= glm::mat4_cast(rotation);
        model = glm::scale(model, multy);
        return model;
    }
    void result_matrix(Camera& camera) {
        glm::mat4 modelMatrix;
        glm::mat4 uProj = camera.Projection_matrix_update();
        glm::mat4 uModel = getModelMatrix();
        glm::mat4 uView = camera.View_matrix_update();
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(uModel)));

        GLuint u = glGetUniformLocation(shaderProgramID, "m");
        glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));

        u = glGetUniformLocation(shaderProgramID, "v");
        glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));

        u = glGetUniformLocation(shaderProgramID, "p");
        glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));


        u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

};
GameObject player(glm::vec3{0,0,0});