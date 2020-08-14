#include "Trees.h"

void Tree::init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{	
	dimension = Vector2(24, 47);
	instantVelocity = Vector2(0, 0);
	currentAngle = 0;
	nearestPlanetIndex = 0;
	position = Vector2(0, 0);
	isCaughtByPlanet = false;
	isCollidingWithPlatform = false;
	distanceFromPlanetPow2 = vector<double>(currentPlanetsNumber);
	angleWithPlanet = vector<double>(currentPlanetsNumber);
	distanceFromPlatformPow2 = vector<double>(currentPlatformsNumber);
	angleWithPlatform = vector<double>(currentPlatformsNumber);
	spriteNumber = (rand() % 7) + 1;
	if (hasToLoadSprites)
	{
		Sprite1 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree1.png"), 0, 0, 206, 442);
		Sprite1->SetHotSpot(206 / 2.0, 442 / 2.0);
		Sprite2 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree2.png"), 0, 0, 206, 442);
		Sprite2->SetHotSpot(206 / 2.0, 442 / 2.0);
		Sprite3 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree3.png"), 0, 0, 206, 442);
		Sprite3->SetHotSpot(206 / 2.0, 442 / 2.0);
		Sprite4 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree4.png"), 0, 0, 206, 442);
		Sprite4->SetHotSpot(206 / 2.0, 442 / 2.0);
		Sprite5 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree5.png"), 0, 0, 206, 442);
		Sprite5->SetHotSpot(206 / 2.0, 442 / 2.0);
		Sprite6 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Tree6.png"), 0, 0, 206, 442);
		Sprite6->SetHotSpot(206 / 2.0, 442 / 2.0);
		hasToLoadSprites = false;
	}
}

void Tree::update(vector<Tree>* Trees, int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	if (position.x == 0 && position.y == 0)
		generateNewReandomPosition(Trees, currentPlanetsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	if (!isCaughtByPlanet)
	{
		//Reset
		instantVelocity = Vector2(0, 0);

		//Calculate InstantVelocity for Player (Sum of effects method abandoned? one ruling gravity at the time method abandoned?)
		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			(*planets)[i].calculateDistance(position, &distanceFromPlanetPow2[i], dimension);
			(*planets)[i].calculateAngle(position, &angleWithPlanet[i]);
			instantVelocity += (*planets)[i].calculateGravity(position, -1, &distanceFromPlanetPow2[i], gravityAcceleration);
		}
		for (int i = 0; i < currentPlatformsNumber; i++)
		{
			(*platforms)[i].calculateAngle(position, &angleWithPlatform[i]);
		}

		currentAngle = atan2((-instantVelocity.x), (-instantVelocity.y));

		if (instantVelocity.x == 0 || instantVelocity.y == 0)
			generateNewReandomPosition(Trees, currentPlanetsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);

		//Collisions With Planets and Platforms
		for (int n = 0; n < currentPlanetsNumber; n++)
		{
			if ((*planets)[n].calculateCollision(dimension, distanceFromPlanetPow2[n]))
			{
				if ((*planets)[n].getBlackHole() || (*planets)[n].getRepel()) //If BlackHole or Repellent
				{
					instantVelocity = Vector2(0, 0);
					generateNewReandomPosition(Trees, currentPlanetsNumber, LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
					break;
				}
				else //If Planet
				{
					//Now we can choose if let the Player detatch from a planet only by jumping, or just when forces are high enough to catch him.
					//So we can stop the gravity effect on him, or leave it, stop it's velocity and set his weight.
					isCaughtByPlanet = true;
					currentAngle = angleWithPlanet[n];
					dimension *= (*planets)[n].getRadius() / 84;
					position = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + (dimension.y * 0.88)) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + (dimension.y * 0.88)) * cos(currentAngle)));
					nearestPlanetIndex = n;
					break;
				}
			}
			isCaughtByPlanet = false;
		}

		GravityManager::normalizeAngle(&currentAngle);
	}
	if (!isCaughtByPlanet)
		position += instantVelocity;
}

void Tree::render()
{
	if (isCaughtByPlanet)
	{
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
		
		if (spriteNumber == 1)
			Sprite1->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		else if (spriteNumber == 2)
			Sprite2->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		else if (spriteNumber == 3)
			Sprite3->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		else if (spriteNumber == 4)
			Sprite4->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		else if (spriteNumber == 5)
			Sprite5->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
		else //if (spriteNumber == 6 || spriteNumber == 7)
			Sprite6->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
	}
	else
	{
		GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
		GameShell::print(1, 500, 300, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Generating Chests and Trees");
	}
}

void Tree::generateNewReandomPosition(vector<Tree>* Trees, int currentPlanetsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	dimension = Vector2(26, 56);

	bool newCentreCalculated = false;
	int numberOfAttempts = 0;
	srand(time(NULL));
	int i2 = 0;
	while (!newCentreCalculated)
	{
		Vector2 newCentre = Vector2((rand() % (LevelSizeXPositive - LevelSizeXNegative) - ((LevelSizeXPositive - LevelSizeXNegative) / 2)), (rand() % (LevelSizeYPositive - LevelSizeYNegative) - ((LevelSizeYPositive - LevelSizeYNegative) / 2)));
		//Avoids getting stuck in the while
		int numberOfSucesses = 0;
		if (numberOfAttempts < 100)
		{
			for (int i = 0; i < currentPlanetsNumber * 2; i++)
			{
				i2 = i;
				if (sqrt(pow((*Trees)[i].getPosition().x - newCentre.x, 2) + pow((*Trees)[i].getPosition().y - newCentre.y, 2)) > sqrt((dimension.x + dimension.y) * 1.3))
					numberOfSucesses++;
				//if (abs(newCentre.x - ((*Trees)[i].getPosition().x)) > dimension.x * 3 || abs(newCentre.y - abs((*Trees)[i].getPosition().y)) > dimension.y * 3)
				else
				{
					numberOfAttempts++;
					break;
				}
			}
			if (numberOfSucesses == (currentPlanetsNumber * 2))
			{
				position = newCentre;
				newCentreCalculated = true;
			}
		}
		else
		{
			position = newCentre;
			newCentreCalculated = true;
		}
	}
}

Vector2 Tree::getPosition()
{
	return position;
}