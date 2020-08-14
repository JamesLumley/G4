#include"GameStates.h"
#include"AudioManager.h"
#include"Levels.h"

int GameState_ShowImage::imageNumber = 0;

void GameState_ShowImage::init()
{
	controller = new XboxController(1);
	Controls = new hgeSprite(GameShell::hge->Texture_Load("Contents/Controls.png"), 0, 0, ixResolution, iyResolution);
	Credits = new hgeSprite(GameShell::hge->Texture_Load("Contents/Credits.png"), 0, 0, ixResolution, iyResolution);
}

bool GameState_ShowImage::update()
{
	if (GameShell::GetKeyState(HGEK_ENTER) == GameShell::KeyState_Released || GameShell::GetKeyState(HGEK_SPACE) == GameShell::KeyState_Released || GameShell::GetKeyState(HGEK_BACKSPACE) == GameShell::KeyState_Released || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
		GameShell::setState(new GameState_Menu);
	return State::update();
}

void GameState_ShowImage::render()
{
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	if (imageNumber == 0)
	{
		Controls->Render4V(0, 0, ixResolution, 0, ixResolution, iyResolution, 0, iyResolution);
		GameShell::print(1, ixResolution * 0.407, iyResolution * 0.09, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Controls");
	}
	else if (imageNumber == 1)
	{
		Credits->Render4V(0, 0, ixResolution, 0, ixResolution, iyResolution, 0, iyResolution);
		GameShell::print(1, ixResolution * 0.397, iyResolution * 0.09, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "G4: Credits");
	}
}

void GameState_ShowImage::exit()
{
	delete controller;
	delete Controls;
	delete Credits;
}