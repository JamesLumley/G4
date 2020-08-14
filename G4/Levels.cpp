#include "Levels.h"
#include "AudioManager.h"
#include "GameStates.h"

void Levels::init()
{
	SpeedUp = false;
	if (currentPlayersNumber > maxPlayersNumber)
		currentPlayersNumber = maxPlayersNumber;
	if (currentEnemiesNumber > maxEnemiesNumber)
		currentEnemiesNumber = maxEnemiesNumber;
	if (currentPlanetsNumber > maxPlanetsNumber)
		currentPlanetsNumber = maxPlanetsNumber;
	if (currentBlackHolesNumber + currentPlanetsNumber > maxPlanetsNumber)
		currentBlackHolesNumber = 0;
	if (currentPlatformsNumber > maxPlatformsNumber)
		currentPlatformsNumber = maxPlatformsNumber;
	if (currentStarsChestsNumber > maxStarsChestsNumber)
		currentStarsChestsNumber = maxStarsChestsNumber;

	srand(time(NULL));
	if (isLevelRandomGenerated)
	{
		//Generate Random Planets
		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			planets[i].setRadius((rand() % 148) + 33);
			bool newCentreCalculated = false;
			int numberOfAttempts = 0;
			int numberOfOverAttempts = 0;
			Vector2 newCentre;
			//Creates near planets
			while (!newCentreCalculated)
			{
				newCentre = Vector2(((LevelSizeXPositive - LevelSizeXNegative) * 0.87) * ((rand() % 202 - 101) / 200.0), ((LevelSizeYPositive - LevelSizeYNegative) * 0.87) *  ((rand() % 202 - 101) / 200.0));
				//Avoids getting stuck in the while
				if (numberOfAttempts < 500 && i != 0)
				{
					int numberOfSucesses = 0;
					for (int n = i - 1; n >= 0; n--)
					{
						if (sqrt(pow(planets[n].getCentre().x - newCentre.x, 2) + pow(planets[n].getCentre().y - newCentre.y, 2)) > (planets[n].getRadius() + planets[i].getRadius()) * 1.355)
							numberOfSucesses++;
						else
						{
							numberOfAttempts++;
							break;
						}
					}
					if (numberOfSucesses == i)
					{
						//Second Check, avoid too far planets
						for (int n = i - 1; n >= 0; n--)
						{
							if (!planets[n].getBlackHole())
							{
								if (sqrt(pow(planets[n].getCentre().x - newCentre.x, 2) + pow(planets[n].getCentre().y - newCentre.y, 2)) < (planets[n].getRadius() + planets[i].getRadius()) * PlanetsMaxDistance)
								{
									planets[i].setCentre(newCentre);
									newCentreCalculated = true;
									break;
								}
							}
						}
						if (!newCentreCalculated)
							numberOfAttempts++;
					}
				}
				else
				{
					newCentreCalculated = true;
				}
			}
			planets[i].setWeight(planets[i].getRadius());
			planets[i].setRange(900);
			if (rand() % 6 == 1)
			{
				if (abs(planets[i].getCentre().x) > 832 && planets[i].getCentre().y > 468)
					planets[i].setRepel(true);
			}
			else
				planets[i].setRepel(false);
			if (rand() % 4 == 1)
			{
				if (abs(planets[i].getCentre().x) > 832 && abs(planets[i].getCentre().y) > 468)
				{
					planets[i].setBlackHole(true);
					planets[i].setRepel(false);
				}
			}
		}
	}
	for (int i = 0; i < currentPlayersNumber; i++)
	{
		players[i].init(currentPlanetsNumber, currentPlatformsNumber, i * 80, i * 80, currentPlayersNumber, i + 1, playerLivesNumber);
	}
	for (int i = 0; i < currentEnemiesNumber; i++)
	{
		enemies[i].init(currentPlanetsNumber, currentPlatformsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}

	for (int i = 0; i < currentPlatformsNumber; i++)
	{
		platforms[i].weight = 1;
		platforms[i].range = 1000;
	}
	//Automatically Decides the number of cloud, based on the Level Size
	currentCloudsNumber = (LevelSizeXPositive - LevelSizeXNegative + LevelSizeYPositive - LevelSizeYNegative) / 100;
	if (currentCloudsNumber > maxCloudsNumber)
		currentCloudsNumber = maxCloudsNumber;
	for (int i = 0; i < currentCloudsNumber; i++)
	{
		clouds[i].init(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}
	for (int i = 0; i < currentStarsChestsNumber; i++)
	{
		starChests[i].init(currentPlanetsNumber, currentPlatformsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}
	for (int i = 0; i < currentPlanetsNumber * 2; i++)
	{
		trees[i].init(currentPlanetsNumber, currentPlatformsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}

	invertPlanetTimer = -1;
	invertGTimer = -1;
	vibrationDelay = 0;

	controller = new XboxController(1);

	InvertGEffect = GameShell::hge->Effect_Load("Contents/InvertG.wav");

	AudioManager::StartBackgroundSound();
}

bool Levels::update()
{
	invertPlanetTimer++;
	invertGTimer++;
	vibrationDelay--;

	if (GameShell::GetKeyState(HGEK_Y) == GameShell::KeyState_Released)
		SpeedUp = !SpeedUp;

	int repelNumber = 0;

	if (vibrationDelay == 0)
	{
		controller = new XboxController(1);
		controller->vibrate(0, 0);
		controller = new XboxController(2);
		controller->vibrate(0, 0);
	}
	for (int i = 0; i < currentPlanetsNumber; i++)
	{
		if (planets[i].getRepel())
			repelNumber++;
	}
	if (repelNumber < currentPlanetsNumber / 3)
	{
		if (currentPlayersNumber == 2 && invertGTimer > 400)
		{
			if (players[0].getInvertingGravity())
			{
				if (players[1].getColliding())
				{
					int n = players[1].getCollidingPlanetIndex();
					if (planets[n].isRepellable())
					{
						GameShell::hge->Effect_PlayEx(InvertGEffect, 80, 0, 1, false);
						planets[n].setRepel(!planets[n].getRepel());
						players[1].setInvertingGravity();
					}
				}
				invertGTimer = 0;
			}
			else if (players[1].getInvertingGravity())
			{
				if (players[0].getColliding())
				{
					int n = players[0].getCollidingPlanetIndex();
					if (planets[n].isRepellable())
					{
						GameShell::hge->Effect_PlayEx(InvertGEffect, 80, 0, 1, false);
						planets[n].setRepel(!planets[n].getRepel());
						players[0].setInvertingGravity();
					}
				}
				invertGTimer = 0;
			}
		}
	}

	if (invertPlanetTime > 0 && invertPlanetTimer > invertPlanetTime / currentPlanetsNumber)
	{
		int n = rand() % currentPlanetsNumber;
		if (planets[n].isRepellable())
		{
			if (repelNumber > currentPlanetsNumber / 3)
				planets[n].setRepel(false);
			else
				planets[n].setRepel(true);
			if (currentPlayersNumber >= 1 && players[0].getColliding() && players[0].getCollidingPlanetIndex() == n)
			{
				players[0].setInvertingGravity();
				vibrationDelay = 30;
				controller = new XboxController(1);
				controller->vibrate(30000, 30000);
			}
			if (currentPlayersNumber == 2 && players[1].getColliding() && players[1].getCollidingPlanetIndex() == n)
			{
				players[1].setInvertingGravity();
				vibrationDelay = 30;
				controller = new XboxController(2);
				controller->vibrate(30000, 30000);
			}
			for (int i = 0; i < currentEnemiesNumber; i++)
			{
				if (n == enemies[i].getCollidingPlanetIndex())
					enemies[i].setInvertingGravity();
			}
			for (int i = 0; i < currentStarsChestsNumber; i++)
			{
				if (n == starChests[i].getCollidingPlanetIndex())
					starChests[i].setInvertingGravity();
			}
		}
		invertPlanetTimer = 0;
	}
	if (!SpeedUp)
	{
		for (int i = 0; i < currentPlanetsNumber * 2; i++)
		{
			trees[i].update(&trees, currentPlanetsNumber, currentPlatformsNumber, &planets, &platforms, gravityAcceleration, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
		}
	}
	for (int i = 0; i < currentStarsChestsNumber; i++)
	{
		starChests[i].update(currentPlanetsNumber, currentPlatformsNumber, &planets, &platforms, gravityAcceleration, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}
	for (int i = 0; i < currentEnemiesNumber; i++)
	{
		enemies[i].update(currentPlanetsNumber, currentPlatformsNumber, &planets, &platforms, gravityAcceleration, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	}
	for (int i = 0; i < currentPlayersNumber; i++)
	{
		players[i].update(currentPlanetsNumber, currentPlatformsNumber, &planets, &platforms, gravityAcceleration, currentEnemiesNumber, &enemies, currentStarsChestsNumber, &starChests, &LevelSizeXNegative, &LevelSizeXPositive, &LevelSizeYNegative, &LevelSizeYPositive);
	}
	if (!SpeedUp)
	{
		for (int i = 0; i < currentCloudsNumber; i++)
		{
			clouds[i].update(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
		}
	}

	//Camera
	if (currentPlayersNumber > 0)
	{
		if (currentPlayersNumber == 2)
		{
			cameraCentre = players[0].getPosition() - ((players[0].getPosition() - players[1].getPosition()) / 2);

			distance = players[0].getPosition() - players[1].getPosition();
			if (distance.x < 0)
				distance.x = -distance.x;
			if (distance.y < 0)
				distance.y = -distance.y;
			if (distance.x > ixResolution / 1.34 || distance.y > iyResolution / 1.34)
			{
				if (ixResolution / 1.34 / distance.x > iyResolution / 1.34 / distance.y)
					scale = iyResolution / 1.34 / distance.y;
				else
					scale = ixResolution / 1.34 / distance.x;
				scale *= 0.85;
			}
			else
				scale = 0.85;

			if (players[1].getLives() == 0)
			{
				scale = 0.85;
				cameraCentre = players[0].getPosition();
			}
			if (players[0].getLives() == 0)
			{
				scale = 0.85;
				cameraCentre = players[1].getPosition();
			}
		}
		else //if (PlayersNumber == 1)
		{
			scale = 0.85;
			cameraCentre = players[0].getPosition();
		}
		if (GameShell::GetKeyState(HGEK_M) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			scale = 0.19;
		else
		{
			//Camera Level Borders
			if (cameraCentre.x < (LevelSizeXNegative + (ixResolution / 2)) * scale)
				cameraCentre.x = (LevelSizeXNegative + (ixResolution / 2)) * scale;
			else if (cameraCentre.x > (LevelSizeXPositive - (ixResolution / 2)) * scale)
				cameraCentre.x = (LevelSizeXPositive - (ixResolution / 2)) * scale;
			if (cameraCentre.y < (LevelSizeYNegative + (iyResolution / 2)) * scale)
				cameraCentre.y = (LevelSizeYNegative + (iyResolution / 2)) * scale;
			else if (cameraCentre.y > (LevelSizeYPositive - (iyResolution / 2)) * scale)
				cameraCentre.y = (LevelSizeYPositive - (iyResolution / 2)) * scale;
		}
	}
	
	return State::update();
}

void Levels::render()
{
	//Set Camera
	GameShell::hge->Gfx_SetTransform(0, 0, (-cameraCentre.x*scale) + (ixResolution / 2), (-cameraCentre.y*scale) + (iyResolution / 2), 0, scale,scale);

	//Draw Clouds
	if (!SpeedUp)
	{
		for (int i = 0; i < currentCloudsNumber; i++)
		{
			clouds[i].render();
		}
	}
	//Draw Trees
	if (!SpeedUp)
	{
		for (int i = 0; i < currentPlanetsNumber * 2; i++)
		{
			GameShell::hge->Gfx_SetTransform(0, 0, (-cameraCentre.x*scale) + (ixResolution / 2), (-cameraCentre.y*scale) + (iyResolution / 2), 0, scale, scale);
			trees[i].render();
		}
	}
	GameShell::hge->Gfx_SetTransform(0, 0, (-cameraCentre.x*scale) + (ixResolution / 2), (-cameraCentre.y*scale) + (iyResolution / 2), 0, scale, scale);
	//Draw Planets
	for (int i = 0; i < currentPlanetsNumber; i++)
	{
		planets[i].render();
	}
	//Draw Platforms
	for (int i = 0; i < currentPlatformsNumber; i++)
	{
		Vector2 PlatformsPoints[4];
		PlatformsPoints[0] = Vector2(platforms[i].centre.x - (platforms[i].dimension.x / 2), platforms[i].centre.y - (platforms[i].dimension.y / 2));
		PlatformsPoints[1] = Vector2(platforms[i].centre.x - (platforms[i].dimension.x / 2), platforms[i].centre.y + (platforms[i].dimension.y / 2));
		PlatformsPoints[2] = Vector2(platforms[i].centre.x + (platforms[i].dimension.x / 2), platforms[i].centre.y + (platforms[i].dimension.y / 2));
		PlatformsPoints[3] = Vector2(platforms[i].centre.x + (platforms[i].dimension.x / 2), platforms[i].centre.y - (platforms[i].dimension.y / 2));

		platform->Render4V(PlatformsPoints[3].x, PlatformsPoints[3].y, PlatformsPoints[0].x, PlatformsPoints[0].y, PlatformsPoints[1].x, PlatformsPoints[1].y, PlatformsPoints[2].x, PlatformsPoints[2].y);

		//GameShell::Vertex v3[8];
		//unsigned int index3[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
		//for (unsigned int iIndex = 0; iIndex < ARRAY_LENGTH(index3); iIndex++)
		//{
		//	v3[iIndex].Position.x = PlatformsPoints[index3[iIndex]].x;
		//	v3[iIndex].Position.y = PlatformsPoints[index3[iIndex]].y;
		//	v3[iIndex].col = GameShell::RGBTOCOLOR(255, 255, 255);
		//}
		//GameShell::RenderLines(v3, ARRAY_LENGTH(v3) / 2);
	}
	//Draw Chests
	for (int i = 0; i < currentStarsChestsNumber; i++)
	{
		starChests[i].render(&planets);
		GameShell::hge->Gfx_SetTransform(0, 0, (-cameraCentre.x*scale) + (ixResolution / 2), (-cameraCentre.y*scale) + (iyResolution / 2), 0, scale, scale);
	}
	//Draw Players
	for (int i = 0; i < currentPlayersNumber; i++)
	{
		players[i].render();
		GameShell::hge->Gfx_SetTransform(0, 0, (-cameraCentre.x*scale) + (ixResolution / 2), (-cameraCentre.y*scale) + (iyResolution / 2), 0, scale, scale);
	}
	//Draw Enemies
	for (int i = 0; i < currentEnemiesNumber; i++)
	{
		enemies[i].render();
	}

	//Level Limits
	Vector2 limitsPoints[4];
	limitsPoints[0] = Vector2(LevelSizeXNegative, LevelSizeYNegative);
	limitsPoints[1] = Vector2(LevelSizeXNegative, LevelSizeYPositive);
	limitsPoints[2] = Vector2(LevelSizeXPositive, LevelSizeYPositive);
	limitsPoints[3] = Vector2(LevelSizeXPositive, LevelSizeYNegative);
	GameShell::Vertex limitsVertices[8];
	unsigned int limitsIndex[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
	for (unsigned int iIndex = 0; iIndex < ARRAY_LENGTH(limitsIndex); iIndex++)
	{
		limitsVertices[iIndex].position.x = limitsPoints[limitsIndex[iIndex]].x;
		limitsVertices[iIndex].position.y = limitsPoints[limitsIndex[iIndex]].y;
		limitsVertices[iIndex].col = GameShell::RGBTOCOLOR(255, 255, 255);
	}
	GameShell::renderLines(limitsVertices, ARRAY_LENGTH(limitsVertices) / 2);

	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);

	if (!(GameShell::GetKeyState(HGEK_M) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) && scale < 0.33)
		GameShell::print(1, 535, 290, 0, GameShell::RGBATOCOLOR(0, 255, 0, 255), "HEY COME BACK!!!");
}

void Levels::exit()
{

}