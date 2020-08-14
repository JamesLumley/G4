#pragma once
#include <vector>
#include "Matrix.h"
#include "hgeanim.h"
#include "hgesprite.h"
#include "GameShell.h"
#include "PlanetsAndBlackHoles.h"
//#include "BlackHole.h"
#include "Platforms.h"

using namespace std;

class Enemy
{
public:
	Enemy();
	void init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, double LevelSizeXNegative, double LevelSizeXPositive, double LevelSizeYNegative, double LevelSizeYPositive);
	void render();
	Vector2 getCenterHead();
	int getCollidingPlanetIndex();
	float getHeadRadius();
	void setDead(bool death);
	bool isDead();
	void setInvertingGravity();
private:
	void generateNewReandomPosition(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	Vector2 position;
	Vector2 dimension;
	Vector2 instantVelocity;
	Vector2 movingVelocity;
	Vector2 currentGravity;
	double weight;
	double currentGravityAngle;
	double currentAngle;
	vector<double> distanceFromPlanetPow2;
	vector<double> angleWithPlanet;
	vector<double> distanceFromPlatformPow2;
	vector<double> angleWithPlatform;
	int nearestPlanetIndex;
	bool isCollidingWithLimits;
	bool isCaughtByPlanet;
	bool isCollidingWithPlatform;
	bool flipX;
	bool dead;
	bool deadSoundPlayed;
	int deathDelay;
	
	HEFFECT deathSound;

	bool isJumping;
	bool isFalling;
	bool isJumpable;
	int jumpDelay;
	Vector2 jump;
	double jumpPower;
	double jumpPowerBK;

	//Enemy Variables
	hgeAnimation* sprite1;
	hgeAnimation* sprite2;
	hgeAnimation* sprite3;
	hgeAnimation* sprite4;
	hgeAnimation* sprite5;
	hgeAnimation* sprite6;
	hgeAnimation* sprite7;
	hgeAnimation* sprite8;
	hgeAnimation* sprite9;
	hgeAnimation* sprite10;

	//Collision
	float headRadius;
	Vector2 centerHead;

	int spriteNumber;
	bool hasToLoadFiles;
};