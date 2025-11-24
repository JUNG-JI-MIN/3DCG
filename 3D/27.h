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
bool tank_move[4] = { false };
float rotate_tank_angle = 0.0f, rotate_posin_angle = 0.0f, rotate_flag_angle = 0.0f;
float d_posin_angle = 0.03f, d_flag_angle = 0.03f;
float RotateCamera = 0.0f, d_camera = 3.0f;
bool atime = true, tro, lro, gro, pro, yro, rro;
//여기까지가 이 코드에서만 필요한거
float light_color[5][3] = {
    { 1.0f,1.0f,1.0f },
    { 1.0f,0.5f,1.0f },
    { 1.0f,1.0f,0.5f },
    { 0.7f,1.0f,0.7f },
    { 1.0f,0.7f,1.0f }
};
int light_color_count = 0;
float light_angle = 0.0f, d_light_angle = 0.0f;
bool light_rotate = false;
bool light_off = false;
//여기까지 조명에 관한거
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
    void camera_PY() {
        glm::vec3 relativeTarget = target - position;

        glm::vec3 rotatedVector = glm::rotate(
            relativeTarget,
            glm::radians(d_camera),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        target = position + rotatedVector;
    }

    void camera_TY() {
        glm::vec3 relativeTarget = position - target;

        glm::vec3 rotatedVector = glm::rotate(
            relativeTarget,
            glm::radians(d_camera),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        position = target + rotatedVector;
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
Light light({ 30.0f,15.0f,-30.0f }, { 1.0f,1.0f,1.0f });
Camera camera({ 0.0f,55.0f,60.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });

class TankPart {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
    glm::vec3 move_xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 part_xyz = { 0.0f,0.0f,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    glm::quat ro = glm::quat(1, 0, 0, 0);
    // 이코드 에서 사용할거
    // 이 코드에서 사용할거
    TankPart(const vector<Vertex>& ver, const vector<unsigned int>& ind, glm::vec3 a, glm::vec3 b)
        :vertices(ver), indices(ind), move_xyz(a), part_xyz(b) {
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
    virtual glm::mat4 getModelMatrix(int a) = 0;
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

class TankBody : public TankPart {
public:
    TankBody(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 0.0f,2.0f,0.0f }) {
    }
    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, part_xyz);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
    }
};
class TankCenter : public TankPart {
public:
    TankCenter(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 0.0f,5.0f,0.0f }) {
    }
    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = glm::rotate(model, glm::radians(rotate_tank_angle), { 0.0f,1.0f,0.0f });
        model = glm::translate(model, -move_xyz); // 이동하는 거
        model = glm::translate(model, part_xyz);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
    }
};
class TankTop : public TankPart {
public:
    TankTop(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 3.0f,7.5f,0.0f }) {
    }
    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = glm::rotate(model, glm::radians(rotate_tank_angle), { 0.0f,1.0f,0.0f });
        model = glm::translate(model, -move_xyz); // 이동하는 거
        if (a == 1) {
            model = glm::translate(model, { part_xyz.x - part_xyz.x * 2,part_xyz.y,part_xyz.z });
        }
        else model = glm::translate(model, part_xyz);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
    }
};
class Posin : public TankPart {
public:
    Posin(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 3.0f,7.0f,2.0f }) {
    }
    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = glm::rotate(model, glm::radians(rotate_tank_angle), { 0.0f,1.0f,0.0f });
        model = glm::translate(model, -move_xyz); // 이동하는 거
        if (a == 1) {
            model = glm::translate(model, { part_xyz.x - part_xyz.x * 2,part_xyz.y,part_xyz.z });
        }
        else model = glm::translate(model, part_xyz);

        model = glm::translate(model, move_xyz);

        model = glm::translate(model, { 0, 0,-2 });
        if (a == 1) model = glm::rotate(model, rotate_posin_angle, { 0,1,0 });
        else model = glm::rotate(model, -rotate_posin_angle, { 0,1,0 });
        model = glm::translate(model, { 0, 0,2 }); // 이동하는 거

        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        return model;
    }
};
class Flag : public TankPart {
public:
    Flag(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 3.0f,11.0f,0.0f }) {
    }
    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = glm::rotate(model, glm::radians(rotate_tank_angle), { 0.0f,1.0f,0.0f });
        model = glm::translate(model, -move_xyz); // 이동하는 거
        if (a == 1) {
            model = glm::translate(model, { part_xyz.x - part_xyz.x * 2,part_xyz.y,part_xyz.z });
        }
        else model = glm::translate(model, part_xyz);

        model = glm::translate(model, move_xyz);

        model = glm::translate(model, { 0, -4,0 });
        if (a == 1) model = glm::rotate(model, rotate_flag_angle, { 1,0,0 });
        else model = glm::rotate(model, -rotate_flag_angle, { 1,0,0 });
        model = glm::translate(model, { 0, 4,0 }); // 이동하는 거

        model = model * glm::mat4_cast(ro); // 자전
        model = glm::scale(model, multy);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        return model;
    }
};

