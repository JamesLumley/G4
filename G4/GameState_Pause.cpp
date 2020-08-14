#include"GameStates.h"
#include "AudioManager.h"

GameState_Pause::~GameState_Pause()
{
	delete controller;
}

void GameState_Pause::init()
{
	AudioManager::StopBackgroundSound();
	controller = new XboxController(1);
}
bool GameState_Pause::update() 
{
	controller->vibrate(0, 0);
	if (GameShell::GetKeyState(HGEK_ESCAPE) == GameShell::KeyState_Pressed || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		// quit game and return to menu
		GameShell::resetPauseMode(new GameState_Menu);
	}	
	return true;
}

void GameState_Pause::render()
{
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);

	GameShell::drawRect(380, 255, 650, 150, GameShell::RGBTOCOLOR(255, 255, 255));
	GameShell::drawRect(385, 260, 640, 140, GameShell::RGBTOCOLOR(0, 0, 0));

	GameShell::print(1, 600, 270, 0, GameShell::RGBATOCOLOR(255,255,255,255),"PAUSE");
	GameShell::print(1, 405, 320, 0, GameShell::RGBATOCOLOR(255,255,255,255),"P or Start - Resume");
	GameShell::print(1, 405, 349, 0, GameShell::RGBATOCOLOR(255,255,255,255),"Esc or Back - Back Main Menu");			
}

void GameState_Pause::exit()
{
	AudioManager::StartBackgroundSound();
}
