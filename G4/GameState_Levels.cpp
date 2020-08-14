#include "GameStates.h"

void GameState_LevelTutorial::init()
{
	isLevelRandomGenerated = false;
	LevelSizeXPositive = 1350;
	LevelSizeXNegative = -1350;
	LevelSizeYPositive = 1350;
	LevelSizeYNegative = -1350;
	currentPlayersNumber = 1;
	currentEnemiesNumber = 3;
	currentPlanetsNumber = 5;
	currentBlackHolesNumber = 0;
	currentPlatformsNumber = 1;
	currentStarsChestsNumber = 1;
	invertPlanetTime = -1;
	playerLivesNumber = 3;
	gravityAcceleration = 13;
	PlanetsMaxDistance = 0;

	Levels::init();

	planets[2].setBlackHole(true);
	planets[0].setCentre(Vector2(ixResolution * 0.175, iyResolution * 0.63));
	planets[1].setCentre(Vector2(ixResolution * 0.8, iyResolution * 0.50));
	planets[2].setCentre(Vector2(ixResolution * 0.79, iyResolution * 0.92));
	planets[3].setCentre(Vector2(ixResolution * 0.4, iyResolution * 0.15));
	planets[4].setCentre(Vector2(ixResolution * 0.68, iyResolution * 0.04));
	planets[0].setRadius(150);
	planets[1].setRadius(65);
	planets[2].setRadius(90);
	planets[3].setRadius(28);
	planets[4].setRadius(70);
	planets[3].setRepel(true);
	planets[2].setWeight(70);

	platforms[0].centre = Vector2(ixResolution * -0.38, iyResolution * 0.73);
	platforms[0].dimension = Vector2(400, 40);
	for (int i = 0; i < currentPlanetsNumber; i++)
	{
		planets[i].setWeight(planets[i].getRadius());
		planets[i].setRange(1300);
	}

	State::init();

	platform = new hgeSprite(GameShell::hge->Texture_Load("Contents/Platform.png"), 0.0f, 0.0f, 480, 86);
}

bool GameState_LevelTutorial::update()
{

	if (players[0].getLives() == 0)
	{
		GameState_NextLevelAndGameOver::nextState = 0;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}
	else if (players[0].getCollectedStars() == 1)
	{
		GameState_NextLevelAndGameOver::nextState = 2;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}

	Levels::update();
	return true;
}

void GameState_LevelTutorial::render()
{
	Levels::render();
	//Draw HUD
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	GameShell::print(1, 1125, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Lives %d", players[0].getLives());
	GameShell::print(1, 10, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Score %d", players[0].getScore());
	GameShell::print(1, 535, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Stars %d/1", players[0].getCollectedStars());
}

void GameState_Level2::init()
{
	isLevelRandomGenerated = true;
	LevelSizeXPositive = 2500;
	LevelSizeXNegative = -2500;
	LevelSizeYPositive = 1800;
	LevelSizeYNegative = -1800;
	currentPlayersNumber = 1;
	currentEnemiesNumber = 38;
	currentPlanetsNumber = 30;
	currentBlackHolesNumber = 3;
	currentPlatformsNumber = 0;
	currentStarsChestsNumber = 4;
	invertPlanetTime = 5000;
	platform = new hgeSprite(GameShell::hge->Texture_Load("Contents/Platform.png"), 0.0f, 0.0f, 480, 86);
	playerLivesNumber = 4;
	gravityAcceleration = 14;
	PlanetsMaxDistance = 4;

	Levels::init();
	State::init();
}

bool GameState_Level2::update()
{
	if (players[0].getLives() == 0)
	{
		GameState_NextLevelAndGameOver::nextState = 0;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}
	else if (players[0].getCollectedStars() == 4)
	{
		GameState_NextLevelAndGameOver::nextState = 3;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}

	Levels::update();
	return true;
}

void GameState_Level2::render()
{
	Levels::render();
	//Draw HUD
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	GameShell::print(1, 1125, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Lives %d", players[0].getLives());
	GameShell::print(1, 10, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Score %d", players[0].getScore());
	GameShell::print(1, 535, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Stars %d/4", players[0].getCollectedStars());
}

void GameState_Level3::init()
{
	isLevelRandomGenerated = true;
	LevelSizeXPositive = 3100;
	LevelSizeXNegative = -3100;
	LevelSizeYPositive = 3100;
	LevelSizeYNegative = -3100;
	currentPlayersNumber = 1;
	currentEnemiesNumber = 70;
	currentPlanetsNumber = 80;
	//currentBlackHolesNumber = 0;
	currentPlatformsNumber = 0;
	currentStarsChestsNumber = 12;
	invertPlanetTime = 3000;
	platform = new hgeSprite(GameShell::hge->Texture_Load("Contents/Platform.png"), 0.0f, 0.0f, 480, 86);
	playerLivesNumber = 3;
	gravityAcceleration = 16;
	PlanetsMaxDistance = 7;

	Levels::init();
	State::init();
}

bool GameState_Level3::update()
{
	if (players[0].getLives() == 0)
	{
		GameState_NextLevelAndGameOver::nextState = 0;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}
	else if (players[0].getCollectedStars() == 12)
	{
		GameState_NextLevelAndGameOver::nextState = 4;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}

	Levels::update();
	return true;
}

void GameState_Level3::render()
{
	Levels::render();
	//Draw HUD
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	GameShell::print(1, 1125, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Lives %d", players[0].getLives());
	GameShell::print(1, 10, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Score %d", players[0].getScore());
	GameShell::print(1, 518, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Stars %d/12", players[0].getCollectedStars());
}

void GameState_LevelMultiplayer::init()
{
	isLevelRandomGenerated = true;
	LevelSizeXPositive = 2800;
	LevelSizeXNegative = -2800;
	LevelSizeYPositive = 2050;
	LevelSizeYNegative = -2050;
	currentPlayersNumber = 2;
	currentEnemiesNumber = 100;
	currentPlanetsNumber = 58;
	currentBlackHolesNumber = 3;
	currentPlatformsNumber = 0;
	currentStarsChestsNumber = 0;
	invertPlanetTime = 2500;
	platform = new hgeSprite(GameShell::hge->Texture_Load("Contents/Platform.png"), 0.0f, 0.0f, 480, 86);
	playerLivesNumber = 4;
	gravityAcceleration = 14;
	PlanetsMaxDistance = 5;

	Levels::init();
	State::init();
}

bool GameState_LevelMultiplayer::update()
{
	if (players[0].getLives() == 0 && players[1].getLives() == 0)
	{
		GameState_NextLevelAndGameOver::nextState = 0;
		GameShell::setState(new GameState_NextLevelAndGameOver);
	}
	Levels::update();
	return true;
}

void GameState_LevelMultiplayer::render()
{
	Levels::render();
	//Draw Lives
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	GameShell::print(1, 10, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "P1 Lives %d", (players)[0].getLives());
	GameShell::print(1, 230, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Score %d", players[0].getScore());
	GameShell::print(1, 890, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "P2 Lives %d", players[1].getLives());
	GameShell::print(1, 1110, 10, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "Score %d", players[1].getScore());
}