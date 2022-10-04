#include "Line.h"
#include "glm/detail/func_geometric.inl"
 

Line::Line(Point p) {
	init();
	addPoint(p);
	//addPoint(p);
	heading = randomVec3InRange(-1,1, false);
}

Line::Line(float v) {
	init();
	velocity = v;
	heading = randomVec3InRange(-1, 1, false);
	

}

Line::Line() {
	init();
	Point p = randomPoint();
	addPoint(p);
	addPoint(p);
	heading = randomVec3InRange(-1, 1, false);
	
}



void Line::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glLineWidth(7.0);

	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(20);

	lastUpdateTime = chrono::steady_clock::now();
}





void Line::updateVAO() {
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * numPoints, &points[tail], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//age attribute
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//initEvenlySpaced(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Line::advance() {

	//first check how long it has been since last update
	chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
	float msElapsed = chrono::duration_cast<chrono::milliseconds>(currentTime - lastUpdateTime).count();
	lastUpdateTime = currentTime;


	//determine the step distance based on how long since last step
	float step = msElapsed / 1000 * velocity;

	points.at(head).pos += (glm::normalize(heading) * step);

	//increase the length of the line
	currLen += step;

	//check if this line has met its max length and needs to be shortened
	handleExceedMaxLength(step);


	//also add the milliseconds elapsed since the last update to the age of all the points
	for (int i = 0; i < points.size()-1; i++) {
		points.at(i).age += msElapsed;
	}

	/*
	points.back().pos.x = pos.x;
	points.back().pos.y = pos.y;
	points.back().pos.z = pos.z;
	*/
}


void Line::handleExceedMaxLength(float step) {
	if (currLen > MAX_LEN) {
		if (numPoints > 1) {
			glm::vec3 lastSegment = points.at(tail+1).pos - points.at(tail).pos;
			if (glm::length(lastSegment) < step) {
				removeTail();
				return;
			}
			lastSegment = glm::normalize(lastSegment);

			points.at(tail).pos += lastSegment * step;

			currLen = MAX_LEN;
		}

	}
}


void Line::render() {
	glBindVertexArray(VAO);
	//glDrawArrays(GL_POINTS, 0, points.size());
	glDrawArrays(GL_LINE_STRIP, 0, numPoints);
	glBindVertexArray(0);
}



Point randomPoint() {
	Point p;
	p.pos = randomVec3InRange(-1, 1, false);
	p.color = randomVec3InRange(0, 1, true);
	p.age = 0;

	return p;
}

void Line::addRandomPoint() {
	Point p = randomPoint();
	addPoint(p);
}

void Line::addPoint(Point p) {
	p.age = 0;
	points.push_back(p);
	numPoints++;
	if (numPoints!=1) head++;
}

void Line::removeTail() {
	tail++;
	numPoints--;
}


//setters

void Line::setHead(float x, float y) {
	points.at(head).pos.x = x;
	points.at(head).pos.y = y;
}

void Line::newHead() {
	addPoint(points.at(head));
}


//getters

Segment Line::getHeadSegment() { return getSegment(head - tail -1); }
Segment Line::getTailSegment() { return getSegment(0); }
Segment Line::getSegment(int i) { 
	if (numPoints > 1) { 
		return Segment(points.at(tail + i).pos.x, points.at(tail + i).pos.y, points.at(tail + i + 1).pos.x, points.at(tail + i + 1).pos.y); 
	} 
};



//vec3 functions 
// 
//returns a random vector3 with each x, y, and z between min and max
glm::vec3 randomVec3InRange(float min, float max, bool useZ) {
	float generateMax = 1000;
	float dif = max - min;

	glm::vec3 v;

	float n = (rand() % (int)generateMax) / generateMax;	// set a value n between 0 and 1
	v.x = min + n * dif;
	n = (rand() % (int)generateMax) / generateMax;
	v.y = min + n * dif;
	n = (rand() % (int)generateMax) / generateMax;
	v.z = useZ? min + n * dif: 0;

	return v;
}
glm::vec2 randomVec2InRange(float min, float max) {
	glm::vec3 v1 = randomVec3InRange(min, max, false);
	return glm::vec2(v1.x, v1.y);
}

glm::vec3 randomNormalVec3() {
	glm::vec3 v = randomVec3InRange(-1, 1, false);
	v = glm::normalize(v);

	return v;
};

void Segment::getEquation() {
	ea = y2 - y1;
	eb = x1 - x2;
	ec = ea * x1 + eb * y1;
	heading = glm::normalize(glm::vec3((x2 - x1), (y2 - y1), 0));
};