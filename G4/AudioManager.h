#pragma once
#include "GameShell.h"

class AudioManager
{
private:
	static HEFFECT BackgroundSong;
public:
	static void init();
	static void StartBackgroundSound();
	static void StopBackgroundSound();
};