void result_matrix(Camera& camera, TankPart& tank, int a = 0, int b = 0, int i = 0) { // a는 반대편 인지, b는 flag posin
    glm::mat4 modelMatrix;
    glm::mat4 uProj = glm::mat4{ 1.0f };
    if (i == 1) uProj = camera.Orthographic_matrix_update();
    else uProj = camera.Projection_matrix_update();
    glm::mat4 uModel = tank.getModelMatrix(a);
    glm::mat4 uView = camera.View_matrix_update();
    GLuint u = glGetUniformLocation(shaderProgramID, "m");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));
    u = glGetUniformLocation(shaderProgramID, "v");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));
    u = glGetUniformLocation(shaderProgramID, "p");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));

}
class Ground : public TankPart {
public:
    Ground(const vector<Vertex>& ver,
        const vector<unsigned int>& ind,
        glm::vec3 pos) : TankPart(ver, ind, pos, glm::vec3{ 0.0f, -1.0f, 0.0f }) {
    }

    glm::mat4 getModelMatrix(int a) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 땅은 고정된 위치에 있으므로 간단한 변환만 적용
        model = glm::translate(model, part_xyz);  // 기본 위치
        model = glm::translate(model, move_xyz);  // 추가 이동 (필요시)
        model = model * glm::mat4_cast(ro);       // 회전
        model = glm::scale(model, multy);         // 스케일

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        return model;
    }
};
class Tank {
private:

public:
    TankBody tbody;
    TankCenter tcenter;
    Flag tflag;
    TankTop ttop;
    Posin tposin;
    Tank(glm::vec3 pos)
        :tbody(body, body_index, pos),
        tcenter(center, center_index, pos),
        tflag(flag, flag_index, pos),
        ttop(top, top_index, pos),
        tposin(posin, posin_index, pos)
    {
    }
    void reset() {
        tbody.reset_state();
        tcenter.reset_state();
        tflag.reset_state();
        ttop.reset_state();
        tposin.reset_state();
    }
    void Init() {
        tbody.Init();
        tcenter.Init();
        tflag.Init();
        ttop.Init();
        tposin.Init();
    }
    void Update() {
        tbody.Update();
        tcenter.Update();
        tflag.Update();
        ttop.Update();
        tposin.Update();
    }
    void Draw(Camera& ca, int i) {

        result_matrix(ca, tbody, 0, 0, i);
        tbody.Draw();

        result_matrix(ca, tcenter, 0, 0, i);
        tcenter.Draw();

        result_matrix(ca, tflag, 0, 2, i);
        tflag.Draw();

        result_matrix(ca, ttop, 0, 0, i);
        ttop.Draw();

        result_matrix(ca, tposin, 0, 1, i);
        tposin.Draw();
        // 반대쪽 구현
        result_matrix(ca, tflag, 1, 2, i);
        tflag.Draw();

        result_matrix(ca, ttop, 1, 0, i);
        ttop.Draw();

        result_matrix(ca, tposin, 1, 1, i);
        tposin.Draw();
    }
    void moveing_man(float dx, float dy, float dz) {
        tbody.add_XYZ(dx, dy, dz);
        tcenter.add_XYZ(dx, dy, dz);
        tflag.add_XYZ(dx, dy, dz);
        ttop.add_XYZ(dx, dy, dz);
        tposin.add_XYZ(dx, dy, dz);
    }
    void y_rotate(int a = 0) {
        if (a == 0) {
            rotate_tank_angle -= 0.3f;
        }
        else rotate_tank_angle += 0.3f;
    }
    void Top_trace() {
        if (tflag.part_xyz.x < 3.0f)
        {
            tflag.part_xyz.x += 0.1f;
            ttop.part_xyz.x += 0.1f;
            tposin.part_xyz.x += 0.1f;
        }
        else {
            tflag.part_xyz.x = 3.0f;
            ttop.part_xyz.x = 3.0f;
            tposin.part_xyz.x = 3.0f;
            lro = false;
        }
    }
    void posin_Y() {
        rotate_posin_angle += d_posin_angle;

        // ★ 범위 체크 후 클램프
        if (rotate_posin_angle < glm::radians(-180.0f)) {
            rotate_posin_angle = glm::radians(-180.0f);
            d_posin_angle *= -1.0f;
        }
        else if (rotate_posin_angle > 0.0f) {
            rotate_posin_angle = 0.0f;
            d_posin_angle *= -1.0f;
        }
    }
    void flag_X(int a = 0) {
        rotate_flag_angle += d_flag_angle;

        // ★ 범위 체크 후 클램프
        if (rotate_flag_angle < glm::radians(-90.0f)) {
            rotate_flag_angle = glm::radians(-90.0f);
            d_flag_angle *= -1.0f;
        }
        else if (rotate_flag_angle > glm::radians(90.0f)) {
            rotate_flag_angle = glm::radians(90.0f);
            d_flag_angle *= -1.0f;
        }
    }
    void handle_event(unsigned char key) {
    }
};

