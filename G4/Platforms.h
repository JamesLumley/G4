#pragma once
#include "Vector2.h"
#include "GravityManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Platform
{
public:
	Platform();
	Platform(Vector2 newCenter, double newWeight, double newRange, Vector2 newDimension, Vector2 newDirection);
	//void calculateDistance(Vector2 playerPosition, Vector2 playerDimension);
	void calculateAngle(Vector2 playerPosition, double* angleWithPlatform);
	Vector2 calculateGravity(Vector2 playerPosition, double objectWeight);
	bool calculateCollision(Vector2 playerPosition, Vector2 playerDimension);
//private:
	Vector2 centre;
	Vector2 direction;
	double weight;
	double range; //Should be proportional to force, if negative then infinite
	//double Direction; ???
	Vector2 dimension;
	bool collidingWithTop;
	bool collidingWithBottom;
	bool collidingWithRight;
	bool collidingWithLeft;
};