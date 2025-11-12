#ifndef _GAME_H_
#define _GAME_H_

#include "Global.h"
#include <string> // (cho save/load)
#include <map>

// === Các biến trạng thái game (từ Game.cpp cũ) ===
// Chúng ta "extern" để các file khác có thể ĐỌC
extern int g_board[BOARD_SIZE][BOARD_SIZE];
extern bool g_turn;
extern GameStatus g_status;

// === Các hàm logic (từ Game.h cũ) ===
void InitGame();      // Thay thế cho resetData
void UpdateGame();    // Xử lý input và logic cho màn hình game
GameStatus TestBoard(); // Giữ nguyên logic testBoard
bool CheckAndPlace(int cellX, int cellY); // Kết hợp logic checkBoard

// Các hàm save/load
void saveGame(const std::string& filename);
void loadGame(const std::string& filename);

#endif // _GAME_H_