class Line {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    float rotate_angle = 0.0f;
    glm::vec3 xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    glm::vec3 plus_xyz = { 0.0f,0.0f ,0.0f };
    int dur;
    float real_gong_angle = 0.0f;
    Line(vector<Vertex> ver, float angle = 0.0f, int durgkf = 0) {
        vertices = ver;
        rotate_angle = angle;
        if (durgkf == 1) xyz = { 1.5f,0.0f ,0.0f };
        dur = durgkf;
    }
    void set_XYZ(float dx, float dy, float dz)
    {
        xyz.x = dx;
        xyz.y = dy;
        xyz.z = dz;
    }
    void reset() {
        plus_xyz = { 0.0f,0.0f ,0.0f };
    }
    void add_XYZ(float dx, float dy, float dz)
    {
        plus_xyz.x += dx;
        plus_xyz.y += dy;
        plus_xyz.z += dz;
    }
    glm::mat4 getModelMatrix()
    {
        float r = glm::radians(rotate_angle);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, plus_xyz);
        if (dur == 1) {
            model = glm::rotate(model, glm::radians(rotate_angle), { 0.0f,0.0f, 1.0f });
        }
        model = glm::translate(model, xyz);
        if (dur == 0) model = glm::rotate(model, r, { 0.0f,0.0f, 1.0f });
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
        glDrawArrays(GL_LINES, 1, vertices.size());
    }
    void Delete()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

};

void result_line_matrix(Camera& camera, Line& line) {
    glm::mat4 modelMatrix;
    glm::mat4 uProj;
    glm::mat4 uModel = line.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_update();
    uProj = camera.Projection_matrix_update();
    modelMatrix = uProj * uView * uModel;

    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "u");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
