#include <hge.h>
#include "GameShell.h"
#include "GameStates.h"

HGE* hge = NULL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameShell::hge = hgeCreate(HGE_VERSION);
	GameShell::hge->System_SetState(HGE_FRAMEFUNC, GameShell::frameFunc);
	GameShell::hge->System_SetState(HGE_RENDERFUNC, GameShell::drawFunc);	
	GameShell::hge->System_SetState(HGE_TITLE, "G4 - Group 4 Game");	
	GameShell::hge->System_SetState(HGE_WINDOWED, true);
	GameShell::hge->System_SetState(HGE_SHOWSPLASH, false);
	GameShell::hge->System_SetState((hgeIntState)17, ixResolution);
	GameShell::hge->System_SetState((hgeIntState)18, iyResolution);
	GameShell::hge->System_SetState(HGE_USESOUND, true);
	//TOTemp: Only for debug
	//GameShell::hge->System_SetState(HGE_DONTSUSPEND, true);

	GameShell::setPauseMode(new GameState_Pause);
	GameState_NextLevelAndGameOver::nextState = 9;
	GameShell::setState(new GameState_NextLevelAndGameOver);

	if( GameShell::hge->System_Initiate() && GameShell::initGame() == true)
		GameShell::hge->System_Start();
	else
		MessageBox(NULL, GameShell::hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);

	GameShell::shutdownGame();

	GameShell::hge->System_Shutdown();
	GameShell::hge->Release();

	return 0;
}