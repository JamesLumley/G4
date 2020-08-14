#pragma once
#include "GameStateManager.h"
#include "XboxController.h"

class GameState_Menu : public GameStateManager::State
{
public:
	void init();
	bool update();
	void render();
	void exit();
private:
	XboxController* controller;
	int selection;
	int moveDelay;
};