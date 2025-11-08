#pragma once
#include <iostream>
#include <vector>
#include <gl/glm/glm.hpp>
using namespace std;
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};
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