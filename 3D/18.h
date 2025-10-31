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
    float rotateZ = 0.0f;
    glm::vec3 xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    // 이코드 에서 사용할거
    int kind_of_shape = 0;
    // 이 코드에서 사용할거
    Shape(const vector<Vertex>& ver, const vector<unsigned int>& ind, int kos)
        :vertices(ver), indices(ind), kind_of_shape(kos) {
    }
    void set_state(float angle, glm::vec3 XYZ, glm::vec3 mul)
    {
        xyz = XYZ;
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
        R = glm::rotate(R, glm::radians(rotateZ), XYZ); //각도, 회전축)
        return R;
    }
    void matrix_updata() {
        glm::mat4 T = get_T_Matrix(xyz);
        glm::mat4 R = get_R_Matrix(rotateZ, { 0.0f,1.0f,.0f });
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