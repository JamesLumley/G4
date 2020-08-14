#pragma once
#include <vector>
#include "Matrix.h"
#include "hgesprite.h"
#include "GameShell.h"
#include "PlanetsAndBlackHoles.h"
#include "Platforms.h"
#include <time.h>
using namespace std;

class Tree
{
public:
	void init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void update(vector<Tree>* Trees, int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void render();
	void generateNewReandomPosition(vector<Tree>* Trees, int currentPlanetsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	Vector2 getPosition();
private:
	Vector2 position;
	Vector2 dimension;
	Vector2 instantVelocity;
	double currentAngle;
	int spriteNumber;
	vector<double> distanceFromPlanetPow2;
	vector<double> angleWithPlanet;
	vector<double> distanceFromPlatformPow2;
	vector<double> angleWithPlatform;
	int nearestPlanetIndex;
	bool isCaughtByPlanet;
	bool isCollidingWithPlatform;
	hgeSprite* Sprite1;
	hgeSprite* Sprite2;
	hgeSprite* Sprite3;
	hgeSprite* Sprite4;
	hgeSprite* Sprite5;
	hgeSprite* Sprite6;
	bool hasToLoadSprites;
};