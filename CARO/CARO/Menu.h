#ifndef _MENU_H_
#define _MENU_H_

#include "Global.h"

// Biến extern lưu lựa chọn (từ Global.h cũ)
extern int g_menuChoice; //

// Các hàm cho màn hình menu
void InitMenu();
void UpdateMenu(GameScreen& currentScreen); // Cập nhật logic menu
void DrawMenu();                           // Vẽ menu

// Các màn hình con
void DrawAbout();
void UpdateAbout(GameScreen& currentScreen);
void DrawSetting();
void UpdateSetting(GameScreen& currentScreen);

#endif // _MENU_H_
