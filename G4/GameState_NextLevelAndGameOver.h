#pragma once
#include "GameStateManager.h"
#include "GameShell.h"
class GameState_NextLevelAndGameOver : public GameStateManager::State
{
public:
	void init();
	bool update();
	void render();
	void exit();
	static int nextState;
};