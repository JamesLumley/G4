#include "GameShell.h"
#include "Matrix.h"
#include <math.h>
#include <varargs.h>
#include <stdlib.h>
#include <stdio.h>
#include "Timer.h"

HGE* GameShell::hge = NULL;
hgeFont* debugFont = NULL;
hgeFont* menuFont = NULL;
GameShell::KeyState GameShell::keyStates[256] = {KeyState_NotPressed};
GameStateManager GameShell::theGameStateManager;
GameShell::Viewport GameShell::viewport;
int GameShell::iFrameCount = 0;

int GameShell::RGBATOCOLOR(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return b | (g<<8) | (r<<16) | (a<<24);
}

int GameShell::RGBTOCOLOR(char r, char g, char b)
{
	return RGBATOCOLOR(r,g,b,255);
}

void GameShell::drawRect(int x, int y, int w, int h, int c,HTEXTURE tex)
{
	hgeQuad quad;
	quad.blend=BLEND_ALPHABLEND | BLEND_ZWRITE;

	quad.tex = tex;
	quad.v[0].x=x;	 quad.v[0].y=y;		quad.v[0].z = 0;	quad.v[0].col = c;	quad.v[0].tx = 0; quad.v[0].ty = 0;
	quad.v[1].x=x+w; quad.v[1].y=y;		quad.v[1].z = 0;	quad.v[1].col = c;	quad.v[1].tx = 1; quad.v[1].ty = 0;
	quad.v[2].x=x+w; quad.v[2].y=y+h;	quad.v[2].z = 0;	quad.v[2].col = c;	quad.v[2].tx = 1; quad.v[2].ty = 1;
	quad.v[3].x=x;	 quad.v[3].y=y+h;	quad.v[3].z = 0;	quad.v[3].col = c;	quad.v[3].tx = 0; quad.v[3].ty = 1;

	hge->Gfx_RenderQuad(&quad);			
}

void GameShell::print(int n, int x, int y, int flags, int colour, const char* fmt, ...)
{
	char buffer[4096];
	hgeFont* fnt = NULL;
	if (n == 0 || NULL)
		fnt = debugFont;
	else if (n == 1)
		fnt = menuFont;

	va_list args;
	va_start( args, fmt );
	vsprintf_s( buffer, fmt, args );

	if(fnt != NULL)
	{
		fnt->SetColor(0xFF000000);
		fnt->printf(x+1, y+1, flags, buffer);
		fnt->SetColor(colour);
		fnt->printf(x, y, flags, buffer);
	}
}

void GameShell::debugPrint(int x, int y, const char* fmt, ...)
{
	char buffer[4096];

	va_list args;    

	va_start( args, fmt );

	vsprintf_s( buffer, fmt, args );

	print(0, x, y, HGETEXT_LEFT, RGBTOCOLOR(255, 255, 255), buffer);
}

bool GameShell::frameFunc()
{
	iFrameCount++;
	processKeyStates();

	return !theGameStateManager.update();
}

bool GameShell::drawFunc()
{
	int timer = (((hge->Timer_GetDelta()*1000))/19)+1;
	if(timer <= 19)
	{
		Sleep(19 - timer);
	}

	hge->Gfx_BeginScene();
	hge->Gfx_Clear(RGBTOCOLOR(7,145,255));

	bool bReturn = true;

	theGameStateManager.render();

	hge->Gfx_EndScene();

	return bReturn;
}

bool GameShell::initGame()
{
	debugFont = new hgeFont("Contents/Font/Font1.fnt");
	menuFont = new hgeFont("Contents/Font/Moonhouse.fnt");

	viewport.x = viewport.y = 0;
	viewport.width = hge->System_GetState(HGE_SCREENWIDTH);
	viewport.height = hge->System_GetState(HGE_SCREENHEIGHT);

	return true;
}

void GameShell::shutdownGame()
{
	theGameStateManager.exit();
}

bool GameShell::getRawKeyState(int keyCode)
{
	return GameShell::hge->Input_GetKeyState(keyCode);
}

GameShell::KeyState GameShell::GetKeyState(int keyCode)
{
	assert(keyCode < 256);
	return keyStates[keyCode];
}

void GameShell::processKeyStates()
{
	for(int i=0;i<256;i++)
	{
		bool pressed = getRawKeyState(i);

		switch(keyStates[i])
		{
		case KeyState_NotPressed:
			if(pressed == true)
			{
				keyStates[i] = KeyState_Pressed;
			}
			break;

		case KeyState_Pressed:
		case KeyState_Held:
			if(pressed == true)
			{
				keyStates[i] = KeyState_Held;
			}
			else
			{
				keyStates[i] = KeyState_Released;
			}
			break;

		case KeyState_Released:		
			if(pressed == true)
			{
				keyStates[i] = KeyState_Pressed;
			}
			else
			{
				keyStates[i] = KeyState_NotPressed;
			}
			break;
		}
	}
}

void GameShell::setState(GameStateManager::State* pState)
{
	theGameStateManager.setGameState(pState);
}

void GameShell::setPauseMode(GameStateManager::State* pState)
{
	theGameStateManager.setPauseMode(pState);
}

void GameShell::resetPauseMode(GameStateManager::State* restartState)
{
	theGameStateManager.restartGameFromPauseMode(restartState);
}

void GameShell::renderLines (const Vertex* rVerts, unsigned int PrimCount)
{
	for(unsigned int i=0;i<PrimCount;i++)
	{
		hge->Gfx_RenderLine(rVerts[(i*2)+0].position.x
			,rVerts[(i*2)+0].position.y
			,rVerts[(i*2)+1].position.x
			,rVerts[(i*2)+1].position.y
			,rVerts[(i*2)+0].col);
	}
}

void GameShell::drawCircle(const Vector2& Position, float radius,int c)
{
	Vector2 Points[20];

	for(int i=0;i<ARRAY_LENGTH(Points);i++)
	{
		Points[i] = Vector2::Transform(Vector2(0,radius), Matrix::CreateRotationZ(DEG_TO_RAD((i*360)/ARRAY_LENGTH(Points))));
	}

	Matrix transform = Matrix::CreateTranslation(Position.x,Position.y,0);

	for (unsigned int i = 0; i < ARRAY_LENGTH(Points); i++)
	{		
		Points[i] = Vector2::Transform(Points[i],transform);
	}

	GameShell::Vertex v[ARRAY_LENGTH(Points)*2];

	int index=0;

	for(unsigned int i=0;i < ARRAY_LENGTH(Points);i++)
	{
		v[index].position.x = Points[i+0].x;
		v[index].position.y = Points[i+0].y;
		v[index].position.z = 0;

		v[index].col = c;	
		index++;

		v[index].position.x = Points[(i+1)%ARRAY_LENGTH(Points)].x;
		v[index].position.y = Points[(i+1)%ARRAY_LENGTH(Points)].y;
		v[index].position.z = 0;

		v[index].col = c;	
		index++;

	}

	GameShell::renderLines(v,index/2);
}