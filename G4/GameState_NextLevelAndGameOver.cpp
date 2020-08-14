#include"GameStates.h"
#include"AudioManager.h"
#include"Levels.h"

int GameState_NextLevelAndGameOver::nextState = 0;

void GameState_NextLevelAndGameOver::init()
{
	AudioManager::StopBackgroundSound();
}

bool GameState_NextLevelAndGameOver::update()
{
	if (nextState == 5)
		nextState = 0;

	if (nextState == 6)
	{
		GameShell::setState(new GameState_Level2);
		nextState = 5;
	}
	if (nextState == 7)
	{
		GameShell::setState(new GameState_Level3);
		nextState = 5;
	}
	if (nextState == 8)
	{
		GameShell::setState(new GameState_LevelMultiplayer);
		nextState = 5;
	}
	if (nextState == 9)
	{
		GameShell::setState(new GameState_Menu);
		nextState = 5;
	}
	if (frameCount > (4 * 30))
	{
		if (nextState == 0)
			GameShell::setState(new GameState_Menu);
		else if (nextState == 1)
			GameShell::setState(new GameState_LevelTutorial);
		else if (nextState == 2)
			GameShell::setState(new GameState_Level2);
		else if (nextState == 3)
			GameShell::setState(new GameState_Level3);
		else if (nextState == 4)
			GameShell::setState(new GameState_Menu);
		nextState = 5;
	}
	return State::update();
}

void GameState_NextLevelAndGameOver::render()
{
	GameShell::hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
	if (nextState == 0)
		GameShell::print(1, 540, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "GAME OVER");
	else if (nextState == 1)
		GameShell::print(1, 470, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Welcome: Level 1 Tutorial");
	else if (nextState == 2)
		GameShell::print(1, 500, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Passing to Level 2");
	else if (nextState == 3)
		GameShell::print(1, 500, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Passing to Level 3");
	else if (nextState == 4)
		GameShell::print(1, 500, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Campaign Finished");
	else //if (nextState == 5)
		GameShell::print(1, 540, 330, 0, GameShell::RGBATOCOLOR(255, 255, 255, 255), "Loading...");
}

void GameState_NextLevelAndGameOver::exit()
{

}