#pragma once
#include "GameStateManager.h"
#include "GameShell.h"
#include "hgesprite.h"
#include "XboxController.h"

class GameState_ShowImage : public GameStateManager::State
{
public:
	void init();
	bool update();
	void render();
	void exit();
	static int imageNumber;
private:
	XboxController* controller;
	hgeSprite* Controls;
	hgeSprite* Credits;
};
