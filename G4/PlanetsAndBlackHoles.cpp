#include "PlanetsAndBlackHoles.h"

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
		spriteNumber = (rand() % 9) + 1;
	if (hasToLoadSprites)
	{
		loadSprites();
		hasToLoadSprites = false;
	}
	rotation = ((rand() % 200) / 200.0) * M_PI;
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
	if (blackHole)
		blackHoleSprite->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
	else if (repel)
		planetSpriteRepel->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
	else
	{
		if (spriteNumber == 1)
			planetSprite1->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 2)
			planetSprite2->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 3)
			planetSprite3->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 4)
			planetSprite4->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 5)
			planetSprite5->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 6)
			planetSprite6->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 7)
			planetSprite7->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else if (spriteNumber == 8)
			planetSprite8->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
		else //if (spriteNumber == 9)
			planetSprite9->RenderEx(centre.x, centre.y, rotation, radius / 188, radius / 188);
	}
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
}
void Planet::setBlackHole(bool newBlackHole)
{
	blackHole = newBlackHole;
	if (newBlackHole)
		repel = false;
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
	blackHoleSprite = new hgeSprite(GameShell::hge->Texture_Load("Contents/BlackHole.png"), 0, 0, 385, 385);
	planetSprite1 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet1.png"), 0, 0, 385, 385);
	planetSprite2 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet2.png"), 0, 0, 385, 385);
	planetSprite3 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet3.png"), 0, 0, 385, 385);
	planetSprite4 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet4.png"), 0, 0, 385, 385);
	planetSprite5 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet5.png"), 0, 0, 385, 385);
	planetSprite6 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet6.png"), 0, 0, 385, 385);
	planetSprite7 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet7.png"), 0, 0, 385, 385);
	planetSprite8 = new hgeSprite(GameShell::hge->Texture_Load("Contents/Planet8.png"), 0, 0, 385, 385);
	planetSprite9 = new hgeSprite(GameShell::hge->Texture_Load("Contents/PlanetStable.png"), 0, 0, 425, 425);
	planetSpriteRepel = new hgeSprite(GameShell::hge->Texture_Load("Contents/PlanetRepel.png"), 0, 0, 385, 385);
	blackHoleSprite->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite1->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite2->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite3->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite4->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite5->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite6->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite7->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite8->SetHotSpot(385 / 2.0, 385 / 2.0);
	planetSprite9->SetHotSpot(425 / 2.0, 425 / 2.0);
	planetSpriteRepel->SetHotSpot(385 / 2.0, 385 / 2.0);
}