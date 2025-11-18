#ifndef _VIEW_H_
#define _VIEW_H_

#include "Global.h"


//Vẽ X và O
Texture2D GenXTexture(int size, Color color);
Texture2D GenOTexture(int size, Color color);

// Tài nguyên đồ họa
void InitGameView();
void UnloadGameView();


void DrawGameView();

#endif 
