#pragma once
#include "hge.h"
#include "hgefont.h"
#include "Vector3.h"
#include "Vector2.h"
#include <assert.h>
#include <stdlib.h>

#pragma warning(disable:4244 4309)

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))


//HGE 'forgot' to define these
#define HGEK_LCONTROL (0xa2)
#define HGEK_RCONTROL (0xa3)
#define HGEK_LSHIFT	(0xA0)
#define HGEK_RSHIFT (0xA1)


#include "GameStateManager.h"


static int ixResolution = 1280;
static int iyResolution = 720;


class GameShell
{
public:
	static int RGBATOCOLOR(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	static int RGBTOCOLOR(char r, char g, char b);
	static void drawRect(int x, int y, int w, int h, int c,HTEXTURE tex=NULL);
	static void print(int n, int x, int y, int flags, int colour, const char* fmt, ...);
	static void updateCamera(Vector2 charPosition);
	static void debugPrint(int x, int y, const char* fmt, ...);

	static HGE *hge;

	static bool initGame();
	static void shutdownGame();
	static bool frameFunc();
	static bool drawFunc();

	static void setState(GameStateManager::State* pState);
	static void setPauseMode(GameStateManager::State* pState);
	static void resetPauseMode(GameStateManager::State* restartState);
	
	static bool getRawKeyState(int keyCode);

	typedef enum KeyState{KeyState_NotPressed
		,KeyState_Pressed
		,KeyState_Held
		,KeyState_Released								
	};

	static KeyState GetKeyState(int keyCode);

	struct Vertex
	{
		Vertex()
		{
			position.x = 0;
			position.y = 0;
			position.z = 0;
			col = 0;
		}

		Vector3 position;
		int   col;
	};

	static void renderPoints(const Vertex* rVerts, unsigned int primCount);
	static void renderLines (const Vertex* rVerts, unsigned int primCount);

	static void drawCircle(const Vector2& position, float radius, int c = RGBTOCOLOR(255,255,255)); 

	struct Viewport
	{
		int x,y;
		int width,height;
	};

	static void SetViewport(const Viewport& v) {viewport = v;}
	static void GetViewport(Viewport& v) { v= viewport;}

	static int getFrameCount() { return iFrameCount; }
	static Viewport viewport;

private:
	static KeyState keyStates[256];

	static void processKeyStates();

	static GameStateManager theGameStateManager;

	static int iFrameCount;
};

#define PI (3.1416f)

float inline DEG_TO_RAD(float degree)
{
	return (PI * degree)/180.0f;
}