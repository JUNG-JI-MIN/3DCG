#pragma once
#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#define PI 3.141592f
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
//쓸대가리 없는거
bool qkseofh = false;
bool change = false;
int style = GL_TRIANGLES;
int radius_change = 1;
bool depth_test_enabled = true, timer = true;

float light_angle = 0.0f, d_light_angle = 3.0f;
bool light_rotate = true,light_off = false;
float ring = 1.0f;

bool movement[6]; // 0 : 윗면 돌기, 1: 앞면 열리기, 2: 옆면, 3: 뒷면 커작, 4: 왔다갔다, 5: 하나씩
// 쓸데가리 없는거
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
};
vector<Vertex> create_circle(float r = 1.0f) {
    vector<Vertex> circle_vertices;
    float angle = 0.0f, d_angle = 3.0f;

    while (angle <= 360) {
        float rad = glm::radians(angle);
        float x = r * cos(rad);
        float z = r * sin(rad);
        circle_vertices.push_back({ glm::vec3(x, 0.0f, z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(x, 0, z)) });
        angle += d_angle;
    }
    return circle_vertices;
}
vector<Vertex> create_cube(float x = 1, float y = 1, float z = 1) {
    vector<Vertex> cube_vertices;

    // 편의를 위해 색상을 흰색으로 통일 (조명 효과 확인 용이)
    glm::vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };

    // --- 앞면 (Normal: +Z) ---
    glm::vec3 n_front = { 0.0f, 0.0f, 1.0f };
    cube_vertices.push_back({ {-x, -y,  z}, white, n_front }); // 0
    cube_vertices.push_back({ { x, -y,  z}, white, n_front }); // 1
    cube_vertices.push_back({ { x,  y,  z}, white, n_front }); // 2
    cube_vertices.push_back({ {-x,  y,  z}, white, n_front }); // 3

    // --- 뒷면 (Normal: -Z) ---
    glm::vec3 n_back = { 0.0f, 0.0f, -1.0f };
    cube_vertices.push_back({ {-x, -y, -z}, white, n_back }); // 4
    cube_vertices.push_back({ { x, -y, -z}, white, n_back }); // 5
    cube_vertices.push_back({ { x,  y, -z}, white, n_back }); // 6
    cube_vertices.push_back({ {-x,  y, -z}, white, n_back }); // 7

    // --- 윗면 (Normal: +Y) ---
    glm::vec3 n_top = { 0.0f, 1.0f, 0.0f };
    cube_vertices.push_back({ {-x,  y,  z}, white, n_top }); // 8 (3번 정점)
    cube_vertices.push_back({ { x,  y,  z}, white, n_top }); // 9 (2번 정점)
    cube_vertices.push_back({ { x,  y, -z}, white, n_top }); // 10 (6번 정점)
    cube_vertices.push_back({ {-x,  y, -z}, white, n_top }); // 11 (7번 정점)

    // --- 아랫면 (Normal: -Y) ---
    glm::vec3 n_bottom = { 0.0f, -1.0f, 0.0f };
    cube_vertices.push_back({ {-x, -y,  z}, white, n_bottom }); // 12 (0번 정점)
    cube_vertices.push_back({ { x, -y,  z}, white, n_bottom }); // 13 (1번 정점)
    cube_vertices.push_back({ { x, -y, -z}, white, n_bottom }); // 14 (5번 정점)
    cube_vertices.push_back({ {-x, -y, -z}, white, n_bottom }); // 15 (4번 정점)

    // --- 오른쪽면 (Normal: +X) ---
    glm::vec3 n_right = { 1.0f, 0.0f, 0.0f };
    cube_vertices.push_back({ { x, -y,  z}, white, n_right }); // 16 (1번 정점)
    cube_vertices.push_back({ { x, -y, -z}, white, n_right }); // 17 (5번 정점)
    cube_vertices.push_back({ { x,  y, -z}, white, n_right }); // 18 (6번 정점)
    cube_vertices.push_back({ { x,  y,  z}, white, n_right }); // 19 (2번 정점)

    // --- 왼쪽면 (Normal: -X) ---
    glm::vec3 n_left = { -1.0f, 0.0f, 0.0f };
    cube_vertices.push_back({ {-x, -y,  z}, white, n_left }); // 20 (0번 정점)
    cube_vertices.push_back({ {-x, -y, -z}, white, n_left }); // 21 (4번 정점)
    cube_vertices.push_back({ {-x,  y, -z}, white, n_left }); // 22 (7번 정점)
    cube_vertices.push_back({ {-x,  y,  z}, white, n_left }); // 23 (3번 정점)


    return cube_vertices; // 총 24개의 정점
}
vector<unsigned int> create_cube_index() {
    vector<unsigned int> cube_indices = {
        // 앞면
        0, 1, 2,
        2, 3, 0,
        // 뒷면
        6, 5, 4,
        6, 4, 7,
        // 왼쪽면
        4, 0, 3,
        3, 7, 4,
        // 오른쪽면
        1, 5, 6,
        6, 2, 1,
        // 위면
        3, 2, 6,
        6, 7, 3,
        // 아래면
        4, 5, 1,
        1, 0, 4
    };
    return cube_indices;
}

