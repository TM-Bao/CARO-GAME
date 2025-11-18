#ifndef _MENU_H_
#define _MENU_H_

#include "Global.h"
#include <string>
#include <map>
#include "raylib.h"


extern int g_menuChoice; 


extern std::map<std::string, Texture2D> game_textures;

//Màn hình Menu
void InitMenu();
void UpdateMenu(GameScreen& currentScreen); 
void DrawMenu();   // Vẽ menu
void DrawMenuArrow(Vector2 LeftArrow, Vector2 RightArrow);
void DrawImage();
void DrawTextAndBorder(const char* text, int posX, int posY, int Size, int Outline, Color BColor, Color TColor);


void DrawAbout();
void UpdateAbout(GameScreen& currentScreen);
void DrawSetting();
void UpdateSetting(GameScreen& currentScreen);

//Xóa tài nguyên
void UnloadAllTextures();

#endif 


