#pragma once
#include <vector>
#include "Matrix.h"
#include "hgesprite.h"
#include "GameShell.h"
#include "PlanetsAndBlackHoles.h"
#include "Platforms.h"
using namespace std;

class StarChest
{
public:
	void init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void render(vector<Planet>* planets);
	void setOpened(bool newOpened);
	void setStarTaken(bool newStarTaken);
	bool getStarTaken();
	bool getOpened();
	Vector2 getPosition();
	Vector2 getStarPosition();
	Vector2 getDimensions();
	int getCollidingPlanetIndex();
	void setInvertingGravity();
private:
	Vector2 position;
	Vector2 starPosition;
	Vector2 starHeight;
	Vector2 starRadius;
	Vector2 dimension;
	Vector2 instantVelocity;
	double radius;
	double weight;
	double currentAngle;
	bool hasGenerated;
	vector<double> distanceFromPlanetPow2;
	vector<double> angleWithPlanet;
	vector<double> distanceFromPlatformPow2;
	vector<double> angleWithPlatform;
	int nearestPlanetIndex;
	bool isCaughtByPlanet;
	bool isCollidingWithPlatform;
	bool opened;
	bool starTaken;
	bool hasToLoadSprites;
	hgeSprite* chestClosed;
	hgeSprite* chestOpened;
	hgeSprite* star;
};