struct Total {
    Vertex vertex;
    unsigned int index;
};

class Shape {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
    float rotateZ = 0.0f;
    glm::vec3 move_xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 rotate_xyz = { 0.0f,0.0f ,1.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    glm::quat ro = glm::quat(1, 0, 0, 0);
    // 이코드 에서 사용할거
    int kind_of_shape = 0;
    float Trotate = 0.0f, d_Trotate = -0.3f;
    float Frotate = 0.0f, d_Frotate = 0.5f;
    float Srotate = 0.0f, d_Srotate = 0.5f;
    glm::vec3 Bmul = { 1.0f,1.0f,1.0f };
    glm::vec3 d_Bmul = { -0.05f,-0.05f,-0.05f };

    float Orotate = 0.0f, d_Orotate = 0.5f;
    float Rrotate[4] = { 0.0f }, d_Rrotate = 5.0f;
    int pyramid_face = 0;
    float rotatey = 0.0f, rotate__;

    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind, int kos)
        :vertices(ver), indices(ind), kind_of_shape(kos) {
    }
    void ja_addRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));
        glm::quat qz = glm::angleAxis(glm::radians(dAngleZ), glm::vec3(0, 0, 1));
        ro = glm::normalize(qz * qy * qx * ro);
    }
    void reset_state()
    {
        move_xyz = { 0.0f,0.0f ,0.0f };
        rotate_xyz = { 0.0f,0.0f ,1.0f };
        rotateZ = 0.0f;
        rotatey = 0.0f;
        multy = { 1.0f, 1.0f, 1.0f };
        Trotate = 0.0f; d_Trotate = -0.3f;
        Frotate = 0.0f; d_Frotate = 0.5f;
        Srotate = 0.0f; d_Srotate = 0.5f;
        Bmul = { 1.0f,1.0f,1.0f };
        d_Bmul = { -0.05f,-0.05f,-0.05f };
        Orotate = 0.0f; d_Orotate = 0.5f;
        for (int i = 0; i < 4; i++) {
            Rrotate[i] = 0.0f;
        }
        d_Rrotate = 5.0f;
    }
    void set_state(float angle, glm::vec3 rxyz, glm::vec3 mXYZ, glm::vec3 mul)
    {
        move_xyz = mXYZ;
        rotate_xyz = rxyz;
        rotateZ = angle;
        multy = mul;
    }

    glm::mat4 get_T_Matrix(glm::vec3 move_vector)
    {
        //TRS 행렬 계산
        glm::mat4 T = glm::mat4(1.0f);
        T = glm::translate(T, move_vector); //이동 벡터
        return T;
    }
    glm::mat4 get_S_Matrix(glm::vec3 mul)
    {
        //TRS 행렬 계산
        glm::mat4 S = glm::mat4(1.0f);
        S = glm::scale(S, mul); //신축성 벡터
        return S;
    }
    glm::mat4 get_R_Matrix(float angle, glm::vec3 XYZ)
    {
        //TRS 행렬 계산
        glm::mat4 R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(angle), XYZ); //각도, 회전축)
        return R;
    }
    glm::mat4 getModelMatrix()
    {
        //스케일링 회전 이동
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
    }
    void matrix_updata() {
        glm::mat4 T = get_T_Matrix(move_xyz);
        glm::mat4 R = get_R_Matrix(rotatey, { 0.0f, 1.0f, 0.0f });
        glm::mat4 S = get_S_Matrix(multy);
        glm::mat4 modelMatrix = T * R * S; // TRS 순서로 행렬 곱셈
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    }
    void change_face_matrix_updata(int i) {
        glm::mat4 T = get_T_Matrix(move_xyz);
        glm::mat4 R = get_R_Matrix(rotatey, { 0.0f, 1.0f, 0.0f });
        glm::mat4 S = get_S_Matrix(multy);
        glm::mat4 modelMatrix = T * R * S; // TRS 순서로 행렬 곱셈
        if (kind_of_shape == 6) {
            if (i == 0) {
                glm::mat4 T_top = get_T_Matrix({ 0.0f, -0.5f, -0.5f });
                glm::mat4 R_top = get_R_Matrix(Trotate, { 1.0f, 0.0f, 0.0f });
                glm::mat4 T_top_inv = get_T_Matrix({ 0.0f, 0.5f, 0.5f });
                modelMatrix = T * R * S * T_top_inv * R_top * T_top;
            }
            else if (i == 2) {
                glm::mat4 T_top = get_T_Matrix({ 0.0f, -0.5f, 0.0f });
                glm::mat4 R_top = get_R_Matrix(Frotate, { 0.0f, 0.0f, 1.0f });
                glm::mat4 T_top_inv = get_T_Matrix({ 0.0f, 0.5f, 0.0f });
                modelMatrix = T * R * S * T_top_inv * R_top * T_top;
            }
            else if (i == 4) {
                glm::mat4 T_top = get_T_Matrix({ 0.5f, 0.0f, 0.0f });
                glm::mat4 R_top = get_R_Matrix(Srotate, { 1.0f, 0.0f, 0.0f });
                glm::mat4 T_top_inv = get_T_Matrix({ -0.5f, 0.0f, 0.0f });
                modelMatrix = T * R * S * T_top_inv * R_top * T_top;
            }
            else if (i == 5) {
                glm::mat4 T_top = get_T_Matrix({ -0.5f, 0.0f, 0.0f });
                glm::mat4 R_top = get_R_Matrix(Srotate, { 1.0f, 0.0f, 0.0f });
                glm::mat4 T_top_inv = get_T_Matrix({ 0.5f, 0.0f, 0.0f });
                modelMatrix = T * R * S * T_top_inv * R_top * T_top;
            }
            else if (i == 1) {
                glm::mat4 T_top = get_T_Matrix({ 0.0f, 0.0f, 0.5f });
                glm::mat4 S_top = get_S_Matrix(Bmul);
                glm::mat4 T_top_inv = get_T_Matrix({ 0.0f, 0.0f, -0.5f });

                modelMatrix = T * R * T_top_inv * S_top * T_top;
            }
        }
        else if (kind_of_shape == 3) {
            if (movement[4]) {
                if (i == 0) { // 앞
                    glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, -0.5f });
                    glm::mat4 temp_R = get_R_Matrix(Orotate, { 1.0f, 0.0f, 0.0f }); // 축은 그대로 두었습니다
                    glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, 0.5f });
                    modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                }
                else if (i == 1) { // 왼
                    glm::mat4 temp_T = get_T_Matrix({ 0.5f, 0.5f, 0.0f });
                    glm::mat4 temp_R = get_R_Matrix(Orotate, { 0.0f, 0.0f, 1.0f });
                    glm::mat4 temp_T_inv = get_T_Matrix({ -0.5f, -0.5f, 0.0f });
                    modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                }
                else if (i == 2) { // 뒤
                    glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, 0.5f });
                    glm::mat4 temp_R = get_R_Matrix(-Orotate, { 1.0f, 0.0f, 0.0f });
                    glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, -0.5f });
                    modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                }
                else if (i == 3) { // 오
                    glm::mat4 temp_T = get_T_Matrix({ -0.5f, 0.5f, 0.0f });
                    glm::mat4 temp_R = get_R_Matrix(-Orotate, { 0.0f, 0.0f, 1.0f });
                    glm::mat4 temp_T_inv = get_T_Matrix({ +0.5f, -0.5f, 0.0f });
                    modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                }
            }
            else if (movement[5]) {
                if (!qkseofh) {
                    if (i == 0 && pyramid_face >= 0) { // 앞
                        glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, -0.5f });
                        glm::mat4 temp_R = get_R_Matrix(Rrotate[0], { 1.0f, 0.0f, 0.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, 0.5f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 1 && pyramid_face >= 1) { // 왼
                        glm::mat4 temp_T = get_T_Matrix({ 0.5f, 0.5f, 0.0f });
                        glm::mat4 temp_R = get_R_Matrix(Rrotate[1], { 0.0f, 0.0f, 1.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ -0.5f, -0.5f, 0.0f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 2 && pyramid_face >= 2) { // 뒤
                        glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, 0.5f });
                        glm::mat4 temp_R = get_R_Matrix(-Rrotate[2], { 1.0f, 0.0f, 0.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, -0.5f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 3 && pyramid_face >= 3) { // 오
                        glm::mat4 temp_T = get_T_Matrix({ -0.5f, 0.5f, 0.0f });
                        glm::mat4 temp_R = get_R_Matrix(-Rrotate[3], { 0.0f, 0.0f, 1.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.5f, -0.5f, 0.0f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                }
                else {
                    if (i == 0) { // 앞
                        glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, -0.5f });
                        glm::mat4 temp_R = get_R_Matrix(Rrotate[0], { 1.0f, 0.0f, 0.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, 0.5f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 1) { // 왼
                        glm::mat4 temp_T = get_T_Matrix({ 0.5f, 0.5f, 0.0f });
                        glm::mat4 temp_R = get_R_Matrix(Rrotate[1], { 0.0f, 0.0f, 1.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ -0.5f, -0.5f, 0.0f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 2) { // 뒤
                        glm::mat4 temp_T = get_T_Matrix({ 0.0f, 0.5f, 0.5f });
                        glm::mat4 temp_R = get_R_Matrix(-Rrotate[2], { 1.0f, 0.0f, 0.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.0f, -0.5f, -0.5f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                    else if (i == 3) { // 오
                        glm::mat4 temp_T = get_T_Matrix({ -0.5f, 0.5f, 0.0f });
                        glm::mat4 temp_R = get_R_Matrix(-Rrotate[3], { 0.0f, 0.0f, 1.0f });
                        glm::mat4 temp_T_inv = get_T_Matrix({ 0.5f, -0.5f, 0.0f });
                        modelMatrix = T * R * S * temp_T_inv * temp_R * temp_T;
                    }
                }
            }
        }
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
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

        // 정점 속성 설정 (position, color 등)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
    void Update() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }

    void Draw(int style)
    {
        glBindVertexArray(vao);
        change_face_matrix_updata(kind_of_shape);

        if (style == GL_LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    void face_draw() {
        for (int i = 0; i < indices.size() / kind_of_shape; i++) {
            change_face_matrix_updata(i);
            glBindVertexArray(vao);
            if (style == GL_LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, kind_of_shape, GL_UNSIGNED_INT, (void*)(i * kind_of_shape * sizeof(unsigned int)));
        }
    }

    void Delete()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
};

class Line {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    float rotateZ = 3.141592f / 6;
    glm::vec3 xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    Line(vector<Vertex> ver) {
		vertices = ver;
    }
    glm::mat4 get_T_Matrix(glm::vec3 move_vector)
    {
        //TRS 행렬 계산
        glm::mat4 T = glm::mat4(1.0f);
        T = glm::translate(T, move_vector); //이동 벡터
        return T;
    }
    glm::mat4 get_S_Matrix(glm::vec3 mul)
    {
        //TRS 행렬 계산
        glm::mat4 S = glm::mat4(1.0f);
        S = glm::scale(S, mul); //신축성 벡터
        return S;
    }
    glm::mat4 get_R_Matrix(float angle, glm::vec3 XYZ)
    {
        //TRS 행렬 계산
        glm::mat4 R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(rotateZ), XYZ); //각도, 회전축)
        return R;
    }
    void matrix_updata() { //일단은 회전만 적용
        glm::mat4 Ry = get_R_Matrix(rotateZ, { 0.0f,1.0f,0.0f });
        glm::mat4 Rx = get_R_Matrix(rotateZ, { 1.0f,0.0f,0.0f });
        glm::mat4 modelMatrix = Ry * Rx; // TRS 순서로 행렬 곱셈
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "uModel");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }
    glm::mat4 getModelMatrix()
    {
        //스케일링 회전 이동
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, rotateZ, glm::vec3{ 0,1,0 }); // 자전
        model = glm::translate(model, xyz); // 이동하는 거
        model = glm::scale(model, {ring,ring,ring});
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
	}
    void Init()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // 정점 속성 설정 (position, color 등)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
    void Update() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
    void Draw()
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, vertices.size());
    }
    void r_Draw()
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, vertices.size());
        glDrawArrays(GL_LINES, 1, vertices.size()-1);
	}
    void Delete()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

};
class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fovy = 45.0f; // 시양각
    float aspect; // 종횡비 아직 w와 h가 없기 때문에 계산 불가
    float n = 0.1f; // near
    float f = 100.0f; // far
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 u)
        : position(pos), target(tar), up(u) {

        cout << aspect << endl;
    }
    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }
    glm::mat4 View_matrix_updata() {
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(position, target, up);
		return viewMatrix;
    }
    glm::mat4 Projection_matrix_updata() {
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
};
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
    void turn_off() {
        light_color = glm::vec3(0.0f, 0.0f, 0.0f);
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
void result_matrix(Camera camera, Shape shape) {
    glm::mat4 uProj = camera.Projection_matrix_updata();
    glm::mat4 uModel = shape.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_updata();

    GLuint u = glGetUniformLocation(shaderProgramID, "m");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));
    u = glGetUniformLocation(shaderProgramID, "v");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));
    u = glGetUniformLocation(shaderProgramID, "p");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));
}
void result_line_matrix(Camera camera, Line line) {
    glm::mat4 uProj = camera.Projection_matrix_updata();
    glm::mat4 uModel = line.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_updata();

    GLuint u = glGetUniformLocation(shaderProgramID, "m");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));
    u = glGetUniformLocation(shaderProgramID, "v");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));
    u = glGetUniformLocation(shaderProgramID, "p");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));
}
Shape cube = {
    {
        // ----------------- 앞면 (RED) | Normal: +Z ------------------
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 0
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 1
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 2
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 3

        // ----------------- 뒷면 (GREEN) | Normal: -Z -----------------
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 4
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 5
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 6
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 7

        // ----------------- 윗면 (BLUE) | Normal: +Y ------------------
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 8
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 9
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 10
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 11

        // ----------------- 아랫면 (CYAN) | Normal: -Y ----------------
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 12
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 13
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 14
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 15

        // ----------------- 왼쪽 면 (MAGENTA) | Normal: -X ---------------
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 16
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 17
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 18
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 19

        // ----------------- 오른쪽 면 (YELLOW) | Normal: +X --------------
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 20
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 21
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 22
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }  // 23
    },
    // 36개 인덱스 (12개 삼각형)
    {
        // Front (0, 1, 2, 3) -> Red
        0, 1, 2,
        2, 3, 0,

        // Back (4, 5, 6, 7) -> Green
        4, 5, 6,
        6, 7, 4,

        // Top (8, 9, 10, 11) -> Blue
        8, 9, 10,
        10, 11, 8,

        // Bottom (12, 13, 14, 15) -> Cyan
        12, 13, 14,
        14, 15, 12,

        // Left (16, 17, 18, 19) -> Magenta
        16, 17, 18,
        18, 19, 16,

        // Right (20, 21, 22, 23) -> Yellow
        20, 21, 22,
        22, 23, 20
    },
    6
};
Shape pyramid = {
    {
        // ----------------- 앞면 (부드러운 핑크 톤) | Normal: (0.0, 0.447, 0.894) 근사치 ------------------
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(1.0f, 0.8f, 0.9f, 1.0f), glm::vec3(0.0f, 0.447f, 0.894f) },   // 꼭대기
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.7f, 0.85f, 1.0f), glm::vec3(0.0f, 0.447f, 0.894f) },  // 왼쪽
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.95f, 0.6f, 0.8f, 1.0f), glm::vec3(0.0f, 0.447f, 0.894f) },   // 오른쪽

        // ----------------- 왼쪽면 (민트/그린 톤) | Normal: (-0.894, 0.447, 0.0) 근사치 -----------------
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.7f, 1.0f, 0.9f, 1.0f), glm::vec3(-0.894f, 0.447f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.6f, 0.95f, 0.85f, 1.0f), glm::vec3(-0.894f, 0.447f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.5f, 0.9f, 0.8f, 1.0f), glm::vec3(-0.894f, 0.447f, 0.0f) },

        // ----------------- 뒤쪽면 (하늘색 톤) | Normal: (0.0, 0.447, -0.894) 근사치 ------------------
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.7f, 0.85f, 1.0f, 1.0f), glm::vec3(0.0f, 0.447f, -0.894f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.6f, 0.8f, 0.95f, 1.0f), glm::vec3(0.0f, 0.447f, -0.894f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.75f, 0.9f, 1.0f), glm::vec3(0.0f, 0.447f, -0.894f) },

        // ----------------- 오른쪽면 (라벤더/보라 톤) | Normal: (0.894, 0.447, 0.0) 근사치 ----------------
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.85f, 0.7f, 1.0f, 1.0f), glm::vec3(0.894f, 0.447f, 0.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.8f, 0.6f, 0.95f, 1.0f), glm::vec3(0.894f, 0.447f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.75f, 0.55f, 0.9f, 1.0f), glm::vec3(0.894f, 0.447f, 0.0f) },

        // ----------------- 밑면 (두 삼각형) | Normal: -Y ----------------------------------------------------
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.95f, 0.8f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.95f, 0.9f, 0.75f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.9f, 0.85f, 0.7f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },

        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.95f, 0.8f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.9f, 0.85f, 0.7f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.95f, 0.9f, 0.75f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
    },
    {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17
    },
    3
};

vector<Vertex> line_list = {
// Y는 레드
    { glm::vec3(0, 3, 0), glm::vec4(1, 0, 0, 1) },
    { glm::vec3(0,-3, 0), glm::vec4(1, 0, 0, 1) },
// X는 그린
    { glm::vec3(3, 0, 0), glm::vec4(0, 1, 0, 1) },
    { glm::vec3(-3, 0, 0), glm::vec4(0, 1, 0, 1) },
//Z는 블루
    { glm::vec3(0, 0, 3), glm::vec4(0, 0, 1, 1) },
    { glm::vec3(0, 0,-3), glm::vec4(0, 0, 1, 1) }
};
Line line(line_list);
Line camera_line(create_circle(2.0f));
Shape camera_cube(create_cube(0.25f, 0.25f, 0.25f),create_cube_index(),6);

Camera camera({ 5.0f,5.0f,5.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
Light light({ 2.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });