#pragma once
#include <vector>
#include "Matrix.h"
#include "hgeanim.h"
#include "hgesprite.h"
#include "GameShell.h"
#include "PlanetsAndBlackHoles.h"
#include "Platforms.h"
#include "XboxController.h"
#include "Enemies.h"
#include "StarsChests.h"
using namespace std;

class Player
{
public:
	~Player();
	void init(int currentPlanetsNumber, int currentPlatformsNumber, float positionX, float positionY, int newPlayersNumber, int newPlayerNumber, int newlives);
	void update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int currentEnemiesNumber, vector<Enemy>* enemies, int currentStarsChestsNumber, vector<StarChest>* (maxStarsChestsNumber), double* LevelSizeXNegative, double* LevelSizeXPositive, double* LevelSizeYNegative, double* LevelSizeYPositive);
	void render();
	bool checkDeathByEnemy(Enemy &enemy);
	bool checkEnemyKilled(Enemy &enemy);
	Vector2 getPosition();
	bool getColliding();
	int getCollidingPlanetIndex();
	int getLives();
	int getScore();
	int getCollectedStars();
	bool getInvertingGravity();
	void setInvertingGravity();
private:
	void setAsSecondPlayer();
	Vector2 position;
	Vector2 dimension;
	Vector2 dimensionBK;
	Vector2 instantVelocity;
	Vector2 currentMovingVelocity;
	Vector2 currentMovingVelocityBK;
	Vector2 walkingVelocity;
	Vector2 runningVelocity;
	Vector2 currentGravity;
	double weight;
	double currentAngle;
	double currentGravityAngle;
	vector<double> distanceFromPlanetPow2;
	vector<double> angleWithPlanet;
	vector<double> distanceFromPlatformPow2;
	vector<double> angleWithPlatform;
	int nearestPlanetIndex;
	bool isCollidingWithLimits;
	bool isCaughtByPlanet;
	bool isCollidingWithPlatform;
	bool dead;
	bool isSmashing;
	int deathDelay;
	int stepDelay;
	int playersNumber;
	int playerNumber;
	int lives;
	bool resetPosition;
	bool resetAngle;
	int resetTimer;
	int score;
	int collectedStars;
	int vibrationDelay;
	int jumpDelay;
	int FlyingTime;
	double currentAngleBK;
	bool ResetMovement;
	double StandardizeJump;

	//Controls
	XboxController* controller;
	bool isDebugMode;
	bool rightPressed;
	bool leftPressed;
	bool jumpPressed;
	bool smashPressed;
	bool runPressed;

	//Jumping
	Vector2 jump;
	double CaptureRadius;
	double jumpPower;
	double jumpPowerBK;
	bool isJumping;
	bool isFalling;
	bool inertialRight;
	bool inertialLeft;
	bool inertia;
	bool interactPressed;

	//TEMP
	//bool bIsJetPackActive;

	//Effects:
	hgeAnimation* playerRun;
	hgeAnimation* playerJump;
	hgeAnimation* playerLand;
	hgeAnimation* playerRoll;
	hgeAnimation* playerActiveAn;
	bool flipX;
	bool flipY;
	HEFFECT footStep;
	HEFFECT starTakenEffect;
	HEFFECT deathEffect;
	HEFFECT SmashEffect;

	//Collision
	float headRadius;
	Vector2 centerHead;
	float feetRadius;
	Vector2 centerFeet;
};