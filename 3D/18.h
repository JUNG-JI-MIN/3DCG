#pragma once
#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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
bool jaY = false, jaX = false;
bool gong = false;
bool scales = false, onescale = false;
bool change = false;

bool animate_t = false;  // t 키 애니메이션 진행 중
bool animate_u = false;  // u 키 애니메이션 진행 중
bool animate_z = false;  // z 키 애니메이션 진행 중
int ani_count = 0;          // 애니메이션 진행 카운트
int step = 0;
// 쓸데가리 없는거
struct Vertex {
    glm::vec3 position;
    glm::vec4 color; 
};
struct Total {
    Vertex vertex;
    unsigned int index;
};
bool loadOBJ_simple(const std::string& filename,
    std::vector<Vertex>& out_vertices,
    std::vector<unsigned int>& out_indices)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::vector<glm::vec3> temp_positions;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {  // 정점
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "f") {  // 면
            std::vector<unsigned int> face_indices;
            std::string vertStr;
            while (ss >> vertStr) {
                // 정점 인덱스만 쓰고 나머지는 무시
                int vi = std::stoi(vertStr) - 1;  // OBJ는 1부터 시작
                face_indices.push_back(vi);
            }

            // 삼각형으로 쪼개기 (quad면 0-1-2, 0-2-3)
            for (size_t i = 1; i + 1 < face_indices.size(); ++i) {
                out_indices.push_back(face_indices[0]);
                out_indices.push_back(face_indices[i]);
                out_indices.push_back(face_indices[i + 1]);
            }
        }
    }

    // 정점 배열 채우기
    for (auto& pos : temp_positions) {
        Vertex v;
        v.position = pos;
        v.color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f); // 임의 색상
        out_vertices.push_back(v);
    }

    return true;
}
class Shape {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
    glm::vec3 xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    // 이코드 에서 사용할거
    glm::quat ja_rotation = glm::quat(1, 0, 0, 0);
    glm::quat gong_rotation = glm::quat(1, 0, 0, 0);
	glm::vec3 onemul = { 1.0f, 1.0f, 1.0f };
    bool able =true;
    float d_ja_angleX = 5.0f, d_ja_angleY = 5.0f;
	float d_gong_angle = 5.0f;
	float d_onemul = 0.1f;
	float d_scale = 0.1f;
    int kind = 0;
	float speed = 0.05f;
	glm::vec3 vang = { 0.0f,0.0f,0.0f };
	float angle = 0.0f;
    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind,int Kind)
        :vertices(ver), indices(ind), kind(Kind) {}

    void set_state(glm::vec3 XYZ, glm::vec3 mul)
    {
        xyz = XYZ;
        multy = mul;
    }
    void reset_state()
    {   
        if (kind == -1) xyz = { -3.0f,0.0f ,0.0f };
		else xyz = { 3.0f,0.0f ,0.0f };
        multy = { 1.0f, 1.0f, 1.0f };
        ja_rotation = glm::quat(1, 0, 0, 0);
        gong_rotation = glm::quat(1, 0, 0, 0);
        onemul = { 1.0f, 1.0f, 1.0f };
		able = true;
	}
    void add_XYZ(float dx, float dy, float dz)
    {
        xyz.x += dx;
        xyz.y += dy;
        xyz.z += dz;
	}
    void add_gongRotation(float dAngle)
    {
        glm::quat q = glm::angleAxis(glm::radians(dAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        gong_rotation = glm::normalize(q * gong_rotation);
    }
    void add_onemult(float dScale)
    {
        onemul.x += dScale;
        onemul.y += dScale;
        onemul.z += dScale;
        if (onemul.x < 0.1f) onemul = { 0.1f,0.1f,0.1f };
		else if (onemul.x > 5.0f) onemul = { 5.0f,5.0f,5.0f };
	}
    void add_scale(float dScale)
    {
        multy.x += dScale;
        multy.y += dScale;
        multy.z += dScale;
        if (multy.x < 0.1f) multy = { 0.1f,0.1f,0.1f };
		else if (multy.x > 5.0f) multy = { 5.0f,5.0f,5.0f };
    }
    void ja_addRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));
        glm::quat qz = glm::angleAxis(glm::radians(dAngleZ), glm::vec3(0, 0, 1));
        ja_rotation = glm::normalize(qz * qy * qx * ja_rotation);
    }

    glm::mat4 getModelMatrix()
    {   
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, onemul); // 원점 확대 축소
        model = model * glm::mat4_cast(gong_rotation); // 공전
        model = glm::translate(model, xyz); // 위치 이동
        model = model * glm::mat4_cast(ja_rotation); // 자전
        model = glm::scale(model, multy); // 제자기 확대 축소
        return model;
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
    glm::mat4 getModelMatrix()
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, xyz);
        model = glm::scale(model, multy);
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
void result_matrix(Camera camera, Shape shape) {
	glm::mat4 uModel = shape.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_updata();
    glm::mat4 uProj = camera.Projection_matrix_updata();
	glm::mat4 modelMatrix = uProj * uView * uModel;
    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
void result_line_matrix(Camera camera, Line line) {
    glm::mat4 uModel = line.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_updata();
    glm::mat4 uProj = camera.Projection_matrix_updata();
    glm::mat4 modelMatrix = uProj * uView * uModel;
    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

vector<vector<Vertex>> line_list = {
    { // Y는 레드
        { glm::vec3(0, 10, 0), glm::vec4(1, 0, 0, 1) },
        { glm::vec3(0,-10, 0), glm::vec4(1, 0, 0, 1) }
    },
    { // X는 그린
        { glm::vec3(10, 0, 0), glm::vec4(0, 1, 0, 1) },
        { glm::vec3(-10, 0, 0), glm::vec4(0, 1, 0, 1) }
    },
    { //Z는 블루
        { glm::vec3(0, 0, 10), glm::vec4(0, 0, 1, 1) },
        { glm::vec3(0, 0,-10), glm::vec4(0, 0, 1, 1) }
    }
};
Line line(line_list);
Camera camera({ 4.0f,6.0f,4.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
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
    },-1
};
Shape pyramid = {
    {
        // 앞면 - 부드러운 핑크 톤
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(1.0f, 0.8f, 0.9f, 1.0f) },  // 꼭대기
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.7f, 0.85f, 1.0f) },  // 왼쪽
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.95f, 0.6f, 0.8f, 1.0f) },  // 오른쪽

        // 왼쪽면 - 민트/그린 톤
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.7f, 1.0f, 0.9f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.6f, 0.95f, 0.85f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.5f, 0.9f, 0.8f, 1.0f) },

        // 뒤쪽면 - 하늘색 톤
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.7f, 0.85f, 1.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.6f, 0.8f, 0.95f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.75f, 0.9f, 1.0f) },

        // 오른쪽면 - 라벤더/보라 톤
        { glm::vec3(0.0f,  0.5f,  0.0f), glm::vec4(0.85f, 0.7f, 1.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.8f, 0.6f, 0.95f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.75f, 0.55f, 0.9f, 1.0f) },

        // 밑면 (두 삼각형) - 부드러운 옐로/살구 톤
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.95f, 0.8f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.95f, 0.9f, 0.75f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.9f, 0.85f, 0.7f, 1.0f) },

        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.95f, 0.8f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.9f, 0.85f, 0.7f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.95f, 0.9f, 0.75f, 1.0f) }
    },
    {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17
    },1
};
Shape prism = {
    // 18개의 정점 선언 (6개 정점 * 3면 = 18개 정점 + 밑면 2개)
    {
        // ----------------- 앞면 (PINK) ------------------
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.7f, 0.8f, 1.0f) }, // 0: 좌하
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.7f, 0.8f, 1.0f) }, // 1: 우하
        { glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(1.0f, 0.7f, 0.8f, 1.0f) }, // 2: 상

        // ----------------- 뒷면 (LAVENDER) -----------------
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.8f, 0.7f, 1.0f, 1.0f) }, // 3: 우하
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.8f, 0.7f, 1.0f, 1.0f) }, // 4: 좌하
        { glm::vec3(0.0f,0.5f, -0.5f), glm::vec4(0.8f, 0.7f, 1.0f, 1.0f) }, // 5: 상

        // ----------------- 왼쪽 사각형 (GREEN) ------------------
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) }, // 6: 좌하_뒷
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) }, // 7: 좌하_앞
        { glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) }, // 8: 상_앞
        { glm::vec3(0.0f, 0.5f, -0.5f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) }, // 9: 상_뒷

        // ----------------- 오른쪽 사각형 (YELLOW) -----------------
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 0.5f, 1.0f) }, // 10: 우하_앞
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.5f, 1.0f) }, // 11: 우하_뒷
        { glm::vec3(0.0f, 0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.5f, 1.0f) }, // 12: 상_뒷
        { glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 0.5f, 1.0f) }, // 13: 상_앞

        // ----------------- 아랫면 (CYAN) -------------------
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f) }, // 14: 좌하_앞
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f) }, // 15: 좌하_뒷
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f) }, // 16: 우하_뒷
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f) } // 17: 우하_앞
    },
    // 18개 인덱스 (총 6개 삼각형: 앞1, 뒤1, 옆2, 밑2)
    {
        // Front (1 삼각형)
        0, 1, 2,

        // Back (1 삼각형) - 반시계 방향
        3, 4, 5,

        // Left Side (2 삼각형: 6, 7, 8, 9)
        6, 7, 8,
        8, 9, 6,

        // Right Side (2 삼각형: 10, 11, 12, 13)
        10, 11, 12,
        12, 13, 10,

        // Bottom (2 삼각형: 14, 15, 16, 17)
        14, 15, 16,
        16, 17, 14
    },
    -1
};
Shape frustum = {
    // 24개 정점 선언 (이 부분은 수정할 필요 없습니다.)
    {
        // ----------------- 윗면 (GRAY) ------------------ (0.3x0.3)
        { glm::vec3(-0.3f, 0.5f, 0.3f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) }, // 0: 좌상_앞
        { glm::vec3(0.3f, 0.5f, 0.3f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) }, // 1: 우상_앞
        { glm::vec3(0.3f, 0.5f, -0.3f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) }, // 2: 우상_뒷
        { glm::vec3(-0.3f, 0.5f, -0.3f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) }, // 3: 좌상_뒷

        // ----------------- 아랫면 (BROWN) ----------------- (0.5x0.5)
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.5f, 0.3f, 0.1f, 1.0f) }, // 4: 좌하_앞
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.5f, 0.3f, 0.1f, 1.0f) }, // 5: 우하_앞
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.3f, 0.1f, 1.0f) }, // 6: 우하_뒷
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.3f, 0.1f, 1.0f) }, // 7: 좌하_뒷

        // ----------------- 앞면 사다리꼴 (ORANGE) ------------------
        { glm::vec3(-0.3f, 0.5f, 0.3f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) }, // 8: 윗면_좌
        { glm::vec3(0.3f, 0.5f, 0.3f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) }, // 9: 윗면_우
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) }, // 10: 밑면_우
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) }, // 11: 밑면_좌

        // ----------------- 오른쪽 사다리꼴 (RED) ------------------
        { glm::vec3(0.3f, 0.5f, 0.3f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f) }, // 12
        { glm::vec3(0.3f, 0.5f, -0.3f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f) }, // 13
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f) }, // 14
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f) }, // 15

        // ----------------- 뒷면 사다리꼴 (GREEN) ------------------
        { glm::vec3(0.3f, 0.5f, -0.3f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f) }, // 16
        { glm::vec3(-0.3f, 0.5f, -0.3f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f) }, // 17
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f) }, // 18
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f) }, // 19

        // ----------------- 왼쪽 사다리꼴 (BLUE) ------------------
        { glm::vec3(-0.3f, 0.5f, -0.3f), glm::vec4(0.2f, 0.2f, 1.0f, 1.0f) }, // 20
        { glm::vec3(-0.3f, 0.5f, 0.3f), glm::vec4(0.2f, 0.2f, 1.0f, 1.0f) }, // 21
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.2f, 0.2f, 1.0f, 1.0f) }, // 22
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.2f, 0.2f, 1.0f, 1.0f) } // 23
    },
    // 36개 인덱스
    {
        // 1. Top (0, 1, 2, 3) - CCW: (0, 3, 2) / (0, 2, 1)
        0, 1, 2, 
        0, 2, 3, 

        // 2. Bottom (4, 5, 6, 7) - CCW: (4, 7, 6) / (4, 6, 5)
        4, 7, 6,
        4, 6, 5, 

        // 3. Front (8, 9, 10, 11) - CCW: (8, 11, 10) / (8, 10, 9)
        8, 11, 10,
        8, 10, 9,

        // 4. Right (12, 13, 14, 15) - CCW: (12, 15, 14) / (12, 14, 13)
        12, 15, 14,
        12, 14, 13,

        // 5. Back (16, 17, 18, 19) - CCW: (16, 19, 18) / (16, 18, 17)
        16, 19, 18,
        16, 18, 17,

        // 6. Left (20, 21, 22, 23) - CCW: (20, 23, 22) / (20, 22, 21)
        20, 23, 22,
        20, 22, 21
    },
    1
};
std::vector<Shape*> objects;