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
};
/*
궤도는 원형이고 궤도의 크기는 스케일링, 반지름이 r이라 할떄 스케일링이 s라면 궤도의 반지름은 r*s이다.
별의 위치는 (0,0,0)이고 행성의 초기 위치는 (r*s,0,0)이다.
*/
vector<Vertex> create_circle(float r = 1.0f) {
	vector<Vertex> circle_vertices;
    float angle = 0.0f, d_angle =3.0f;

	while (angle <= 360) {
		float rad = glm::radians(angle);
		float x = r * cos(rad);
		float z = r * sin(rad);
		circle_vertices.push_back({ glm::vec3(x, 0.0f, z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
		angle += d_angle;
    }
    return circle_vertices;
}

class Shape {
public:
    glm::mat4 getModelMatrix();
};
class Line {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    float rotate_angle = 0.0f;
    glm::vec3 xyz = { 0.0f,0.0f ,0.0f };
    glm::vec3 multy = { 1.0f, 1.0f, 1.0f };
    Line(vector<Vertex> ver,float angle = 0.0f) {
		vertices = ver;
		rotate_angle = angle;
    }
    glm::mat4 getModelMatrix()
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, xyz);
        model = glm::rotate(model, glm::radians(rotate_angle), {0.0f,0.0f, 1.0f});
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
    glm::mat4 uView = camera.View_matrix_updata();
    glm::mat4 uProj = camera.Projection_matrix_updata();
    glm::mat4 modelMatrix = uProj * uView;
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
Camera camera({ 0.0f,1.0f,5.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });
vector<Line*> line;

