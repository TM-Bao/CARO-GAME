#include "Audio.h"
#include "raylib.h"
#include "settings.h"
Sound fxMove;
Sound fxWin;
Music bgMusic;
void InitAudio() {
	bgMusic = LoadMusicStream("resources/cerydra_music.mp3");
	fxMove = LoadSound("resources/place_chess.mp3");
	fxWin = LoadSound("resources/winner.mp3");
	SetMusicVolume(bgMusic, 0.5f);
	PlayMusicStream(bgMusic);
	SetMusicVolume(bgMusic, (float)g_settings.musicVolume / 100.0f);
}
void UnloadAudio() {
	UnloadMusicStream(bgMusic);
	CloseAudioDevice();
}
