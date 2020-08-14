#include "GameStateManager.h"
#include <stdlib.h>
#include "GameShell.h"
#include "GameStates.h"

GameStateManager::GameStateManager()
{
	pauseDelay = 0;
	controller = new XboxController(1);
	reset();
}

void GameStateManager::reset()
{
	theGameState = NULL;
	theNextGameState = NULL;
	prePauseState = NULL;
	pauseState = NULL;
}

void GameStateManager::init()
{
	if (theGameState != NULL)
	{
		theGameState->init();
	}
	else
	{
		if (theNextGameState != NULL)
		{
			if(theGameState)
			{
				theGameState->exit();
				delete theGameState;
			}

			theGameState = theNextGameState;
			theNextGameState = NULL;
		}

		if(theGameState)
		{
			theGameState->init();
		}
	}
}

void GameStateManager::setGameState(State* newState)
{
	theNextGameState = newState;
}

bool GameStateManager::update()
{
	if (pauseDelay > 0)
		pauseDelay--;
	if (pauseDelay == 0 && (GameShell::GetKeyState(HGEK_P) == GameShell::KeyState_Pressed || GameShell::GetKeyState(HGEK_ESCAPE) == GameShell::KeyState_Released || controller->getState().Gamepad.wButtons & XINPUT_GAMEPAD_START) && theGameState != NULL)
	{
		pauseDelay = 15;
		if(prePauseState != NULL) // currently in pause, so go back to the game
		{
			theGameState->exit();		
			theGameState = prePauseState;
			prePauseState = NULL;
		}
		else
		{
			//not in pause state, so see if we can transfer ..
			if(theGameState->canPause())
			{
				assert(prePauseState == NULL);

				prePauseState = theGameState;
				theGameState = pauseState;
				theGameState->init();
			}
		}
	}
	else
	{
		if (theNextGameState != NULL)
		{
			if(theGameState)
			{
				theGameState->exit();
				delete theGameState;
			}

			theGameState = theNextGameState;
			theNextGameState = NULL;

			theGameState->init();
			theGameState->frameCount = 0;
		}
	}
	
	if (theGameState == NULL)
	{
		return true;
	}

	return theGameState->update();
	
}

void GameStateManager::render()
{
	if(prePauseState != NULL)
	{
		prePauseState->render();
	}

	if (theGameState != NULL)
	{	
		theGameState->render();
	}
}

void GameStateManager::exit()
{
	if(theGameState != NULL)
	{
		theGameState->exit();
		delete theGameState;

		theGameState = NULL;
	}

	if(theNextGameState != NULL)
	{
		delete theNextGameState;

		theNextGameState = NULL;
	}
}

void GameStateManager::setPauseMode(GameStateManager::State* pauseState)
{
	this->pauseState = pauseState;
}

void GameStateManager::restartGameFromPauseMode(GameStateManager::State* restartState)
{
	pauseState = NULL;

	GameShell::setPauseMode(new GameState_Pause);

	if(theGameState != NULL)
	{
		delete theGameState;
		theGameState = NULL;
	}

	if(prePauseState != NULL)
	{
		prePauseState->exit();
		delete prePauseState;
		prePauseState = NULL;
	}

	if(restartState != NULL)
	{
		setGameState(restartState);
		update();
	}
}