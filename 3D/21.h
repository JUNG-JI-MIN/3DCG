#pragma once
#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#define GLM_ENABLE_EXPERIMENTAL
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
#include "tank_model.h"
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
///dsadasdsa
bool atime = true;
bool yrotate = false;
bool mouse_rotate = false;
float dy_rotate = 2.0f;
float d_angle = 0.0f;
float world_angle = 0.0f;
float startx = 0.0f;
bool opened = false;
vector<Vertex> create_sphere(float r = 1.0f) {
    vector<Vertex> circle_vertices;
    glm::vec4 color = { 0.0f, 1.0f, 1.0f, 1.0f };
    float uangle = 0.0f, vangle = 0.0f, d_angle = 10.0f;
    while (vangle <= 360) {
        uangle = 0.0f;
        while (uangle <= 360) {
            float u = glm::radians(uangle);
            float v = glm::radians(vangle);
            float x = r * cos(u) * sin(v);
            float y = r * cos(v);
            float z = r * sin(u) * sin(v);
            circle_vertices.push_back({ glm::vec3(x, y, z), glm::vec4(color) });
            uangle += d_angle;
        }
        vangle += d_angle;
    }
    return circle_vertices;
}
vector<unsigned int> create_sphere_index(int slices = 36) { // 여기서는 360 / 3으로 진행함 그러므로 120이다. 면의 개수는 level-1이다.
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
vector<Vertex> create_cube(float x = 1,float y = 1, float z = 1) {
    vector<Vertex> cube_vertices = {
        // 앞면
        {{-x, -y,  z}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 0
        {{ x, -y,  z}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 1
        {{ x,  y,  z}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 2
        {{-x,  y,  z}, {1.0f, 1.0f, 0.0f, 1.0f}}, // 3
        // 뒷면
		{{-x, -y, -z}, {1.0f, 0.0f, 1.0f, 1.0f}}, // 4
		{{ x, -y, -z}, {0.0f, 1.0f, 1.0f, 1.0f}}, // 5
		{{ x,  y, -z}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 6
		{{-x,  y, -z}, {0, 0, 0, 0}}, // 7
	};
	return cube_vertices;
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
//여기까지가 이 코드에서만 필요한거
float random(float min = 0.0f, float max = 1.0f) {
    // 0.0 ~ 1.0 사이의 정규화된 난수 생성
    float randomValue = (float)rand() / (float)RAND_MAX;
    // min ~ max 범위로 스케일링
    float result = min + randomValue * (max - min);
    if (result == 0) result = 0.001;
    return result;
}
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
};

Camera camera({ 0.0f,0.0f,15.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
class Shape {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
    glm::vec3 move_xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    glm::quat ro = glm::quat(1, 0, 0, 0);
    // 이코드 에서 사용할거
    float open_angle = 0.0f , d_open_angle = -0.1f;
    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind, glm::vec3 a)
        :vertices(ver), indices(ind), move_xyz(a) {
    }
    void reset_state()
    {
        move_xyz = { 0.0f,0.0f ,0.0f };
        ro = glm::quat(1, 0, 0, 0);
        multy = { 1.0f,1.0f ,1.0f };
    }
    void Rotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));
        glm::quat qz = glm::angleAxis(glm::radians(dAngleZ), glm::vec3(0, 0, 1));
        ro = glm::normalize(qz * qy * qx * ro);
    }
    void add_XYZ(float dx, float dy, float dz)
    {
        move_xyz.x += dx;
        move_xyz.y += dy;
        move_xyz.z += dz;
    }
    void open_Draw() {
        if (open_angle <= -70.0f) return;
        open_angle += d_open_angle; // 각도를 라디안으로 증가

        double angle = glm::radians(open_angle);

        // 기준 축
        const double cx = -3.0;
        const double cy = -3.0;

        for (int i = 12; i < 16; i++) {

            glm::vec3& v = vertices[i].position;

            // 1) 회전축 기준으로 이동
            double tx = v.x - cx;
            double ty = v.y - cy;

            // 2) 회전 (z축 기준)
            double rx = tx * cos(angle);
            double ry = tx * sin(angle);

            // 3) 원래 위치로 이동
            v.x = rx + cx;
            v.y = ry + cy;
            // z는 그대로: v.z = v.z;
        }

        // GPU(VBO) 업데이트
        Update();
    }


    glm::mat4 getModelMatrix()
    {
        glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(world_angle), glm::vec3(0, 0, 1));
        modelMatrix = glm::translate(modelMatrix, move_xyz);
        modelMatrix *= glm::mat4_cast(ro);
        modelMatrix = glm::scale(modelMatrix, multy);
		return modelMatrix;
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
class Cube : public Shape {
public:
    // 낙하 속도 (dt를 사용하지 않으므로 적절한 값으로 설정)
    float gravity = 0.05f;
    bool is_falling = true;

    // 이제 banghyang은 초기화 시 사용되지 않으며, 필요 없습니다.
    // 기존 코드와의 호환을 위해 banghyang은 유지했으나, 실제 로직에서는 사용하지 않습니다.
    glm::vec3 banghyang;

    // 생성자 수정: 초기 방향 벡터는 더 이상 사용되지 않습니다.
    Cube(const vector<Vertex>& ver, const vector<unsigned int>& ind, glm::vec3 pos)
        : Shape(ver, ind, pos) {
        // Cube의 크기 정보를 저장하여 충돌 검사에 사용합니다.
        // 현재 create_cube가 (x, y, z)를 multy로 설정하지 않으므로, 충돌 검사 시 직접 전달해야 합니다.
    }
    void Rotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        Shape::Rotation(dAngleX, dAngleY, dAngleZ);
    }
    

    void applyGravity_WorldSpace(float current_world_angle)
    {
        if (is_falling) {
            // 1. 월드 좌표계에서의 낙하 방향 벡터 (Y축 음의 방향)
            glm::vec4 world_fall_vector = glm::vec4(0.0f, -gravity, 0.0f, 0.0f);

            // 2. 월드 회전 각도의 '역변환' 회전 행렬을 생성합니다.
            //    -> 회전된 공간을 다시 월드 축으로 되돌립니다.
            glm::mat4 rotation_matrix = glm::rotate(
                glm::mat4(1.0f),
                glm::radians(-current_world_angle), // 각도에 마이너스(-)를 붙여 역회전
                glm::vec3(0, 0, 1)                  // Z축 기준
            );

            // 3. 로컬 이동 벡터 계산: 역회전 행렬을 낙하 벡터에 적용합니다.
            glm::vec4 local_fall_vector = rotation_matrix * world_fall_vector;

            // 4. 위치 업데이트: 로컬 이동 벡터를 위치에 적용합니다.
            move_xyz += glm::vec3(local_fall_vector);
        }
    }

    // 충돌 검사 (낙하 중지)
    bool checkCollision(const glm::quat& space_rotation, float space_bound, float cube_size)
    {
        bool collision_detected = false;

        // 로컬 위치를 월드 좌표로 변환
        glm::vec3 world_pos = space_rotation * move_xyz;
        glm::quat inv_rotation = glm::inverse(space_rotation); // 역회전 쿼터니언

        // X, Y, Z축에 대한 최소/최대 경계 (6개 면) 검사

        // X축 경계 (좌/우 벽)
        if (world_pos.x + cube_size >= space_bound) {
            world_pos.x = space_bound - cube_size;
            collision_detected = true;
        }
        else if (world_pos.x - cube_size <= -space_bound) {
            world_pos.x = -space_bound + cube_size;
            collision_detected = true;
        }

        // Y축 경계 (천장/바닥)
        if (!opened) {
            if (world_pos.y + cube_size >= space_bound) {
                world_pos.y = space_bound - cube_size;
                collision_detected = true;
            }
            else if (world_pos.y - cube_size <= -space_bound) {
                world_pos.y = -space_bound + cube_size;
                collision_detected = true;
            }
        }
        // Z축 경계 (앞/뒤 벽)
        if (world_pos.z + cube_size >= space_bound) {
            world_pos.z = space_bound - cube_size;
            collision_detected = true;
        }
        else if (world_pos.z - cube_size <= -space_bound) {
            world_pos.z = -space_bound + cube_size;
            collision_detected = true;
        }

        // 충돌이 감지되었으면 위치 보정 적용
        if (collision_detected) {
            // 보정된 월드 위치를 로컬 위치(move_xyz)로 역변환하여 저장
            move_xyz = inv_rotation * world_pos;
            return true;
        }

        // 충돌이 없으면 낙하 상태 유지 (Y축 경계 안에 있다는 의미)
        is_falling = true;
        return false;
    }
};
class Ball : public Shape {
public:
    // 공의 이동 방향과 속도를 나타내는 벡터
    glm::vec3 velocity;
    float radius; // 충돌 계산을 위한 반지름

    Ball(const vector<Vertex>& ver, const vector<unsigned int>& ind, glm::vec3 pos, float r, glm::vec3 vel)
        : Shape(ver, ind, pos), radius(r), velocity(vel) {
    }
};
void result_matrix(Camera& camera, Shape& shpae) { // a는 반대편 인지, b는 flag posin
    glm::mat4 modelMatrix;
    glm::mat4 uProj = camera.Projection_matrix_update();
    glm::mat4 uModel = shpae.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_update();
    modelMatrix = uProj * uView * uModel;
    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

}

Shape space = {
    // 24개의 정점 선언 (위치는 8개, 색상 때문에 24개로 복제)
    {
        // ----------------- 앞면 (RED) ------------------
        { glm::vec3(-3.0f, -3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 0
        { glm::vec3(3.0f, -3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 1
        { glm::vec3(3.0f,  3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 2
        { glm::vec3(-3.0f,  3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 3

        // ----------------- 뒷면 (GREEN) -----------------
        { glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 4
        { glm::vec3(3.0f, -3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 5
        { glm::vec3(3.0f,  3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 6
        { glm::vec3(-3.0f,  3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // 7

        // ----------------- 윗면 (BLUE) ------------------
        { glm::vec3(-3.0f,  3.0f,  3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 8 (위치 V3 복제)
        { glm::vec3(3.0f,  3.0f,  3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 9 (위치 V2 복제)
        { glm::vec3(3.0f,  3.0f, -3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 10 (위치 V6 복제)
        { glm::vec3(-3.0f,  3.0f, -3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 11 (위치 V7 복제)

        // ... 나머지 3개 면도 같은 방식으로 고유 색상 부여 후 4개씩 복제 ...

        // ----------------- 아랫면 (CYAN) ----------------
        { glm::vec3(-3.0f, -3.0f,  3.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 12
        { glm::vec3(3.0f, -3.0f,  3.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 13
        { glm::vec3(3.0f, -3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 14
        { glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 15

        // ----------------- 왼쪽 면 (MAGENTA) ---------------
        { glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 16
        { glm::vec3(-3.0f, -3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 17
        { glm::vec3(-3.0f,  3.0f,  3.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 18
        { glm::vec3(-3.0f,  3.0f, -3.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 19

        // ----------------- 오른쪽 면 (YELLOW) --------------
        { glm::vec3(3.0f, -3.0f,  3.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 20
        { glm::vec3(3.0f, -3.0f, -3.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 21
        { glm::vec3(3.0f,  3.0f, -3.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 22
        { glm::vec3(3.0f,  3.0f,  3.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }  // 23
    },
    // 36개 인덱스 (12개 삼각형)
    {
        // Front (0, 1, 2, 3) -> Red
        0, 1, 2,
        2, 3, 0,

        // Back (4, 5, 6, 7) -> Green
        4, 6, 5,
        6, 4, 7,

        // Top (8, 9, 10, 11) -> Blue
        8, 9, 10,
        10, 11, 8,

        // Bottom (12, 13, 14, 15) -> Cyan
        12, 14, 13,
        14, 12, 15,

        // Left (16, 17, 18, 19) -> Magenta
        16, 17, 18,
        18, 19, 16,

        // Right (20, 21, 22, 23) -> Yellow
        20, 21, 22,
        22, 23, 20
    },
	glm::vec3(0.0f, 0.0f, 0.0f) // 초기 위치
};
vector<Cube> cubes;
void initialize_cubes() {
    // 큐브 크기 정의 (x=y=z 가정)
    float size1 = 0.2f, size2 = 0.5f, size3 = 0.7f;

    // 초기 위치는 공중에 띄워야 떨어지는 것이 보입니다.
    Cube cube1(create_cube(size1, size1, size1), create_cube_index(), glm::vec3{ 0, 0.0f, 1.2f });
    cube1.multy = glm::vec3(size1); // 크기 설정 (Shape::multy 사용)
    cube1.Init();
    cubes.push_back(cube1);

    Cube cube2(create_cube(size2, size2, size2), create_cube_index(), glm::vec3{ 0.0f, 0.0f, 0.7f });
    cube2.multy = glm::vec3(size2);
    cube2.Init();
    cubes.push_back(cube2);  

    Cube cube3(create_cube(size3, size3, size3), create_cube_index(), glm::vec3{0.0f, 0.0f, 0.0f });
    cube3.multy = glm::vec3(size3);
    cube3.Init();
    cubes.push_back(cube3);
}
vector<Ball> Balls;
void initialize_spheres(int count = 100) {
    for (int i = 0; i < count; i++) {
        glm::vec3 random_direction = glm::normalize(glm::vec3(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(-1.0f, 1.0f)));
        Ball new_sphere(create_sphere(0.5f), create_sphere_index(), glm::vec3{ 0,0,0 }, 0.5f, random_direction);
        new_sphere.Init();
        Balls.push_back(new_sphere);
    }
}