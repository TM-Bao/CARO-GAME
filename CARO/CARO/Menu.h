#ifndef _MENU_H_
#define _MENU_H_

#include "Global.h"
#include <string>
#include <map>
#include "raylib.h"

// Biến extern lưu lựa chọn (từ Global.h cũ)
extern int g_menuChoice; //

// Map extern lưu trữ các textures (từ menu.cpp)
extern std::map<std::string, Texture2D> game_textures;

// Các hàm cho màn hình menu
void InitMenu();
void UpdateMenu(GameScreen& currentScreen); // Cập nhật logic menu
void DrawMenu();   // Vẽ menu
void DrawMenuArrow(Vector2 LeftArrow, Vector2 RightArrow);
void DrawImage();
void DrawTextAndBorder(const char* text, int posX, int posY, int Size, int Outline, Color BColor, Color TColor);

// Các màn hình con
void DrawAbout();
void UpdateAbout(GameScreen& currentScreen);
void DrawSetting();
void UpdateSetting(GameScreen& currentScreen);

// Hàm xóa toàn bộ textures trước khi thoát
void UnloadAllTextures();

#endif // _MENU_H_


