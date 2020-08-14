#include "AudioManager.h"

HEFFECT AudioManager::BackgroundSong = NULL;

void AudioManager::init()
{
	StartBackgroundSound();
}

void AudioManager::StartBackgroundSound()
{
	BackgroundSong = NULL;
	BackgroundSong = GameShell::hge->Effect_Load("Contents/Kalimba.mp3");
	GameShell::hge->Effect_PlayEx(BackgroundSong, 70, 0, 1.0f, true);
}

void AudioManager::StopBackgroundSound()
{
	GameShell::hge->Effect_Free(BackgroundSong);
}