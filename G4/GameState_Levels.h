#pragma once
#include "Levels.h"

class GameState_LevelTutorial : public Levels
{
public:
	void init();
	bool update();
	void render();
};

class GameState_Level2 : public Levels
{
public:
	void init();
	bool update();
	void render();
};

class GameState_Level3 : public Levels
{
public:
	void init();
	bool update();
	void render();
};

class GameState_LevelMultiplayer : public Levels
{
public:
	void init();
	bool update();
	void render();
};