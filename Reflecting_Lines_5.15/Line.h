#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <chrono>

#define flat 1
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

#define MAX_LEN .9


glm::vec3 randomVec3InRange(float min, float max, bool);
glm::vec2 randomVec2InRange(float min, float max);
glm::vec3 randomNormalVec3();


class Point {
public:
	glm::vec3 pos, color;
	float age;
	Point(float x, float y, float z, float r, float g, float b, float n) {
		pos.x = x;
		pos.y = y; 
		pos.z = z;
		color.r = r;
		color.g = g;
		color.b = b;
		age = n;
	}
	Point() {

	}
	
};

Point randomPoint();

class Segment {
	
public:
	float x1, y1, x2, y2;

	//equation for the segment: ax + by = c
	float ea, eb, ec;
	glm::vec3 heading;
	Segment(float a, float b, float c, float d) {
		x1 = a; y1 = b;
		x2 = c; y2 = d;
	}
	void getEquation();
};

class Line {
	float velocity = .3;
	//bool flat = true;

	float currLen = 0;
	int head = 0, tail = 0;

	std::chrono::steady_clock::time_point lastUpdateTime;

	
	unsigned int VBO, VAO;
	void init();
	void handleExceedMaxLength(float step);

	void removeTail();



public:
	vector<Point> points;
	glm::vec3 heading{ 0, 1, 0};
	int numPoints = 0;


	Line();
	Line(float);
	Line(Point);
	Line randomLine();
	void addPoint(float, float, float, float, float, float);
	void addPoint(Point);

	void setHead(float, float);
	void newHead();

	Point getHead() { return points.at(head);  }
	Segment getHeadSegment();
	Segment getTailSegment();
	Segment getSegment(int i);

	void advance();
	void updateVAO();
	void render();

	void addRandomPoint();

};

