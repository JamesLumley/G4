#include "Enemies.h"

Enemy::Enemy()
{
	dimension = Vector2(18, 21);
	jumpPower = 4;
	jumpPowerBK = jumpPower;
	weight = 1;
	instantVelocity = Vector2(0, 0);
	movingVelocity = Vector2(1.6f, 1.6f);
	currentGravity = Vector2(0, 0);
	currentAngle = 0;
	nearestPlanetIndex = 0;
	dead = false;
	deadSoundPlayed = false;
	deathDelay = 0;
	isJumpable = false;
	isJumping = false;
	isFalling = false;
	jumpDelay = 0;


	if (rand() % 2 == 1)
		flipX = true;
	else
		flipX = false;
	jump = Vector2(0, 0);
	currentGravityAngle = 0;
	isCollidingWithLimits = false;
	isCaughtByPlanet = false;
	isCollidingWithPlatform = false;

	spriteNumber = (rand() % 10) + 1;
	if (hasToLoadFiles)
	{
		HTEXTURE t;
		t = GameShell::hge->Texture_Load("Contents/Original.png");
		sprite1 = new hgeAnimation(t, 5, 0.72, 0, 0, 32, 32);
		sprite1->SetFlip(true, false);
		sprite2 = new hgeAnimation(t, 5, 2, 32 * 5, 0, 32, 32);
		sprite2->SetFlip(true, false);
		sprite3 = new hgeAnimation(t, 5, 1.3, 64 * 5, 0, 32, 32);
		sprite3->SetFlip(true, false);
		sprite4 = new hgeAnimation(t, 5, 1, 0, 32, 32, 32);
		sprite4->SetFlip(true, false);
		sprite5 = new hgeAnimation(t, 5, 0.8, 32 * 5, 32, 32, 32);
		sprite5->SetFlip(true, false);
		sprite6 = new hgeAnimation(t, 5, 1.3, 64 * 5, 32, 32, 32);
		sprite6->SetFlip(true, false);
		sprite7 = new hgeAnimation(t, 5, 2, 0, 64, 32, 32);
		sprite7->SetFlip(true, false);
		sprite8 = new hgeAnimation(t, 5, 1, 0, 96, 32, 32);
		sprite8->SetFlip(true, false);
		sprite9 = new hgeAnimation(t, 5, 2, 0, 128, 32, 32);
		sprite9->SetFlip(true, false);
		sprite10 = new hgeAnimation(t, 5, 2, 0, 160, 32, 32);
		sprite10->SetFlip(true, false);
	
		deathSound = GameShell::hge->Effect_Load("Contents/EnemyDeath.mp3");

		hasToLoadFiles = false;
	}
	double randomV = ((rand() % 100) - 50) / 100.0;
	movingVelocity.x = movingVelocity.x + movingVelocity.x*randomV;
	movingVelocity.y = movingVelocity.y + movingVelocity.y*randomV;

	//Circle collision detection
	headRadius = 8;
	if (spriteNumber == 3)
	{
		headRadius = 9;
		isJumpable = true;
	}
	else if (spriteNumber == 4)
	{
		headRadius = 10;
		isJumpable = true;
	}
	else if (spriteNumber == 6)
		headRadius = 9;
	else if (spriteNumber == 8)
	{
		headRadius = 13;
		isJumpable = true;
	}
	else if (spriteNumber == 9)
		isJumpable = true;
	else if (spriteNumber == 10)
		headRadius = 9;
	centerHead = Vector2(position.x + 0.7 * dimension.y * sin(currentAngle), position.y + 0.7 * dimension.y * cos(currentAngle));
}

