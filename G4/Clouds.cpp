#include "Clouds.h"

Cloud::Cloud()
{
	float RandNum = ((rand() % 20) + 5) / 20.0;
	dimension = Vector2(RandNum, RandNum);
	position = Vector2(0, 0);
	instantVelocity = Vector2(((rand() % 4) + 2) / 2, (rand() % 2 - 0.5) / 4);
	if (hasToLoadSprites)
	{
		cloudSprite = new hgeSprite(GameShell::hge->Texture_Load("Contents/Cloud.png"), 0, 0, 301, 132);
		cloudSprite->SetHotSpot(301 / 2.0, 131 / 2.0);
		hasToLoadSprites = false;
	}
}

void Cloud::init(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	position = Vector2((rand() % (LevelSizeXPositive * 2)) - LevelSizeXPositive, (rand() % (LevelSizeYPositive * 2)) - LevelSizeYPositive);
}

void Cloud::update(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive)
{
	position += instantVelocity;
	if (position.x > LevelSizeXPositive + (301 * dimension.x))
		position = Vector2(LevelSizeXNegative - (301 * dimension.x), (rand() % (LevelSizeYPositive * 2)) - LevelSizeYPositive);
}

void Cloud::render()
{
	cloudSprite->RenderEx(position.x, position.y, 0, dimension.x, dimension.y);
}