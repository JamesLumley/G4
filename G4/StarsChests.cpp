#include "StarsChests.h"

void StarChest::init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	position = Vector2((rand() % (LevelSizeXPositive - LevelSizeXNegative) - ((LevelSizeXPositive - LevelSizeXNegative) / 2)), (rand() % (LevelSizeYPositive - LevelSizeYNegative) - ((LevelSizeYPositive - LevelSizeYNegative) / 2)));
	starPosition = Vector2(0, 0);
	dimension = Vector2(35, 30);
	radius = 5;
	weight = -1;
	instantVelocity = Vector2(0, 0);
	currentAngle = 0;
	nearestPlanetIndex = 0;
	isCaughtByPlanet = false;
	isCollidingWithPlatform = false;
	opened = false;
	starTaken = false;
	hasGenerated = false;
	distanceFromPlanetPow2 = vector<double>(currentPlanetsNumber);
	angleWithPlanet = vector<double>(currentPlanetsNumber);
	distanceFromPlatformPow2 = vector<double>(currentPlatformsNumber);
	angleWithPlatform = vector<double>(currentPlatformsNumber);
	chestClosed = new hgeSprite(GameShell::hge->Texture_Load("Contents/ChestClosed.png"), 0, 0, 510, 390);
	chestClosed->SetHotSpot(510 / 2.0, 390 / 2.0);
	chestOpened = new hgeSprite(GameShell::hge->Texture_Load("Contents/ChestOpened.png"), 0, 0, 510, 390);
	chestOpened->SetHotSpot(510 / 2.0, 390 / 2.0);
	star = new hgeSprite(GameShell::hge->Texture_Load("Contents/Star.png"), 0, 0, 414, 390);
	star->SetHotSpot(414 / 2.0, 390 / 2.0);
}

void StarChest::update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	if (!isCaughtByPlanet)
	{
		//Reset
		instantVelocity = Vector2(0, 0);

		//Calculate InstantVelocity for Player (Sum of effects method abandoned? one ruling gravity at the time method abandoned?)
		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			(*planets)[i].calculateDistance(position, &distanceFromPlanetPow2[i], dimension);
			(*planets)[i].calculateAngle(position, &angleWithPlanet[i]);
			instantVelocity += (*planets)[i].calculateGravity(position, weight, &distanceFromPlanetPow2[i], gravityAcceleration);
		}
		for (int i = 0; i < currentPlatformsNumber; i++)
		{
			(*platforms)[i].calculateAngle(position, &angleWithPlatform[i]);
		}

		currentAngle = atan2((-instantVelocity.x), (-instantVelocity.y));

		position += instantVelocity;

		if (instantVelocity.x == 0 || instantVelocity.y == 0)
			position = Vector2((rand() % (LevelSizeXPositive - LevelSizeXNegative) - ((LevelSizeXPositive - LevelSizeXNegative) / 2)), (rand() % (LevelSizeYPositive - LevelSizeYNegative) - ((LevelSizeYPositive - LevelSizeYNegative) / 2)));
		if (position.x > LevelSizeXPositive || position.x < LevelSizeXNegative || position.y > LevelSizeXPositive || position.y < LevelSizeXNegative)
			position = Vector2((rand() % (LevelSizeXPositive - LevelSizeXNegative) - ((LevelSizeXPositive - LevelSizeXNegative) / 2)), (rand() % (LevelSizeYPositive - LevelSizeYNegative) - ((LevelSizeYPositive - LevelSizeYNegative) / 2)));

		//Collisions With Planets and Platforms
		for (int n = 0; n < currentPlanetsNumber; n++)
		{
			if ((*planets)[n].calculateCollision(dimension, distanceFromPlanetPow2[n]))
			{
				if ((*planets)[n].getBlackHole() || (*planets)[n].getRepel()) //If BlackHole or Repellent
				{
					if (!hasGenerated || (*planets)[n].getBlackHole())
						position = Vector2((rand() % (LevelSizeXPositive - LevelSizeXNegative) - ((LevelSizeXPositive - LevelSizeXNegative) / 2)), (rand() % (LevelSizeYPositive - LevelSizeYNegative) - ((LevelSizeYPositive - LevelSizeYNegative) / 2)));
					break;
				}
				else //If Planet
				{
					//Now we can choose if let the Player detatch from a planet only by jumping, or just when forces are high enough to catch him.
					//So we can stop the gravity effect on him, or leave it, stop it's velocity and set his weight.
					isCaughtByPlanet = true;
					hasGenerated = true;
					weight = 1;
					currentAngle = angleWithPlanet[n];
					position = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + (dimension.y * 0.91)) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + (dimension.y * 0.91)) * cos(currentAngle)));
					starPosition = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + (dimension.y * 2.89)) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + (dimension.y * 2.89)) * cos(currentAngle)));
					nearestPlanetIndex = n;
					break;
				}
			}
			isCaughtByPlanet = false;
		}
		for (int n = 0; n < currentPlatformsNumber; n++)
		{
			if ((*platforms)[n].calculateCollision(position, dimension))
			{
				isCollidingWithPlatform = true;
				currentAngle = angleWithPlatform[n];
				//nearestPlanetIndex = i;
				break;
			}
			isCollidingWithPlatform = false;
		}

		GravityManager::normalizeAngle(&currentAngle);
	}
}

void StarChest::render(vector<Planet>* planets)
{
	if (hasGenerated)
	{
		//Draw Chest
		Vector2 points[4];
		points[0] = Vector2(dimension.x, dimension.y);
		points[1] = Vector2(dimension.x, -dimension.y);
		points[2] = Vector2(-dimension.x, -dimension.y);
		points[3] = Vector2(-dimension.x, dimension.y);

		Matrix transform = Matrix::CreateRotationZ(currentAngle) * Matrix::CreateTranslation(position.x, position.y, 0);

		for (unsigned int i = 0; i < 4; i++)
		{
			points[i] = Vector2::Transform(points[i], transform);
		}

		if (opened)
		{
			chestOpened->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		}
		else
			chestClosed->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);

		//Draw Star
		if (opened && !starTaken)
		{
			Vector2 points[4];
			points[0] = Vector2(dimension.x*1.18, dimension.y*1.18);
			points[1] = Vector2(dimension.x*1.18, -dimension.y*1.18);
			points[2] = Vector2(-dimension.x*1.18, -dimension.y*1.18);
			points[3] = Vector2(-dimension.x*1.18, dimension.y*1.18);

			Matrix transform = Matrix::CreateRotationZ(currentAngle) * Matrix::CreateTranslation(starPosition.x, starPosition.y, 0);

			for (unsigned int i = 0; i < 4; i++)
			{
				points[i] = Vector2::Transform(points[i], transform);
			}

			star->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		}
	}
	else
	{
		GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
		GameShell::print(1, 500, 300, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Generating Chests and Trees");
	}

}

void StarChest::setOpened(bool newOpened)
{
	opened = newOpened;
}

void StarChest::setStarTaken(bool newStarTaken)
{
	starTaken = newStarTaken;
}

Vector2 StarChest::getPosition()
{
	return position;
}

Vector2 StarChest::getStarPosition()
{
	return starPosition;
}

Vector2 StarChest::getDimensions()
{
	return dimension;
}
bool StarChest::getOpened()
{
	return opened;
}

bool StarChest::getStarTaken()
{
	return starTaken;
}

int StarChest::getCollidingPlanetIndex()
{
	return nearestPlanetIndex;
}

void StarChest::setInvertingGravity()
{
	isCaughtByPlanet = false;
}