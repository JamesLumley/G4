#pragma once
#include "GameStateManager.h"
#include "GameShell.h"
#include "XboxController.h"

class GameState_Pause : public GameStateManager::State
{
public:
	~GameState_Pause();
	void init();
	bool update();
	void render();
	void exit();
private:
	XboxController* controller;
};
