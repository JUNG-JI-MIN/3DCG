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
float rotationAngle = 0.0f;
float radius_change = 1;
glm::vec3 mumu = { 1.5f,1.5f,1.5f };
float asd = 0.01f;
float center_x = 0.25f, center_y = 0.0f;
bool spin = false;
bool running = true;
bool tspin = false;
// 쓸데가리 없는거
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};
class Shape {
public:
    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint ebo; // Element Buffer Object
    vector<Vertex> vertices; // 버텍스 배열
    vector <unsigned int> indices; // 인덱스
    float rotateZ = 0.0f, tx, ty;
    glm::vec3 multy = {1.0f, 1.0f, 1.0f};
    Shape(vector<Vertex> ver, vector<unsigned int> ind)
        :vertices(ver), indices(ind) {
    }
    void set_state(float angle, float x, float y, glm::vec3 mul)
    {
        tx = x;
        ty = y;
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
    /*void matrix_updata() {
        glm::mat4 modelMatrix = get_R_Matrix(rotateZ, { 0.0f,0.0f,1.0f });
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "uModel");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }*/
	void matrix_updata() {
		glm::mat4 T = get_T_Matrix({ tx,ty,0.0f });
		glm::mat4 R = get_R_Matrix(rotateZ, { 0.0f,0.0f,1.0f });
		glm::mat4 S = get_S_Matrix(multy);
		glm::mat4 modelMatrix = T * R * S; // TRS 순서로 행렬 곱셈
		GLuint modelLoc = glGetUniformLocation(shaderProgramID, "uModel");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}
    void matrix_updata3() {
        glm::mat4 T = get_T_Matrix({ tx,ty,0.0f });
        glm::mat4 R = get_R_Matrix(rotateZ, { 0.0f,0.0f,1.0f });
        glm::mat4 S = get_S_Matrix(multy);
        glm::mat4 modelMatrix = R * T * S; // TRS 순서로 행렬 곱셈
        GLuint modelLoc = glGetUniformLocation(shaderProgramID, "uModel");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }
    void matrix_updata_vo2() {
        // 1. -center로 이동
        glm::mat4 T1 = glm::translate(glm::mat4(1.0f), glm::vec3(-tx, -ty, 0.0f));
        // 2. 회전
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
        // 3. +center로 이동
        glm::mat4 T2 = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0f));
        // 4. 신축
        glm::mat4 S = glm::scale(glm::mat4(1.0f), multy);

        // 최종 행렬: T2 * R * T1 * S
        glm::mat4 modelMatrix = T2 * R * T1 * S;

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



Shape MultiTriangle =
{
    // 8개의 정점 (Vertices)
    {
        // { 위치(vec3), 색상(vec4) }
        { glm::vec3(-0.5f,   0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 0번 정점 (원래 1번, 맨 왼쪽)
        { glm::vec3(-0.25f,  0.25f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 1번 정점 (원래 2번)
        { glm::vec3(0.0f,   0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 2번 정점 (원래 3번, 맨 위)
        { glm::vec3(0.25f,  0.25f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 3번 정점 (원래 4번)
        { glm::vec3(0.5f,   0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 4번 정점 (원래 5번, 맨 오른쪽)
        { glm::vec3(0.25f, -0.25f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 5번 정점 (원래 6번)
        { glm::vec3(0.0f,  -0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, // 6번 정점 (원래 7번, 맨 밑)
        { glm::vec3(-0.25f, -0.25f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }  // 7번 정점 (원래 8번)
    },
    // 4개 삼각형의 인덱스 (총 12개) - 0-based 인덱스로 변환됨
    {
        // 첫 번째 삼각형: 7, 0, 1번 정점 사용 (원래: 7, 0, 1)
        7, 0, 1,
        // 두 번째 삼각형: 1, 2, 3번 정점 사용 (원래: 1, 2, 3)
        1, 2, 3,
        // 세 번째 삼각형: 3, 4, 5번 정점 사용 (원래: 3, 4, 5)
        3, 4, 5,
        // 네 번째 삼각형: 5, 6, 7번 정점 사용 (원래: 5, 6, 7)
        5, 6, 7
    }
};
Shape Multi =
{
    // 8개의 정점 (Vertices)
    {
        // { 위치(vec3), 색상(vec4) }
        { glm::vec3(-0.1f,   0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 0번 정점 (원래 1번, 맨 왼쪽)
        { glm::vec3(-0.25f,  0.25f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 1번 정점 (원래 2번)
        { glm::vec3(0.0f,   0.1f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 2번 정점 (원래 3번, 맨 위)
        { glm::vec3(0.25f,  0.25f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 3번 정점 (원래 4번)
        { glm::vec3(0.1f,   0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 4번 정점 (원래 5번, 맨 오른쪽)
        { glm::vec3(0.25f, -0.25f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 5번 정점 (원래 6번)
        { glm::vec3(0.0f,  -0.1f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, // 6번 정점 (원래 7번, 맨 밑)
        { glm::vec3(-0.25f, -0.25f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }  // 7번 정점 (원래 8번)
    },
    // 4개 삼각형의 인덱스 (총 12개) - 0-based 인덱스로 변환됨
    {
        // 첫 번째 삼각형: 7, 0, 1번 정점 사용 (원래: 7, 0, 1)
        7, 0, 1,
        // 두 번째 삼각형: 1, 2, 3번 정점 사용 (원래: 1, 2, 3)
        1, 2, 3,
        // 세 번째 삼각형: 3, 4, 5번 정점 사용 (원래: 3, 4, 5)
        3, 4, 5,
        // 네 번째 삼각형: 5, 6, 7번 정점 사용 (원래: 5, 6, 7)
        5, 6, 7
    }
};
Shape Left(MultiTriangle.vertices, MultiTriangle.indices);
Shape Right(Multi.vertices, Multi.indices);