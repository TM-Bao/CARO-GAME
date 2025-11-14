#ifndef _GAME_H_
#define _GAME_H_

#include "Global.h"
#include <string> // (cho save/load)
#include <map>

extern int g_board[BOARD_SIZE][BOARD_SIZE];
extern bool g_turn;
extern GameStatus g_status;
extern int g_cursorX;
extern int g_cursorY;

// === Các hàm logic (từ Game.h cũ) ===
void InitGame();      // Thay thế cho resetData
void UpdateGame();    // Xử lý input và logic cho màn hình game
GameStatus TestBoard(); 
bool CheckAndPlace(int cellX, int cellY); // Kết hợp logic checkBoard

// Các hàm save/load
void saveGame(const std::string& filename);
void loadGame(const std::string& filename);

#endif // _GAME_H_
