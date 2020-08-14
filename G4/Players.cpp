#include "Players.h"
#include "GameState_Menu.h"

Player::~Player()
{
	delete controller;
	delete playerRun;
	delete playerJump;
	delete playerLand;
	delete playerRoll;
}

void Player::init(int currentPlanetsNumber, int currentPlatformsNumber, float positionX, float positionY, int newPlayersNumber, int newPlayerNumber, int newlives)
{
	dimension = Vector2(18, 21);
	dimensionBK = dimension;
	jumpPower = 16;
	jumpPowerBK = jumpPower;
	weight = 1;
	walkingVelocity = Vector2(6.85, 6.85);
	runningVelocity = walkingVelocity * 1.95;
	CaptureRadius = dimension.y * 8;
	position = Vector2(positionX, positionY);
	instantVelocity = Vector2(0, 0);
	currentMovingVelocity = Vector2(0, 0);
	currentMovingVelocityBK = Vector2(0, 0);
	currentGravity = Vector2(0, 0);
	currentAngle = 0;
	//bIsJetPackActive = false;
	nearestPlanetIndex = 0;
	score = 0;
	jumpDelay = 0;
	vibrationDelay = 0;
	collectedStars = 0;
	dead = false;
	lives = newlives;
	deathDelay = 0;
	StandardizeJump = 0;
	FlyingTime = 0;
	isJumping = false;
	isFalling = true;
	inertialRight = false;
	inertialLeft = false;
	inertia = false;
	rightPressed = false;
	leftPressed = false;
	jumpPressed = false;
	smashPressed = false;
	runPressed = false;
	isSmashing = false;
	resetAngle = false;
	interactPressed = false;
	playersNumber = newPlayersNumber;
	playerNumber = newPlayerNumber;
	controller = new XboxController(playerNumber);

	jump = Vector2(0, 0);
	currentGravityAngle = 0;
	isCollidingWithLimits = false;
	isCaughtByPlanet = false;
	isCollidingWithPlatform = false;
	flipX = false;
	flipY = false;
	isDebugMode = false;
	
	HTEXTURE t;
	t = GameShell::hge->Texture_Load("Contents/MarioSprite.png");
	playerRun = new hgeAnimation(t, 25, 40, 0, 0, 30, 35);
	playerRun->SetFlip(true, false);
	playerJump = new hgeAnimation(t, 20, 23, 0, 35, 30, 35);
	playerJump->SetFlip(true, false);
	playerLand = new hgeAnimation(t, 6, 16, 0, 70, 30, 35);
	playerLand->SetFlip(true, false);
	playerRoll = new hgeAnimation(t, 10, 20, 0, 105, 30, 35);
	playerRoll->SetFlip(true, false);
	playerActiveAn = playerLand;
	playerActiveAn->SetFlip(true, false);

	resetTimer = 0;
	footStep = GameShell::hge->Effect_Load("Contents/Step.wav");
	starTakenEffect = GameShell::hge->Effect_Load("Contents/Star.wav");
	deathEffect = GameShell::hge->Effect_Load("Contents/Death.wav");
	SmashEffect = GameShell::hge->Effect_Load("Contents/Smash.wav");
	stepDelay = 0;

	distanceFromPlanetPow2 = vector<double>(currentPlanetsNumber);
	angleWithPlanet = vector<double>(currentPlanetsNumber);
	distanceFromPlatformPow2 = vector<double>(currentPlatformsNumber);
	angleWithPlatform = vector<double>(currentPlatformsNumber);

	if (playerNumber == 2)
		setAsSecondPlayer();

	//Circle collision detection
	headRadius = 7.0f;
	feetRadius = 8.0f;
	centerHead = Vector2(position.x + 0.7 * dimension.y * sin(currentAngle), position.y + 0.7 * dimension.y * cos(currentAngle));
	centerFeet = Vector2(position.x - 0.1 * dimension.y * sin(currentAngle), position.y - 0.1 * dimension.y * cos(currentAngle));
}

