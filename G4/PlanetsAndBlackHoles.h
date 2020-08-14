#pragma once
#include "Vector2.h"
#include "Matrix.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include "GameShell.h"
#include "hgesprite.h"
#include "GravityManager.h"

class Planet
{
public:
	Planet();
	~Planet();
	Planet(Vector2 newCenter, double newWeight, double newRange, double newRadius, bool newRepel, bool newBlackHole);
	void calculateDistance(Vector2 objectPosition, double* distanceFromPlanetPow2, Vector2 objectDimension);
	void calculateAngle(Vector2 objectPosition, double* angleWithPlanet);
	Vector2 calculateGravity(Vector2 objectPosition, double objectWeight, double* distanceFromPlanetPow2, double gravityAcceleration);
	bool calculateCollision(Vector2 objectDimension, double distanceFromPlanetPow2);
	bool calculateInRange(double distanceFromPlanetPow2);
	void render();
	void setCentre(Vector2 newCentre);
	void setWeight(double newWeight);
	void setRange(double newRange);
	void setRadius(double newRadius);
	void setRepel(bool newRepel);
	void setBlackHole(bool newBlackHole);
	bool isRepellable();
	Vector2 getCentre();
	double getWeight();
	double getRadius();
	bool getRepel();
	bool getBlackHole();
private:
	void loadSprites();
	Vector2 centre;
	double weight;
	double range; //Should be proportional to force, if negative then infinite
	double radius;
	double rotation;
	bool repel;
	int spriteNumber;
	bool blackHole;
	hgeSprite* planetSprite;
};