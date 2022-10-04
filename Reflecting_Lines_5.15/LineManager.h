#pragma once
#include "Line.h"
#include <iostream>
#include <chrono>
#include "glm/gtc/matrix_transform.hpp"
#define FPS 30


class LineManager
{
	vector<Line> lines;
	vector<Point> collisions;
	std::chrono::steady_clock::time_point lastFrameTime;
	double msPerFrame;

	void handleReflection(int lineNum);
	glm::vec3 getNewHeading(glm::vec3, glm::vec3 );
	void setupNewPoint(Line*, glm::vec3);
	bool handleBorders(Line*);
	

public:
	void update();
	void render();
	void initEvenlySpaced(int);
	LineManager();

};