void Player::update(int currentPlanetsNumber, int currentPlatformsNumber, vector<Planet>* planets, vector<Platform>* platforms, double gravityAcceleration, int currentEnemiesNumber, vector<Enemy>* enemies, int currentStarsChestsNumber, vector<StarChest>* StarChests, double* LevelSizeXNegative, double* LevelSizeXPositive, double* LevelSizeYNegative, double* LevelSizeYPositive)
{
	if (lives > 0)
	{
		//Reset
		instantVelocity = Vector2(0, 0);
		currentGravity = Vector2(0, 0);
		rightPressed = false;
		leftPressed = false;
		jumpPressed = false;
		smashPressed = false;
		runPressed = false;
		resetPosition = false;
		ResetMovement = false;
		interactPressed = false;
		if (vibrationDelay > 0)
			vibrationDelay--;
		if (vibrationDelay == 0)
			controller->vibrate(0, 0);
		if (resetTimer > 0)
			resetTimer--;
		if (stepDelay > 0)
			stepDelay--;
		if (jumpDelay > 0)
			jumpDelay--;

		//Controls
		if (playerNumber == 1 && deathDelay == 0) //First Player (It changes if there are one or two players)
		{
			if (GameShell::GetKeyState(HGEK_RIGHT) || controller->getState().Gamepad.sThumbLX > 10000)
				rightPressed = true;
			if (GameShell::GetKeyState(HGEK_LEFT) || controller->getState().Gamepad.sThumbLX < -10000)
				leftPressed = true;
			if (GameShell::GetKeyState(HGEK_UP) || GameShell::GetKeyState(HGEK_SPACE) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
				jumpPressed = true;
			if (playersNumber == 1)
			{
				if (GameShell::GetKeyState(HGEK_DOWN) || GameShell::GetKeyState(HGEK_ENTER) || GameShell::GetKeyState(HGEK_Q) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
					smashPressed = true;
				if (GameShell::GetKeyState(HGEK_E) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
					interactPressed = true;
				if (GameShell::GetKeyState(HGEK_LSHIFT) || GameShell::GetKeyState(HGEK_LCONTROL) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
					runPressed = true;
			}
			else //if (PlayersNumber == 2)
			{
				if (GameShell::GetKeyState(HGEK_DOWN) || GameShell::GetKeyState(HGEK_ENTER) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
					smashPressed = true;
				if (GameShell::GetKeyState(HGEK_RSHIFT) || GameShell::GetKeyState(HGEK_RCONTROL) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
					runPressed = true;
			}
		}
		else if  (deathDelay == 0)//Second Player
		{
			if (GameShell::GetKeyState(HGEK_D) || controller->getState().Gamepad.sThumbLX > 10000)
				rightPressed = true;
			if (GameShell::GetKeyState(HGEK_A) || controller->getState().Gamepad.sThumbLX < -10000)
				leftPressed = true;
			if (GameShell::GetKeyState(HGEK_W) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
				jumpPressed = true;
			if (GameShell::GetKeyState(HGEK_S) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
				smashPressed = true;
			if (GameShell::GetKeyState(HGEK_LSHIFT) || GameShell::GetKeyState(HGEK_LCONTROL) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				runPressed = true;
		}

		//Calculate CurrentGravity for Player (Sum of effects method abandoned? one ruling gravity at the time method abandoned?)
		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			(*planets)[i].calculateDistance(position, &distanceFromPlanetPow2[i], dimension);
			(*planets)[i].calculateAngle(position, &angleWithPlanet[i]);
			currentGravity += (*planets)[i].calculateGravity(position, weight, &distanceFromPlanetPow2[i], gravityAcceleration);
		}

		currentGravityAngle = atan2((-currentGravity.x), (-currentGravity.y));
		currentAngle = currentGravityAngle;

		//Collisions With Planets and Platforms
		for (int n = 0; n < currentPlanetsNumber; n++)
		{
			if ((*planets)[n].calculateCollision(dimension, distanceFromPlanetPow2[n]))
			{
				if ((*planets)[n].getBlackHole()) //If BlackHole
				{
					resetAngle = false;
					isSmashing = false;
					inertia = false;
					if (deathDelay == 0)
					{
						currentAngleBK = currentAngle;
						if (FlyingTime != 1)
						{
							deathDelay = 155;
							if (vibrationDelay == 0)
							{
								controller->vibrate(30000, 30000);
								vibrationDelay = 30;
							}
						}
					}
					FlyingTime = 0;
					position = (*planets)[n].getCentre();
					break;
				}
				else if ((*planets)[n].getRepel()) //If Repel
				{
					resetAngle = false;
					nearestPlanetIndex = n;
					currentAngle = angleWithPlanet[n];
					setInvertingGravity();
					position = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + (dimension.y * 1)) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + (dimension.y * 1)) * cos(currentAngle)));
					break;
				}
				else
				{
					//Now we can choose if let the Player detatch from a planet only by jumping, or just when forces are high enough to catch him.
					//So we can stop the gravity effect on him, or leave it, stop it's velocity and set his weight.
					FlyingTime = 0;
					if (n != nearestPlanetIndex || isJumping)
					{
						if (inertia)
							jumpDelay = 8;
						else
							jumpDelay = 1;
						GameShell::hge->Effect_PlayEx(SmashEffect, 39, 0, 1, false);
						playerActiveAn = playerLand;
						playerActiveAn->SetFrame(0);
						ResetMovement = true;
					}
					isCaughtByPlanet = true;
					resetAngle = false;
					isJumping = false;
					isFalling = false;
					inertialRight = false;
					inertialLeft = false;
					inertia = false;
					isSmashing = false;
					jumpPower = jumpPowerBK;
					currentAngle = angleWithPlanet[n];
					position = Vector2((*planets)[n].getCentre().x + (((*planets)[n].getRadius() + dimension.y) * sin(currentAngle)), (*planets)[n].getCentre().y + (((*planets)[n].getRadius() + dimension.y) * cos(currentAngle)));
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
				isCaughtByPlanet = true;
				resetAngle = false;
				FlyingTime = 0;
				isCollidingWithPlatform = true;
				isJumping = false;
				isFalling = false;
				inertialRight = false;
				inertialLeft = false;
				inertia = false;
				jumpPower = jumpPowerBK;
				currentAngle = M_PI;
				angleWithPlanet[nearestPlanetIndex] = M_PI;
				isSmashing = false;
				break;
			}
			isCollidingWithPlatform = false;
		}

		//Move Player
		if (runPressed)
			currentMovingVelocity = runningVelocity;
		else
			currentMovingVelocity = walkingVelocity;
		//Jump
		if (jumpPressed && !isFalling && !isJumping && jumpDelay == 0)
		{
			resetAngle = false;
			isJumping = true;
			if (runPressed && (rightPressed || leftPressed))
			{
				StandardizeJump = sqrt(pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).y, 2) + pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).x, 2));
				StandardizeJump /= 6;
				jumpPower = jumpPowerBK * 0.98;//*  18 / sqrt(pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).y, 2) + pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).x, 2));
				currentAngleBK = currentAngle;
				currentMovingVelocityBK = runningVelocity * 1.226;
				inertia = true;
				if (rightPressed)
					inertialRight = true;
				else
					inertialRight = false;
				if (!rightPressed && leftPressed)
					inertialLeft = true;
				if (!leftPressed)
					inertialLeft = false;
			}
			else
			{
				jumpPower = jumpPowerBK;
				inertia = false;
				inertialRight = false;
				inertialLeft = false;
			}
		}
		if (isJumping)
		{
			if (inertia)
			{
				//Inertia force
				if (inertialRight) //If you press right you move anticlockwise
				{
					instantVelocity.x -= currentMovingVelocityBK.x*(cos(currentAngleBK));
					instantVelocity.y += currentMovingVelocityBK.y*(sin(currentAngleBK));
				}
				else if (inertialLeft) //If you press left you move anticlockwise
				{
					instantVelocity.x += currentMovingVelocityBK.x*(cos(currentAngleBK));
					instantVelocity.y -= currentMovingVelocityBK.y*(sin(currentAngleBK));
				}
				//else
					//currentAngleBK = currentAngle;
				currentMovingVelocityBK /= 1.0035;

				if (!isFalling)
				{
					if (jumpPower > 0)
					{
						jump = Vector2(jumpPower * sin(currentAngleBK) * StandardizeJump, jumpPower * cos(currentAngleBK) * StandardizeJump);
						instantVelocity += jump;
					}
					jumpPower -= 1.20;
					if (sqrt(distanceFromPlanetPow2[nearestPlanetIndex]) >(*planets)[nearestPlanetIndex].getRadius() + CaptureRadius)
						isFalling = true;
				}
			}
			else
			{
				//if (!isFalling)
				currentAngle = angleWithPlanet[nearestPlanetIndex];

				//isFalling = false;
				jump = Vector2(jumpPower * sin(angleWithPlanet[nearestPlanetIndex]) * (*planets)[nearestPlanetIndex].getWeight() / (*planets)[nearestPlanetIndex].getRadius(), jumpPower * cos(angleWithPlanet[nearestPlanetIndex]) * (*planets)[nearestPlanetIndex].getWeight() / (*planets)[nearestPlanetIndex].getRadius());
				instantVelocity += jump;
				jumpPower -= 1.3;
			}
		}
		if (isCaughtByPlanet || (isJumping && !inertia))
		{
			if ((isCaughtByPlanet || isJumping) && !inertia)
			{
				if (rightPressed) //If you press right you move anticlockwise
				{
					instantVelocity.x -= currentMovingVelocity.x*(cos(angleWithPlanet[nearestPlanetIndex]));
					instantVelocity.y += currentMovingVelocity.y*(sin(angleWithPlanet[nearestPlanetIndex]));
				}
				else if (leftPressed) //If you press left you move anticlockwise
				{
					instantVelocity.x += currentMovingVelocity.x*(cos(angleWithPlanet[nearestPlanetIndex]));
					instantVelocity.y -= currentMovingVelocity.y*(sin(angleWithPlanet[nearestPlanetIndex]));
				}
			}
			if (isJumping && !inertia)
			{
				for (int i = 0; i < currentPlanetsNumber; i++)
				{
					if (i != nearestPlanetIndex && !(*planets)[i].getBlackHole() && !(*planets)[i].getRepel())
					{
						Vector2 fakeTempPosition = Vector2((*planets)[nearestPlanetIndex].getCentre().x + (((*planets)[nearestPlanetIndex].getRadius() + dimension.y) * sin(currentAngle)), (*planets)[nearestPlanetIndex].getCentre().y + (((*planets)[nearestPlanetIndex].getRadius() + dimension.y) * cos(currentAngle)));
						double fakeTempDistance;
						(*planets)[i].calculateDistance(fakeTempPosition, &fakeTempDistance, dimensionBK);
						if (sqrt(fakeTempDistance) < 300)
							resetAngle = true;
						if ((sqrt(distanceFromPlanetPow2[nearestPlanetIndex]) - (*planets)[nearestPlanetIndex].getRadius()) > (sqrt(distanceFromPlanetPow2[i]) - (*planets)[i].getRadius()))
						{
							if (sqrt(pow((*planets)[i].calculateGravity(position, weight, &distanceFromPlanetPow2[i], gravityAcceleration).y, 2) + pow((*planets)[i].calculateGravity(position, weight, &distanceFromPlanetPow2[i], gravityAcceleration).x, 2)) > sqrt(pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).y, 2) + pow((*planets)[nearestPlanetIndex].calculateGravity(position, weight, &distanceFromPlanetPow2[nearestPlanetIndex], gravityAcceleration).x, 2)));
							{
								resetAngle = true;
								isFalling = true;
							}
						}
					}
				}
			}
		}
		//Add current gravity to current velocity
		if ((isJumping && inertia) || isFalling)
		{
			instantVelocity += currentGravity;
		}
		//Smashing over the nearest planet
		if (smashPressed && isJumping && inertia)
		{
			int NearestI = -1;
			position += instantVelocity;
			for (int i = 0; i < currentPlanetsNumber - 1; i++)
			{
				(*planets)[i].calculateDistance(position, &distanceFromPlanetPow2[i], dimensionBK);
			}
			position -= instantVelocity;
			if (currentPlanetsNumber >= 2)
			{
				for (int i = 0; i < currentPlanetsNumber - 1; i++)
				{
					if (i == 0)
						NearestI = 0;
					else if(distanceFromPlanetPow2[i] < distanceFromPlanetPow2[NearestI] && !(*planets)[i].getBlackHole() && !(*planets)[i].getRepel())
						NearestI = i;
				}
			}
			if (NearestI != -1 && !(*planets)[NearestI].getBlackHole() && !(*planets)[NearestI].getRepel())
			{
				weight = 3;
				isSmashing = true;
				nearestPlanetIndex = NearestI;
				currentAngle = angleWithPlanet[NearestI];
				isJumping = false;
				isFalling = true;
			}
		}
		if (!isSmashing)
			weight = 1;

		//Collisions with Level Limits
		isCollidingWithLimits = false;

		if ((position.x + instantVelocity.x) < *LevelSizeXNegative + dimension.x)
		{
			position.x = *LevelSizeXNegative;
			isCollidingWithLimits = true;
		}
		else if ((position.x + instantVelocity.x) > *LevelSizeXPositive - dimension.x)
		{
			position.x = *LevelSizeXPositive;
			isCollidingWithLimits = true;
		}
		if ((position.y + instantVelocity.y) < *LevelSizeYNegative + dimension.y)
		{
			position.y = *LevelSizeYNegative;
			isCollidingWithLimits = true;
		}
		else if ((position.y + instantVelocity.y) > *LevelSizeYPositive - dimension.y)
		{
			position.y = *LevelSizeYPositive;
			isCollidingWithLimits = true;
		}
		if (!isCollidingWithLimits && deathDelay == 0)
		{
			position += instantVelocity;
			if (ResetMovement)
			{
				currentAngle = angleWithPlanet[nearestPlanetIndex];
				position = Vector2((*planets)[nearestPlanetIndex].getCentre().x + (((*planets)[nearestPlanetIndex].getRadius() + dimension.y) * sin(currentAngle)), (*planets)[nearestPlanetIndex].getCentre().y + (((*planets)[nearestPlanetIndex].getRadius() + dimension.y) * cos(currentAngle)));
			}
		}
		if (resetAngle)
			currentAngle = currentGravityAngle;

		//If no planets are in range or is lost in space anyway, Reset Position
		if (!isCaughtByPlanet)
			FlyingTime++;
		else
			FlyingTime = 0;
		if (FlyingTime > 450)
		{
			deathDelay = 50;
			vibrationDelay = 23;
			controller->vibrate(30000, 30000);
			FlyingTime = 0;
		}
		bool inRange = false;
		for (int i = 0; i < currentPlanetsNumber; i++)
		{
			if ((*planets)[i].calculateInRange(distanceFromPlanetPow2[i]))
			{
				inRange = true;
				break;
			}
		}
		if (!inRange)
		{
			deathDelay == 50;
			vibrationDelay = 23;
			controller->vibrate(30000, 30000);
		}
		if (!resetPosition)
		{
			//Update Center Circles
			centerHead = Vector2(position.x + 0.7 * dimension.y * sin(currentAngle), position.y + 0.7 * dimension.y * cos(currentAngle));
			centerFeet = Vector2(position.x - 0.7 * dimension.y * sin(currentAngle), position.y - 0.7 * dimension.y * cos(currentAngle));
			if (resetTimer == 0)
			{
				//Check collision head-head 
				for (int i = 0; i < currentEnemiesNumber; i++)
				{
					if (checkDeathByEnemy((*enemies)[i]) && !(*enemies)[i].isDead())
					{
						deathDelay = 50;
						vibrationDelay = 23;
						controller->vibrate(30000, 30000);
						break;
					}
				}
			}
			//Check collision feet-head 
			for (int i = 0; i < currentEnemiesNumber; i++)
			{
				if (checkEnemyKilled((*enemies)[i]) && !(*enemies)[i].isDead())
				{
					FlyingTime = 0;
					jumpDelay = 0;
					isSmashing = false;
					isJumping = true;
					isFalling = false;
					inertia = false;
					jumpPower = jumpPowerBK / 1.23;
					vibrationDelay = 13;
					controller->vibrate(30000, 30000);
					(*enemies)[i].setDead(true);
					score++;
					break;
				}
			}
			//Check for collision with chests
			for (int i = 0; i < currentStarsChestsNumber; i++)
			{
				if (!(*StarChests)[i].getOpened() && !isJumping && interactPressed && abs(position.x - (*StarChests)[i].getPosition().x) < (*StarChests)[i].getDimensions().x && abs(position.y - (*StarChests)[i].getPosition().y) < (*StarChests)[i].getDimensions().y)
					(*StarChests)[i].setOpened(true);
				else if ((*StarChests)[i].getOpened() && !(*StarChests)[i].getStarTaken() && abs(position.x - (*StarChests)[i].getStarPosition().x) < (*StarChests)[i].getDimensions().x / 1.32 && abs(position.y - (*StarChests)[i].getStarPosition().y) < (*StarChests)[i].getDimensions().y / 1.32)
				{
					collectedStars++;
					GameShell::hge->Effect_PlayEx(starTakenEffect, 100, 0, 1, false);
					(*StarChests)[i].setStarTaken(true);
				}
			}
		}
		//Check for death
		if (deathDelay > 0)
		{
			if (deathDelay == 50 && lives > 1)
			{
				resetPosition = true;
				GameShell::hge->Effect_PlayEx(deathEffect, 100, 0, 1, false);
			}
			if (deathDelay == 1 && lives > 1)
			{
				lives--;
				controller->vibrate(0, 0);
			}
			else if (lives == 1 && deathDelay < 50)
			{
				lives--;
				controller->vibrate(0, 0);
				GameShell::hge->Effect_PlayEx(deathEffect, 100, 0, 1, false);
			}
			if (deathDelay > 50)
			{
				dimension /= 1.01f;
				currentAngle = currentAngleBK;
				currentAngle -= 0.17;
				currentAngleBK = currentAngle;
			}
			deathDelay--;
		}
		if (resetPosition)
		{
			FlyingTime = 0;
			position = Vector2(0, 0);
			resetTimer = 160;
			isJumping = false;
			isFalling = true;
			dimension = dimensionBK;
		}

		GravityManager::normalizeAngle(&currentAngle);

		//DEBUG
		if (GameShell::GetKeyState(HGEK_F4) == GameShell::KeyState_Pressed)
			isDebugMode = !isDebugMode;
		if (isDebugMode)
		{
			FlyingTime = 1;
			position -= instantVelocity;
			if (GameShell::GetKeyState(HGEK_RIGHT))
				position.x += runningVelocity.x*3;
			else if (GameShell::GetKeyState(HGEK_LEFT))
				position.x -= runningVelocity.x*3;
			if (GameShell::GetKeyState(HGEK_UP))
				position.y -= runningVelocity.y*3;
			else if (GameShell::GetKeyState(HGEK_DOWN))
				position.y += runningVelocity.y*3;
		}
	}
}