void Enemy::init(int currentPlanetsNumber, int currentPlatformsNumber, int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	generateNewReandomPosition(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
	distanceFromPlanetPow2 = vector<double>(currentPlanetsNumber);
	angleWithPlanet = vector<double>(currentPlanetsNumber);
	distanceFromPlatformPow2 = vector<double>(currentPlatformsNumber);
	angleWithPlatform = vector<double>(currentPlatformsNumber);
}

void Enemy::update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, double LevelSizeXNegative, double LevelSizeXPositive, double LevelSizeYNegative, double LevelSizeYPositive)
{
	if (!dead)
	{
		//Reset
		instantVelocity = Vector2(0, 0);
		jumpDelay++;

		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			(*planets)[i].calculateDistance(position, &distanceFromPlanetPow2[i], dimension);
			(*planets)[i].calculateAngle(position, &angleWithPlanet[i]);
			instantVelocity += (*planets)[i].calculateGravity(position, weight, &distanceFromPlanetPow2[i], gravityAcceleration);
		}
		currentGravityAngle = atan2((-instantVelocity.x), (-instantVelocity.y));
		currentAngle = currentGravityAngle;
		if (!isJumping && !isCaughtByPlanet)
			position += instantVelocity;

		if (instantVelocity.x == 0 || instantVelocity.y == 0)
			generateNewReandomPosition(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
		if (position.x > LevelSizeXPositive || position.x < LevelSizeXNegative || position.y > LevelSizeXPositive || position.y < LevelSizeXNegative)
			generateNewReandomPosition(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);

		//Collisions With Planets and Platforms
		for (int n = 0; n < currentPlanetsNumber; n++)
		{
			if ((*planets)[n].calculateCollision(dimension, distanceFromPlanetPow2[n]))
			{
				if ((*planets)[n].getBlackHole()) //If BlackHole
				{
					generateNewReandomPosition(LevelSizeXNegative, LevelSizeXPositive, LevelSizeYNegative, LevelSizeYPositive);
					break;
				}
				else if ((*planets)[n].getRepel()) //If Repellent
				{
					setInvertingGravity();
				}
				else //If Planet
				{
					isCaughtByPlanet = true;
					currentAngle = angleWithPlanet[n];
					if (!isJumping || isFalling)
						position = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + (dimension.y * 1)) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + (dimension.y * 1)) * cos(currentAngle)));
					nearestPlanetIndex = n;
					isFalling = false;
					break;
				}
			}
			isCaughtByPlanet = false;
		}

		GravityManager::normalizeAngle(&currentAngle);

		if (isJumping)
		{
			if (jumpPower < 0 && !isFalling)
				isJumping = false;
			isCaughtByPlanet = true;
			currentAngle = angleWithPlanet[nearestPlanetIndex];
			jump = Vector2(jumpPower * sin(currentAngle), jumpPower * cos(currentAngle));
			position += jump;
			jumpPower -= 0.17;
			if (jumpPower < 0)
				isFalling = true;
		}
		if (isCaughtByPlanet)
		{
			currentAngle = angleWithPlanet[nearestPlanetIndex];
			if (flipX)
			{
				position.x -= movingVelocity.x*(cos(currentAngle));
				position.y += movingVelocity.y*(sin(currentAngle));
			}
			else
			{
				position.x += movingVelocity.x*(cos(currentAngle));
				position.y -= movingVelocity.y*(sin(currentAngle));
			}
			if (isJumpable && jumpDelay > 100 && !isJumping)// && sprite1->GetFrame() == 4)
			{
				isFalling = false;
				isJumping = true;
				jumpDelay = 0;
				jumpPower = jumpPowerBK;
			}
		}

		//Update CenterHead
		centerHead = Vector2(position.x + dimension.y * sin(currentAngle) * 0.7, position.y + dimension.y * cos(currentAngle) * 0.7);
	}
	if (dead)
	{
		if (!deadSoundPlayed && deathDelay == 0)
		{
			GameShell::hge->Effect_PlayEx(deathSound, 52, 0, 1.0f, false);
			deadSoundPlayed = true;
		}
		deathDelay--;
		if (deathDelay == 0)
		{

		}
	}
}

void Enemy::render()
{
	if (!dead)
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
		{
			sprite1->SetFlip(flipX, false);
			sprite1->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite1->IsPlaying())
				sprite1->Play();
			sprite1->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 2)
		{
			sprite2->SetFlip(flipX, false);
			sprite2->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite2->IsPlaying())
				sprite2->Play();
			sprite2->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 3)
		{
			sprite3->SetFlip(flipX, false);
			sprite3->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite3->IsPlaying())
				sprite3->Play();
			sprite3->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 4)
		{
			sprite4->SetFlip(flipX, false);
			sprite4->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite4->IsPlaying())
				sprite4->Play();
			sprite4->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 5)
		{
			sprite5->SetFlip(flipX, false);
			sprite5->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite5->IsPlaying())
				sprite5->Play();
			sprite5->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 6)
		{
			sprite6->SetFlip(flipX, false);
			sprite6->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite6->IsPlaying())
				sprite6->Play();
			sprite6->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 7)
		{
			sprite7->SetFlip(flipX, false);
			sprite7->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite7->IsPlaying())
				sprite7->Play();
			sprite7->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 8)
		{
			sprite8->SetFlip(flipX, false);
			sprite8->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite8->IsPlaying())
				sprite8->Play();
			sprite8->Update(GameShell::hge->Timer_GetDelta());
		}
		else if (spriteNumber == 9)
		{
			sprite9->SetFlip(flipX, false);
			sprite9->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite9->IsPlaying())
				sprite9->Play();
			sprite9->Update(GameShell::hge->Timer_GetDelta());
		}
		else //if (spriteNumber == 10)
		{
			sprite10->SetFlip(flipX, false);
			sprite10->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
			if (!sprite10->IsPlaying())
				sprite10->Play();
			sprite10->Update(GameShell::hge->Timer_GetDelta());
		}
	}
}

Vector2 Enemy::getCenterHead()
{
	return centerHead;
}

int Enemy::getCollidingPlanetIndex()
{
	return nearestPlanetIndex;
}

float Enemy::getHeadRadius()
{
	return headRadius;
}

void Enemy::setDead(bool death)
{
	dead = death;
}

bool Enemy::isDead()
{
	return dead;
}

void Enemy::setInvertingGravity()
{
	isCaughtByPlanet = false;
	isJumping = false;
}

void Enemy::generateNewReandomPosition(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	position = Vector2(((LevelSizeXPositive - LevelSizeXNegative) * 0.94) * ((rand() % 202 - 101) / 200.0), ((LevelSizeYPositive - LevelSizeYNegative) * 0.94) *  ((rand() % 202 - 101) / 200.0));
}