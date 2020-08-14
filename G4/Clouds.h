#pragma once
#include <vector>
#include "Matrix.h"
#include "hgesprite.h"
#include "GameShell.h"
using namespace std;

class Cloud
{
public:
	Cloud();
	void init(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void update(int LevelSizeXNegative, int LevelSizeXPositive, int LevelSizeYNegative, int LevelSizeYPositive);
	void render();
private:
	Vector2 position;
	Vector2 dimension;
	Vector2 instantVelocity;
	hgeSprite *cloudSprite;
};