void Player::render()
{
	if (lives > 0)
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

		if (!(isJumping && inertia))
		{
			if (rightPressed)
				flipX = true;
			else if (leftPressed)
				flipX = false;
		}

		//Set Sprites
		if (isCaughtByPlanet)
		{
			if (rightPressed || leftPressed)
			{
				playerActiveAn = playerRun;
				playerActiveAn->SetFlip(flipX, flipY);

				if (!playerActiveAn->IsPlaying())
					playerActiveAn->Play();
				playerActiveAn->Update(GameShell::hge->Timer_GetDelta());

				if (stepDelay == 0)
				{
					GameShell::hge->Effect_PlayEx(footStep, 100, 0, 1, false);
					stepDelay = 10;
				}
			}
			else
			{
				playerActiveAn = playerLand;
				playerActiveAn->SetFlip(flipX, flipY);
				if (playerActiveAn->GetFrame() == 5)
					playerActiveAn->Stop();
				else if (!playerActiveAn->IsPlaying())
					playerActiveAn->Play();
				playerActiveAn->Update(GameShell::hge->Timer_GetDelta());
			}
		}
		else
		{
			if (isSmashing)
			{
				playerActiveAn = playerRoll;
				playerActiveAn->SetFlip(false, false);
				if (!playerActiveAn->IsPlaying())
					playerActiveAn->Play();
				playerActiveAn->Update(GameShell::hge->Timer_GetDelta());
			}
			else if (isJumping)
			{
				playerActiveAn = playerJump;
				playerActiveAn->SetFlip(flipX, flipY);
				if (!playerActiveAn->IsPlaying())
					playerActiveAn->Play();
				playerActiveAn->Update(GameShell::hge->Timer_GetDelta());
			}
		}

		if ((resetTimer >= 0 && resetTimer <= 10) || (resetTimer >= 20 && resetTimer <= 30) || (resetTimer >= 40 && resetTimer <= 50) || (resetTimer >= 60 && resetTimer <= 70) || (resetTimer >= 80 && resetTimer <= 90) || (resetTimer >= 100 && resetTimer <= 120) || (resetTimer >= 130 && resetTimer <= 140) || (resetTimer >= 150 && resetTimer <= 155))
			playerActiveAn->Render4V(points[3].x, points[3].y, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);

		if (isDebugMode) //Print FPS
		{
			GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
			GameShell::debugPrint((ixResolution / 2) - 27, 43, "%dFPS", GameShell::hge->Timer_GetFPS());
		}
	}
}

