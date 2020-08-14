#pragma once
#include "XboxController.h"

class GameStateManager
{	
	public:

		class State
		{
			public:
				virtual void init(){frameCount = 0;}
				virtual bool update()
				{
					frameCount++;
					return true;
				}
				virtual void render(){};
				virtual void exit(){};

				virtual bool canPause(){return false;}

				int frameCount;
		};

		class PausibleState : public State
		{
			public:
				virtual bool canPause(){return true;}
		};

		GameStateManager();
		
		void init();
		
		void setGameState(State* NewState);
		
		bool update();
		
		void render();
		
		void exit();
	
		State* getCurrentState()
		{
			return theGameState;
		}

		void restartGameFromPauseMode(GameStateManager::State* restartState);
		void setPauseMode            (GameStateManager::State* pauseState);

	private:

		void reset();
		XboxController* controller;
		int pauseDelay;

		State* pauseState;
		State* theGameState;
		State* theNextGameState;

		State* prePauseState;
};