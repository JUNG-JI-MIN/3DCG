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
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
};
/*
궤도는 원형이고 궤도의 크기는 스케일링, 반지름이 r이라 할떄 스케일링이 s라면 궤도의 반지름은 r*s이다.
별의 위치는 (0,0,0)이고 행성의 초기 위치는 (r*s,0,0)이다.
*/
float light_color[5][3] = {
    { 0.5f,1.0f,1.0f },
    { 1.0f,0.5f,1.0f },
    { 1.0f,1.0f,0.5f },
    { 0.7f,1.0f,0.7f },
    { 1.0f,0.7f,1.0f }
};
int light_color_count = 0;
float light_angle = 0.0f, d_light_angle = 0.0f;
bool light_rotate = false;
float ring = 1;
bool trand = true, wi = false, zrotate[2] = { false };
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
            circle_vertices.push_back({ glm::vec3(x, y, z), glm::vec4(color), glm::normalize(glm::vec3(x, y, z))});
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
//여기까지가 이 코드에서만 필요한거
float random(float min = 0.0f, float max = 1.0f) {
    // 0.0 ~ 1.0 사이의 정규화된 난수 생성
    float randomValue = (float)rand() / (float)RAND_MAX;
    // min ~ max 범위로 스케일링
    float result = min + randomValue * (max - min);
    if (result == 0) result = 0.001;
    return result;
}

class Shape {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
	vector <glm::vec3> normal_vertices;
    glm::vec3 move_xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    glm::quat ro = glm::quat(1, 0, 0, 0);
    // 이코드 에서 사용할거
    float d_angle = 3.0f;
    float gong_angle; // 자전
    float org_gong_angle;
    float real_gong_angle = 0.0f, d_real_gong_angle; // 공전
    glm::vec3 B_xyz = { 0.0f,0.0f ,0.0f }; // 공전의 공전
    glm::vec3 plus_xyz = { 0.0f,0.0f ,0.0f };
    int dur; // 0은 별, 1은 행성, 2는 위성
    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind, glm::vec3 a, float angle, int durgkf)
        :vertices(ver), indices(ind), move_xyz(a), gong_angle(angle), dur(durgkf), org_gong_angle(angle)
		
    {
        
    }
    void reset_state()
    {
        move_xyz = { 1.5f,0.0f ,0.0f };
        ro = glm::quat(1, 0, 0, 0);
        gong_angle = org_gong_angle;
        real_gong_angle = 0.0f;
        B_xyz = { 0.0f,0.0f ,0.0f };
        plus_xyz = { 0.0f,0.0f ,0.0f };
    }
    void ja_addRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        glm::quat qx = glm::angleAxis(glm::radians(dAngleX), glm::vec3(1, 0, 0));
        glm::quat qy = glm::angleAxis(glm::radians(dAngleY), glm::vec3(0, 1, 0));
        glm::quat qz = glm::angleAxis(glm::radians(dAngleZ), glm::vec3(0, 0, 1));
        ro = glm::normalize(qz * qy * qx * ro);
    }
    void add_XYZ(float dx, float dy, float dz)
    {
        plus_xyz.x += dx;
        plus_xyz.y += dy;
        plus_xyz.z += dz;
    }
    void set_XYZ(float dx, float dy, float dz)
    {
        move_xyz.x = dx;
        move_xyz.y = dy;
        move_xyz.z = dz;
    }
    glm::mat4 getModelMatrix()
    {
        //스케일링 회전 이동
        glm::mat4 model = glm::mat4(1.0f);
        //glm::vec4 asd = { move_xyz,1.0f };
        //glm::vec4 transformed_position = model * asd;
        model = glm::translate(model, plus_xyz); // 이동하는 거
        model = glm::rotate(model, glm::radians(gong_angle), { 0.0f,0.0f, 1.0f }); // 축 기울이기
        model = glm::translate(model, move_xyz); // 이동하는 거
        model = model * glm::mat4_cast(ro); // 자전
        model = glm::translate(model, B_xyz); // 이동하는 거
        model = glm::scale(model, multy);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLuint u = glGetUniformLocation(shaderProgramID, "n");
        glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(normalMatrix));
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
        if (wi) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
        model = glm::scale(model, { ring,ring,ring });
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
class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fovy = 45.0f; // 시양각
    float aspect; // 종횡비 아직 w와 h가 없기 때문에 계산 불가
    float n = 0.1f; // near
    float f = 100.0f; // far
    float orthoScale = 2.5f; // 직각 투영 범위
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 u)
        : position(pos), target(tar), up(u) {

        cout << aspect << endl;
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
        float scale = orthoScale;
        return glm::ortho(-scale * aspect, scale * aspect,
            -scale, scale, n, f);
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
void result_matrix(Camera camera, Shape shape) {
    glm::mat4 uProj;
    glm::mat4 uModel = shape.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_update();
    if (trand) uProj = camera.Projection_matrix_update();
    else uProj = camera.Orthographic_matrix_update();

    GLuint u = glGetUniformLocation(shaderProgramID, "m");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));
    u = glGetUniformLocation(shaderProgramID, "v");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));
    u = glGetUniformLocation(shaderProgramID, "p");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));
}
void result_line_matrix(Camera camera, Line line) {
    glm::mat4 uProj;
    glm::mat4 uModel = line.getModelMatrix();
    glm::mat4 uView = camera.View_matrix_update();
    if (trand) uProj = camera.Projection_matrix_update();
    else uProj = camera.Orthographic_matrix_update();

    GLuint u = glGetUniformLocation(shaderProgramID, "m");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uModel));
    u = glGetUniformLocation(shaderProgramID, "v");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uView));
    u = glGetUniformLocation(shaderProgramID, "p");
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(uProj));
}
Light light({ 20.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

Camera camera({ 0.0f,1.0f,5.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
vector<Line*> lines;
vector<Shape*> shapes;
Shape c4(create_sphere(0.5f, 0), create_sphere_index(), glm::vec3{ 0.0f,0.0f,0.0f }, 0, 0);

Shape a1(create_sphere(0.2f, 1), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, 45, 1);
Shape a2(create_sphere(0.2f, 1), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, -45, 1);
Shape a3(create_sphere(0.2f, 1), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, 0, 1);

Shape b1(create_sphere(0.1f, 2), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, 45, 2);
Shape b2(create_sphere(0.1f, 2), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, -45, 2);
Shape b3(create_sphere(0.1f, 2), create_sphere_index(), glm::vec3{ 1.5f,0.0f,0.0f }, 0, 2);

Line c1(create_circle(1.5f), 45, 0);
Line c2(create_circle(1.5f), -45, 0);
Line c3(create_circle(1.5f), 0, 0);

Line d1(create_circle(0.5f), 45, 1);
Line d2(create_circle(0.5f), -45, 1);
Line d3(create_circle(0.5f), 0, 1);
