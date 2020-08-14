#include "PlanetsAndBlackHoles.h"
#include <string>

Planet::Planet()
{
	centre = Vector2(0, 0);
	weight = 1;
	range = -1;
	radius = 1;
	blackHole = false;
	repel = false;
	//Sprites Probability
	if ((rand() % 11) == 0)
		spriteNumber = 9;
	else
		spriteNumber = (rand() % 8) + 1;

	loadSprites();
	
	rotation = ((rand() % 200) / 200.0) * M_PI;
}

Planet::~Planet()
{
	delete planetSprite;
}

Planet::Planet(Vector2 newCenter, double newWeight, double newRange, double newRadius, bool newRepel, bool newBlackHole)
{
	centre = newCenter;
	weight = newWeight;
	range = newRange;
	radius = newRadius;
	setRepel(newRepel);
	blackHole = newBlackHole;
}

void Planet::calculateDistance(Vector2 objectPosition, double* distanceFromPlanetPow2, Vector2 objectDimension)
{
	*distanceFromPlanetPow2 = pow(objectPosition.x - centre.x, 2) + pow(objectPosition.y - centre.y, 2);
}

void Planet::calculateAngle(Vector2 objectPosition, double* angleWithPlanet)
{
	*angleWithPlanet = atan2(objectPosition.x - centre.x, objectPosition.y - centre.y);
	GravityManager::normalizeAngle(&*angleWithPlanet);
}	

Vector2 Planet::calculateGravity(Vector2 objectPosition, double objectWeight, double* distanceFromPlanetPow2, double gravityAcceleration)
{
	if (objectWeight == -1)
		objectWeight = 1;
	if ((sqrt(*distanceFromPlanetPow2) <= range || range < 0))
	{
		if (blackHole)
			return Vector2(-gravityAcceleration * objectWeight * weight * (objectPosition.x - centre.x) / *distanceFromPlanetPow2, -gravityAcceleration * objectWeight * weight * (objectPosition.y - centre.y) / *distanceFromPlanetPow2);
		else
		{
			if (sqrt(*distanceFromPlanetPow2) > radius)
			{
				if (repel)
					return Vector2(gravityAcceleration * objectWeight * weight * (objectPosition.x - centre.x) / *distanceFromPlanetPow2, gravityAcceleration * objectWeight * weight * (objectPosition.y - centre.y) / *distanceFromPlanetPow2);
				else
					return Vector2(-gravityAcceleration * objectWeight * weight * (objectPosition.x - centre.x) / *distanceFromPlanetPow2, -gravityAcceleration * objectWeight * weight * (objectPosition.y - centre.y) / *distanceFromPlanetPow2);
			}
		}
	}
	return Vector2(0, 0);
}

bool Planet::calculateCollision(Vector2 objectDimension, double distanceFromPlanetPow2)
{
	if (sqrt(distanceFromPlanetPow2) <= radius + (objectDimension.y  * 1.15))
		return true;
	return false;
}

bool Planet::calculateInRange(double distanceFromPlanetPow2)
{
	if (sqrt(distanceFromPlanetPow2) <= range || range < 0)
		return true;
	return false;
}

void Planet::render()
{
	planetSprite->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
}

void Planet::setCentre(Vector2 newCentre)
{
	centre = newCentre;
}
void Planet::setWeight(double newWeight)
{
	weight = newWeight;
}
void Planet::setRange(double newRange)
{
	range = newRange;
}
void Planet::setRadius(double newRadius)
{
	radius = newRadius;
}
void Planet::setRepel(bool newRepel)
{
	repel = newRepel;
	while (spriteNumber == 0)
		spriteNumber = (rand() % 15) + 1;

	if (repel)
	{
		planetSprite = new hgeSprite(GameShell::hge->Texture_Load("Contents/PlanetRepel.png"), 0, 0, 385, 385);
		planetSprite->SetHotSpot(385 / 2.0, 385 / 2.0);
	}
}
void Planet::setBlackHole(bool newBlackHole)
{
	blackHole = newBlackHole;
	if (newBlackHole)
		repel = false;

	if (blackHole)
	{
		planetSprite = new hgeSprite(GameShell::hge->Texture_Load("Contents/BlackHole.png"), 0, 0, 385, 385);
		planetSprite->SetHotSpot(385 / 2.0, 385 / 2.0);
	}
}

Vector2 Planet::getCentre()
{
	return centre;
}
double Planet::getWeight()
{
	return weight;
}
double Planet::getRadius()
{
	return radius;
}
bool Planet::getRepel()
{
	return repel;
}
bool Planet::getBlackHole()
{
	return blackHole;
}

bool Planet::isRepellable()
{
	if ((!blackHole && spriteNumber != 9) || repel)
		return true;
	return false;
}

void Planet::loadSprites()
{
	if (spriteNumber == 9)
	{
		planetSprite = new hgeSprite(GameShell::hge->Texture_Load("Contents/PlanetStable.png"), 0, 0, 425, 425);
		planetSprite->SetHotSpot(425 / 2.0, 425 / 2.0);
	}
	else
	{
		char spriteNumberBuffer[32];
		sprintf(spriteNumberBuffer, "Contents/Planet%d.png", spriteNumber);
		planetSprite = new hgeSprite(GameShell::hge->Texture_Load(spriteNumberBuffer), 0, 0, 385, 385);
		planetSprite->SetHotSpot(385 / 2.0, 385 / 2.0);
	}
}