void Player::setAsSecondPlayer()
{
	HTEXTURE t;
	t = GameShell::hge->Texture_Load("Contents/LuigiSprite.png");
	playerRun = new hgeAnimation(t, 25, 40, 0, 0, 30, 35);
	playerRun->SetFlip(true, false);
	playerJump = new hgeAnimation(t, 20, 23, 0, 35, 30, 35);
	playerJump->SetFlip(true, false);
	playerLand = new hgeAnimation(t, 6, 16, 0, 70, 30, 35);
	playerLand->SetFlip(true, false);
	playerRoll = new hgeAnimation(t, 10, 20, 0, 105, 30, 35);
	playerRoll->SetFlip(true, false);
	playerActiveAn = playerLand;
	playerActiveAn->SetFlip(true, false);

	dimension = Vector2(18, 24);
	dimensionBK = dimension;
	jumpPower = 17.3;
	jumpPowerBK = jumpPower;
	weight = 1.05;
	walkingVelocity = Vector2(6.95, 6.95);
	runningVelocity = walkingVelocity * 1.97;
}

Vector2 Player::getPosition()
{
	return position;
}

bool Player::getColliding()
{
	return isCaughtByPlanet;
}

int Player::getCollidingPlanetIndex()
{
	return nearestPlanetIndex;
}

