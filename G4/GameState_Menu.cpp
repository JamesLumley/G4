#include "GameStates.h"
#include "AudioManager.h"

void GameState_Menu::init()
{
	controller = new XboxController(1);
	selection = 0;
	moveDelay = 0;
}

bool GameState_Menu::update()
{
	if (moveDelay > 0)
		moveDelay--;
	controller->vibrate(0, 0);
	if (GameShell::GetKeyState(HGEK_ESCAPE) == GameShell::KeyState_Pressed)
		return false;

	if (GameShell::GetKeyState(HGEK_DOWN) == GameShell::KeyState_Pressed || (controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && moveDelay == 0))
	{
		if (selection < 6)
		{
			moveDelay = 5;
			selection++;
		}
	}
	if (GameShell::GetKeyState(HGEK_UP) == GameShell::KeyState_Pressed || (controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && moveDelay == 0))
	{
		if (selection > 0)
		{
			moveDelay = 5;
			selection--;
		}
	}

	if (GameShell::GetKeyState(HGEK_ENTER) == GameShell::KeyState_Released || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (selection == 4)
		{
			GameState_ShowImage::imageNumber = 0;
			GameShell::setState(new GameState_ShowImage);
		}
		if (selection == 5)
		{
			GameState_ShowImage::imageNumber = 1;
			GameShell::setState(new GameState_ShowImage);
		}
	}
	if (GameShell::GetKeyState(HGEK_ENTER) || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (selection == 0)
		{
			GameState_NextLevelAndGameOver::nextState = 1;
			GameShell::setState(new GameState_NextLevelAndGameOver);
		}
		if (selection == 1)
		{
			GameState_NextLevelAndGameOver::nextState = 6;
			GameShell::setState(new GameState_NextLevelAndGameOver);
		}
		if (selection == 2)
		{
			GameState_NextLevelAndGameOver::nextState = 7;
			GameShell::setState(new GameState_NextLevelAndGameOver);
		}
		if (selection == 3)
		{
			GameState_NextLevelAndGameOver::nextState = 8;
			GameShell::setState(new GameState_NextLevelAndGameOver);
		}
		if (selection == 6)
			return false;
	}

	return true;
}

void GameState_Menu::render()
{
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	GameShell::print(1, ixResolution * 0.405, iyResolution * 0.22, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "G4 Menu");
	if (selection == 0)
		GameShell::print(1, ixResolution * 0.4, iyResolution * 0.4, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Campaign");
	else
		GameShell::print(1, ixResolution * 0.4, iyResolution * 0.4, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Campaign");
	if (selection == 1)
		GameShell::print(1, ixResolution * 0.415, iyResolution * 0.45, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Level 1");
	else
		GameShell::print(1, ixResolution * 0.415, iyResolution * 0.45, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Level 1");
	if (selection == 2)
		GameShell::print(1, ixResolution * 0.415, iyResolution * 0.5, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Level 2");
	else
		GameShell::print(1, ixResolution * 0.415, iyResolution * 0.5, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Level 2");
	if (selection == 3)
		GameShell::print(1, ixResolution * 0.26, iyResolution * 0.55, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Endless Competitive Multiplayer");
	else
		GameShell::print(1, ixResolution * 0.26, iyResolution * 0.55, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Endless Competitive Multiplayer");
	if (selection == 4)
		GameShell::print(1, ixResolution * 0.403, iyResolution * 0.6, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Controls");
	else
		GameShell::print(1, ixResolution * 0.403, iyResolution * 0.6, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Controls");
	if (selection == 5)
		GameShell::print(1, ixResolution * 0.417, iyResolution * 0.65, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Credits");
	else
		GameShell::print(1, ixResolution * 0.417, iyResolution * 0.65, 0, GameShell::RGBATOCOLOR(255, 0, 255, 255), "Credits");
	if (selection == 6)
		GameShell::print(1, ixResolution * 0.43, iyResolution * 0.7, 0, GameShell::RGBATOCOLOR(255, 255, 0, 255), "Exit");
	else
		GameShell::print(1, ixResolution * 0.43, iyResolution * 0.7, 0, GameShell::RGBATOCOLOR(0, 0, 0, 255), "Exit");
}

void GameState_Menu::exit()
{

}