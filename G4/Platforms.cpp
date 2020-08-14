#include "Platforms.h"

Platform::Platform()
{
	weight = 14;
}

Platform::Platform(Vector2 newCenter, double newWeight, double newRange, Vector2 newDimension, Vector2 newDirection)
{
	centre = newCenter;
	weight = newWeight;
	range = newRange; //Should be proportional to force, if negative then infinite
	dimension = newDimension;
	direction = newDirection;
	collidingWithTop = false;
	collidingWithBottom = false;
	collidingWithRight = false;
	collidingWithLeft = false;
}

//void Platform::CalculateDistance(Vector2 PlayerPosition, Vector2 PlayerDimension)
//{
//	CurrentDistance.x = PlayerPosition.x - Centre.x;
//	CurrentDistance.y = PlayerPosition.y - Centre.y;
//	if (CurrentDistance.x < Dimension.x)
//		CurrentDistance.x = Dimension.x + PlayerDimension.x;
//	if (CurrentDistance.y < Dimension.y)
//		CurrentDistance.y = Dimension.y + PlayerDimension.y;
//	if (CurrentDistance.x > -Dimension.x)
//		CurrentDistance.x = -Dimension.x - PlayerDimension.x;
//	if (CurrentDistance.y > -Dimension.y)
//		CurrentDistance.y = -Dimension.y - PlayerDimension.y;
//}

void Platform::calculateAngle(Vector2 playerPosition, double *angleWithPlatform)
{
	*angleWithPlatform = atan2(playerPosition.x - centre.x, playerPosition.y - centre.y);
	GravityManager::normalizeAngle(&*angleWithPlatform);
}

Vector2 Platform::calculateGravity(Vector2 PlayerPosition, double ObjectWeight)
{
	weight = 100;
	double distanceFromPlanetPow2 = pow(PlayerPosition.x - centre.x, 2) + pow(PlayerPosition.y - centre.y, 2);
	if (sqrt(distanceFromPlanetPow2) <= range)
	{
		return Vector2(-ObjectWeight * weight * (PlayerPosition.x - centre.x) / distanceFromPlanetPow2, -ObjectWeight * weight * (PlayerPosition.y - centre.y) / distanceFromPlanetPow2);
	}
	//else
	return Vector2(0, 0);
}

bool Platform::calculateCollision(Vector2 playerPosition, Vector2 playerDimension)
{
	collidingWithLeft = false;
	collidingWithRight = false;
	collidingWithTop = false;
	collidingWithBottom = false;
	//if (PlayerPosition.x + PlayerDimension.x >= Centre.x - Dimension.x && (PlayerPosition.y + PlayerDimension.y >= Centre.y - Dimension.y && PlayerPosition.y - PlayerDimension.y <= Centre.y + Dimension.y))
	//{
	//	//Avoid overlapping
	//	//PlayerPosition.x = -Dimension.x + CurrentDistance.x;
	//	CollidingWithLeft = true;
	//	return true;
	//}
	//if (PlayerPosition.x - PlayerDimension.x <= Centre.x + Dimension.x && (PlayerPosition.y + PlayerDimension.y >= Centre.y - Dimension.y && PlayerPosition.y - PlayerDimension.y <= Centre.y + Dimension.y))
	//{
	//	//Avoid overlapping
	//	//PlayerPosition.x = -Dimension.x + CurrentDistance.x;
	//	CollidingWithRight = true;
	//	return true;
	//}
	//if (PlayerPosition.y + PlayerDimension.y >= Centre.y - Dimension.y && (PlayerPosition.x + PlayerDimension.x >= Centre.x - Dimension.x && PlayerPosition.x - PlayerDimension.x <= Centre.x + Dimension.x))
	//{
	//	//Avoid overlapping
	//	//PlayerPosition.x = -Dimension.x + CurrentDistance.x;
	//	CollidingWithTop = true;
	//	return true
	//}
	if (playerPosition.y < centre.y && abs((int)(playerPosition.y - centre.y)) - playerDimension.y < (dimension.y / 2) && playerPosition.x + playerDimension.x >= centre.x - (dimension.x / 2) && playerPosition.x - playerDimension.x <= centre.x + (dimension.x / 2))
	{
		//Avoid overlapping
		//PlayerPosition.x = -Dimension.x + CurrentDistance.x;
		collidingWithBottom = true;
		return true;
	}
	return false;
}