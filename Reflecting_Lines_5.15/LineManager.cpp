#include "LineManager.h"

#define INIT_CIRCLE_RADIUS .3
#define NUM_LINES 20


LineManager::LineManager() {
	srand(time(NULL));

	for (int n = 0; n < NUM_LINES; n++) {
		Line newLine;
		lines.push_back(newLine);
	}

	Point p(-.8, 0, 0, 0, 0, 0, 0);
	Point p2(.8, 0, 0, 0, 0, 0, 0);
	Line horizontal(p);
	horizontal.addPoint(p2);
	//lines.push_back(horizontal);
	/*lines.push_back(l2);
	lines.push_back(l3);
	lines.push_back(l4);*/


	msPerFrame = 1000 / FPS;
	lastFrameTime = std::chrono::steady_clock::now();
}


void LineManager::render() {

	chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();

	std::chrono::duration<double, std::milli> timeElapsed = currentTime - lastFrameTime;

	//if the scene is due for an update
	if (timeElapsed.count() > msPerFrame) {
		lastFrameTime = currentTime;

		//update each line
		for (int i = 0; i < lines.size(); i++) {
			lines[i].advance();
			handleReflection(i);
			//lines[i].addRandomPoint();
			lines[i].updateVAO();
		}
	}

	//always render the scene/each line each time render is called
	for (int i = 0; i < lines.size(); i++) {
		lines[i].render();
	}

}





void LineManager::handleReflection(int lineNum) {

	Line *l = &lines.at(lineNum);

	//check for and handle cases where line has gone out of the border and needs to reflect back
	if (handleBorders(l)) return;

	//now we check if this line segment has just crossed through any other line segments
	Segment thisSeg = l->getHeadSegment();
	thisSeg.getEquation();

	for (int i = 0; i < lines.size(); i++) {
		//only reflect off of other lines
		if (i != lineNum) {
			Line l2 = lines[i];
			for (int j = 0; j < l2.numPoints - 1; j++) {

				//get the next segment of the other line to compare against
				Segment otherSeg = l2.getSegment(j);
				otherSeg.getEquation();

				//from topcoder.com: https://www.topcoder.com/thrive/articles/Geometry%20Concepts%20part%202:%20%20Line%20Intersection%20and%20its%20Applications
				double det = thisSeg.ea * otherSeg.eb - otherSeg.ea * thisSeg.eb;
				if (det == 0) {
					//Lines are parallel
				}
				else {
					float x = (otherSeg.eb * thisSeg.ec - thisSeg.eb * otherSeg.ec) / det;
					float y = (thisSeg.ea * otherSeg.ec - otherSeg.ea * thisSeg.ec) / det;

					//is this x and y on the line segment or somewhere way off?
					if (min(thisSeg.x1, thisSeg.x2) < x && max(thisSeg.x1, thisSeg.x2) > x
						&& min(thisSeg.y1, thisSeg.y2) < y && max(thisSeg.y1, thisSeg.y2) > y && 
						min(otherSeg.x1, otherSeg.x2) < x&& max(otherSeg.x1, otherSeg.x2) > x
						&& min(otherSeg.y1, otherSeg.y2) < y&& max(otherSeg.y1, otherSeg.y2) > y){

						//shuffle the point back a bit so it doesn't always intersect the line
						l->setHead(x - l->heading.x * .01, y - l->heading.y * .01);

						setupNewPoint(l, otherSeg.heading);

						return;
					}
				}


			}
		}
	}
}

bool LineManager::handleBorders(Line* l) {

	if (l->points.back().pos.x < -1) {
		l->points.back().pos.x = -1;		//bring it back into frame
		setupNewPoint(l, glm::vec3(0, 1, 0));
		return true;
	}
	else if (l->points.back().pos.x > 1) {
		l->points.back().pos.x = 1;		//bring it back into frame
		setupNewPoint(l, glm::vec3(0, 1, 0));
		return true;
	}
	if (l->points.back().pos.y < -1) {
		l->points.back().pos.y = -1;		//bring it back into frame
		setupNewPoint(l, glm::vec3(1, 0, 0));
		return true;
		;
	}
	else if (l->points.back().pos.y > 1) {
		l->points.back().pos.y = 1;		//bring it back into frame
		setupNewPoint(l, glm::vec3(1, 0, 0));
		return true;
	}
	return false;
}

void LineManager::setupNewPoint(Line* l, glm::vec3 otherHeading) {

	//Add a new point to move forward
	l->newHead();

	//and reflect the heading
	l->heading = getNewHeading(l->heading, otherHeading);
}


glm::vec3 LineManager::getNewHeading(glm::vec3 headingOriginal, glm::vec3 headingOther) {

	//get the normal of the other segment
	glm::vec3 norm = glm::vec3(headingOther.y, -headingOther.x, 0);	//apparently direction of norm does not matter 

	glm::vec3 reflection = glm::reflect(headingOriginal, norm);
	
	return glm::normalize(reflection);

}


/*
void LineManager::initEvenlySpaced(int lineCount) {
	//initialize one line to start at the center of the screen
	if (lineCount == 1) {
		lines.push_back(Line(.5, .5));
		return;
	}

	//or initialize n lines, starting at positions evenly spaced in a circle around the center of the screen
	float theta = 2 * M_PI / lineCount;
	glm::vec3 position(0.f, INIT_CIRCLE_RADIUS, 0.f);
	glm::mat4 trans(1.0f);
	trans = glm::rotate(trans, theta, glm::vec3(0.0, 0.0, 1.0));	//create the rotation matrix
	for (int i = 0; i < lineCount; i++) {	
		std::cout << "Position: " << position.x << ", " << position.y << std::endl;
		lines.push_back(Line(position.x + .5, position.y + .5));
		position = glm::vec3(trans * glm::vec4(position, 1));		//use the rotation matrix to rotate the position around an origin
	}
	return;

}
*/