bool Player::checkDeathByEnemy(Enemy &enemy)
{
	Vector2 enemyHead = enemy.getCenterHead();
	float enemyHeadRadius = enemy.getHeadRadius();
	if (pow(centerHead.x - enemyHead.x, 2) + pow(centerHead.y - enemyHead.y, 2) <= pow(headRadius + enemyHeadRadius, 2))
		return true;
	return false;
}

bool Player::checkEnemyKilled(Enemy &enemy)
{
	Vector2 enemyHead = enemy.getCenterHead();
	float enemyHeadRadius = enemy.getHeadRadius();
	if (pow(centerFeet.x - enemyHead.x, 2) + pow(centerFeet.y - enemyHead.y, 2) <= pow(feetRadius + enemyHeadRadius, 2))
		return true;
	return false;
}

int Player::getLives()
{
	return lives;
}

int Player::getScore()
{
	return score;
}

int Player::getCollectedStars()
{
	return collectedStars;
}

bool Player::getInvertingGravity()
{
	if (playerNumber == 1)
	{
		if (GameShell::GetKeyState(HGEK_PERIOD) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			return true;
	}
	else
		if (GameShell::GetKeyState(HGEK_R) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			return true;
	return false;;
}

void Player::setInvertingGravity()
{
	isCaughtByPlanet = false;
	isJumping = true;
	inertia = true;
	inertialRight = false;
	inertialLeft = false;
	isFalling = true;
	isSmashing = false;
	jumpPower = jumpPowerBK;
	currentMovingVelocityBK = runningVelocity*0.9;
	FlyingTime = 0;
}