// tank_model.h 뒤나 적절한 위치에 추가
vector<Vertex> ground = {
    // ----------------- 윗면 (Y=0) | Normal: +Y ------------------ (3, 2, 6, 7의 위치)
    { glm::vec3(-50.0f,  0.0f,  50.0f), glm::vec4(0.4f, 1.0f, 0.2f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 0
    { glm::vec3(50.0f,  0.0f,  50.0f), glm::vec4(1.0f, 0.7f, 0.2f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 1
    { glm::vec3(50.0f,  0.0f, -50.0f), glm::vec4(0.4f, 0.7f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 2
    { glm::vec3(-50.0f,  0.0f, -50.0f), glm::vec4(0.0f, 0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // 3

    // ----------------- 아랫면 (Y=-2) | Normal: -Y ---------------- (0, 1, 5, 4의 위치)
    { glm::vec3(-50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 4
    { glm::vec3(50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 5
    { glm::vec3(50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 6
    { glm::vec3(-50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, // 7

    // ----------------- 앞면 (Z=50) | Normal: +Z ------------------ (0, 1, 2, 3의 위치)
    { glm::vec3(-50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 8
    { glm::vec3(50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 9
    { glm::vec3(50.0f,  0.0f,  50.0f), glm::vec4(1.0f, 0.7f, 0.2f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 10
    { glm::vec3(-50.0f,  0.0f,  50.0f), glm::vec4(0.4f, 1.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // 11

    // ----------------- 뒷면 (Z=-50) | Normal: -Z ----------------- (4, 5, 6, 7의 위치)
    { glm::vec3(-50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 12
    { glm::vec3(50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 13
    { glm::vec3(50.0f,  0.0f, -50.0f), glm::vec4(0.4f, 0.7f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 14
    { glm::vec3(-50.0f,  0.0f, -50.0f), glm::vec4(0.0f, 0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, // 15

    // ----------------- 왼쪽면 (X=-50) | Normal: -X --------------- (0, 3, 7, 4의 위치)
    { glm::vec3(-50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 16
    { glm::vec3(-50.0f,  0.0f,  50.0f), glm::vec4(0.4f, 1.0f, 0.2f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 17
    { glm::vec3(-50.0f,  0.0f, -50.0f), glm::vec4(0.0f, 0.0f, 0.2f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 18
    { glm::vec3(-50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, // 19

    // ----------------- 오른쪽면 (X=50) | Normal: +X -------------- (1, 2, 6, 5의 위치)
    { glm::vec3(50.0f, -2.0f,  50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 20
    { glm::vec3(50.0f,  0.0f,  50.0f), glm::vec4(1.0f, 0.7f, 0.2f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 21
    { glm::vec3(50.0f,  0.0f, -50.0f), glm::vec4(0.4f, 0.7f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 22
    { glm::vec3(50.0f, -2.0f, -50.0f), glm::vec4(0.5f, 0.8f, 0.3f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }  // 23
};

vector<unsigned int> ground_index = {
    // 윗면 (Y = 0) - Indices 0-3
    0, 1, 2,
    2, 3, 0,

    // 아랫면 (Y = -2) - Indices 4-7
    4, 5, 6,
    6, 7, 4,

    // 앞면 (Z = 50) - Indices 8-11
    8, 9, 10,
    10, 11, 8,

    // 뒷면 (Z = -50) - Indices 12-15
    12, 13, 14,
    14, 15, 12,

    // 왼쪽면 (X = -50) - Indices 16-19
    16, 17, 18,
    18, 19, 16,

    // 오른쪽면 (X = 50) - Indices 20-23
    20, 21, 22,
    22, 23, 20
};
Ground ground_obj(ground, ground_index, glm::vec3{ 0.0f, 0.0f, 0.0f });
Ground camera_cube(create_cube(0.5f, 0.5f, 0.5f, { 0,0,1,1 }), create_cube_index(), light.position);
