#include "Audio.h"
#include "raylib.h"
Sound fxMove;
Sound fxWin;
Music bgMusic;
void InitAudio() {
	bgMusic = LoadMusicStream("resources/cerydra_music.mp3");
	SetMusicVolume(bgMusic, 0.5f);
	PlayMusicStream(bgMusic);
}
void UnloadAudio() {
	UnloadMusicStream(bgMusic);
	CloseAudioDevice();
}
