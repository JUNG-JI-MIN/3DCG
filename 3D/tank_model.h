#pragma once
#include <iostream>
#include <vector>
#include <gl/glm/glm.hpp>
using namespace std;
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
};
vector<Vertex> create_cube(float x = 1, float y = 1, float z = 1, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }) {
    vector<Vertex> cube_vertices = {
        // 앞면 | Normal: +Z
        { {-x, -y,  z}, color, {0.0f, 0.0f, 1.0f} }, // 0
        { { x, -y,  z}, color, {0.0f, 0.0f, 1.0f} }, // 1
        { { x,  y,  z}, color, {0.0f, 0.0f, 1.0f} }, // 2
        { {-x,  y,  z}, color, {0.0f, 0.0f, 1.0f} }, // 3

        // 뒷면 | Normal: -Z
        { {-x, -y, -z}, color, {0.0f, 0.0f, -1.0f} }, // 4
        { { x, -y, -z}, color, {0.0f, 0.0f, -1.0f} }, // 5
        { { x,  y, -z}, color, {0.0f, 0.0f, -1.0f} }, // 6
        { {-x,  y, -z}, color, {0.0f, 0.0f, -1.0f} }, // 7

        // 왼쪽면 | Normal: -X
        { {-x, -y, -z}, color, {-1.0f, 0.0f, 0.0f} }, // 8
        { {-x, -y,  z}, color, {-1.0f, 0.0f, 0.0f} }, // 9
        { {-x,  y,  z}, color, {-1.0f, 0.0f, 0.0f} }, // 10
        { {-x,  y, -z}, color, {-1.0f, 0.0f, 0.0f} }, // 11

        // 오른쪽면 | Normal: +X
        { { x, -y, -z}, color, {1.0f, 0.0f, 0.0f} }, // 12
        { { x, -y,  z}, color, {1.0f, 0.0f, 0.0f} }, // 13
        { { x,  y,  z}, color, {1.0f, 0.0f, 0.0f} }, // 14
        { { x,  y, -z}, color, {1.0f, 0.0f, 0.0f} }, // 15

        // 위면 | Normal: +Y
        { {-x,  y, -z}, color, {0.0f, 1.0f, 0.0f} }, // 16
        { { x,  y, -z}, color, {0.0f, 1.0f, 0.0f} }, // 17
        { { x,  y,  z}, color, {0.0f, 1.0f, 0.0f} }, // 18
        { {-x,  y,  z}, color, {0.0f, 1.0f, 0.0f} }, // 19

        // 아래면 | Normal: -Y
        { {-x, -y, -z}, color, {0.0f, -1.0f, 0.0f} }, // 20
        { {-x, -y,  z}, color, {0.0f, -1.0f, 0.0f} }, // 21
        { { x, -y,  z}, color, {0.0f, -1.0f, 0.0f} }, // 22
        { { x, -y, -z}, color, {0.0f, -1.0f, 0.0f} }, // 23
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
        19, 18, 15,
        19, 15, 16,
        // 아래면
        20, 23, 22,
        20, 22, 21
    };
    return cube_indices;
}

extern vector<Vertex> body;
extern vector<unsigned int> body_index;
extern vector<Vertex> center;
extern vector<unsigned int> center_index;

extern vector<Vertex> flag;
extern vector<unsigned int> flag_index;

extern vector<Vertex> top;
extern vector<unsigned int> top_index;

extern vector<Vertex> posin;
extern vector<unsigned int> posin_index;