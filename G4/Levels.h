#pragma once
#include "XboxController.h"
#include "GameStateManager.h"
#include "GameState_Menu.h"
#include "GameShell.h"
#include "Vector2.h"
#include "Matrix.h"
#include "PlanetsAndBlackHoles.h"
#include "Platforms.h"
#include "Enemies.h"
#include "StarsChests.h"
#include "Trees.h"
#include "Players.h"
#include "Clouds.h"
#include <time.h>
#include <math.h>
#define _USE_MATH_DEFINES
#include <vector>
using namespace std;

class Levels : public GameStateManager::PausibleState
{
public:
	Levels()
	{
		planets = vector<Planet>(maxPlanetsNumber);
		platforms = vector<Platform>(maxPlatformsNumber);
		clouds = vector<Cloud>(maxCloudsNumber);
		enemies = vector<Enemy>(maxEnemiesNumber);
		players = vector<Player>(maxPlayersNumber);
		trees = vector<Tree>(maxPlanetsNumber * 2);
		starChests = vector<StarChest>(maxStarsChestsNumber);
	}
	virtual void init();
	bool update();
	void render();
	void exit();
protected:
	static const int maxPlanetsNumber = 80;
	static const int maxPlatformsNumber = 25;
	static const int maxPlayersNumber = 2;
	static const int maxEnemiesNumber = 100;
	static const int maxCloudsNumber = 100;
	static const int maxStarsChestsNumber = 12;
	//double dDefaultGravityAngle; //Not Used
	//double dStandardGravity; //Not Used
	double gravityAcceleration;
	bool isLevelRandomGenerated;
	int currentPlanetsNumber;
	int currentBlackHolesNumber;
	int currentPlatformsNumber;
	int currentPlayersNumber;
	int currentEnemiesNumber;
	int currentCloudsNumber;
	int currentStarsChestsNumber;
	int playerLivesNumber;
	int invertPlanetTimer;
	int invertPlanetTime;
	int invertGTimer;
	double PlanetsMaxDistance;
	bool SpeedUp;
	vector<Player> players;
	vector<Enemy> enemies;
	vector<Planet> planets;
	vector<Platform> platforms;
	vector<Tree> trees;
	vector<StarChest> starChests;
	vector<Cloud> clouds;

	double LevelSizeXPositive;
	double LevelSizeXNegative;
	double LevelSizeYPositive;
	double LevelSizeYNegative;
	Vector2 distance;
	double scale;
	XboxController* controller;
	int vibrationDelay;
	Vector2 cameraCentre;
	HEFFECT InvertGEffect;
	hgeSprite* platform;
};