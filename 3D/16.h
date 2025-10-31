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
bool change = false;
int style = GL_TRIANGLES;
int radius_change = 1;
bool depth_test_enabled = true, timer = false, back = false;

// 쓸데가리 없는거
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};
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
    // 이코드 에서 사용할거
	int kind_of_shape = 0;
    float rotatex = 0.0f, rotatey = 0.0f, rotate__;
    glm::quat rotation = glm::quat(1, 0, 0, 0);
    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind, int kos)
        :vertices(ver), indices(ind), kind_of_shape(kos) {
    }
    void reset_state()
    {
        move_xyz = { 0.0f,0.0f ,0.0f };
		rotate_xyz = { 0.0f,0.0f ,1.0f };
		rotateZ = 0.0f;
		rotatex = 0.0f;
		rotatey = 0.0f;
		multy = { 1.0f, 1.0f, 1.0f };
    }
    void set_state(float angle,glm::vec3 rxyz, glm::vec3 mXYZ, glm::vec3 mul)
    {
        move_xyz = mXYZ;
		rotate_xyz = rxyz;
        rotateZ = angle;
        multy = mul;
    }
    void addRotation(float dAngleX, float dAngleY)
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));

        rotation = glm::normalize(qy * qx* rotation );
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
    void matrix_updata() {
        glm::mat4 R = glm::mat4_cast(rotation);
        glm::mat4 T = get_T_Matrix(move_xyz);
        glm::mat4 S = get_S_Matrix(multy);
        glm::mat4 modelMatrix = T * R * S; // TRS 순서로 행렬 곱셈
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "uModel");
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

        glBindVertexArray(0);
    }
    void Update() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }

    void Draw(int style)
    {
        glBindVertexArray(vao);
        
        if (style == GL_LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
    Line(vector<vector<Vertex>> ver) {
        for (int i = 0;i < 3;i++) {
            vertices.push_back(ver[i][0]);
            vertices.push_back(ver[i][1]);
        }
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
    void View_matrix_updata() {
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(position, target, up);
        GLuint viewLoc = glGetUniformLocation(shaderProgramID, "uView");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    }
    void Projection_matrix_updata() {
        aspect = (float)width / (float)height;
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        if (aspect <= 0.0f) {
            aspect = 1.0f; // 기본값으로 설정
        }
        projectionMatrix = glm::perspective(
            glm::radians(fovy), //시야각
            aspect,  // 종횡비
            n, f);// near, far
        GLuint projLoc = glGetUniformLocation(shaderProgramID, "uProj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
};


Shape cube = {
    // 24개의 정점 선언 (위치는 8개, 색상 때문에 24개로 복제)
    {
        // ----------------- 앞면 (RED) ------------------
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 0
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 1
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 2
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 3

        // ----------------- 뒷면 (GREEN) -----------------
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 4
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 5
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 6
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 7

        // ----------------- 윗면 (BLUE) ------------------
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 8 (위치 V3 복제)
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 9 (위치 V2 복제)
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 10 (위치 V6 복제)
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 11 (위치 V7 복제)

        // ... 나머지 3개 면도 같은 방식으로 고유 색상 부여 후 4개씩 복제 ...

        // ----------------- 아랫면 (CYAN) ----------------
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 12
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 13
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 14
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 15

        // ----------------- 왼쪽 면 (MAGENTA) ---------------
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 16
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 17
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 18
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 19

        // ----------------- 오른쪽 면 (YELLOW) --------------
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 20
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 21
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 22
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }  // 23
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
        // 앞면
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },  // 꼭대기
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },  // 앞 왼쪽
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },  // 앞 오른쪽

        // 왼쪽면
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },  // 꼭대기
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },  // 뒤 왼쪽
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },  // 앞 왼쪽

        // 뒤쪽면
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },  // 꼭대기
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },  // 뒤 오른쪽
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },  // 뒤 왼쪽

        // 오른쪽면
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },  // 꼭대기
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },  // 앞 오른쪽
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },  // 뒤 오른쪽

        // 밑면 (두 삼각형)
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) },

        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.0f, 0.5f, 1.0f) }
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
vector<vector<Vertex>> line_list = {
    { // Y는 레드
        { glm::vec3(0, 3, 0), glm::vec4(1, 0, 0, 1) },
        { glm::vec3(0,-3, 0), glm::vec4(1, 0, 0, 1) }
    },
    { // X는 그린
        { glm::vec3(3, 0, 0), glm::vec4(0, 1, 0, 1) },
        { glm::vec3(-3, 0, 0), glm::vec4(0, 1, 0, 1) }
    },
    { //Z는 블루
        { glm::vec3(0, 0, 3), glm::vec4(0, 0, 1, 1) },
        { glm::vec3(0, 0,-3), glm::vec4(0, 0, 1, 1) }
    }
};
Line line(line_list);
Camera camera({ 2.0f,2.0f,2.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });