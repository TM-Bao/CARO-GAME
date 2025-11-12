#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "raylib.h" // Thêm thư viện Raylib vào đây

// === Tinh chỉnh từ file Global.h cũ ===
// 1. Kích thước logic (giữ nguyên từ file cũ)
#define BOARD_SIZE 12 //

// 2. Enums (giữ nguyên từ file cũ)
enum Player { EMPTY = 0, X = -1, O = 1 }; //
enum GameStatus { PLAYING, X_WIN, O_WIN, DRAW }; //

// === Hằng số mới cho Raylib (thay thế LEFT, TOP) ===
const int CELL_SIZE = 40;     // Kích thước mỗi ô
const int SPRITE_SIZE = 32;   // Kích thước X, O
const int BOARD_OFFSET_X = 150; // Vị trí bàn cờ
const int BOARD_OFFSET_Y = 100; // Vị trí bàn cờ
const int BOARD_WIDTH = BOARD_SIZE * CELL_SIZE;

// Màu sắc
#define GRASS_GREEN CLITERAL(Color){ 34, 139, 34, 255 }
#define BOARD_BG CLITERAL(Color){ 210, 180, 140, 255 }
#define BOARD_FRAME CLITERAL(Color){ 139, 69, 19, 255 }
#define BOARD_LINE CLITERAL(Color){ 101, 67, 33, 255 }

// === Trạng thái toàn cục của ứng dụng ===
// Dùng để chuyển cảnh
enum GameScreen { MENU, GAMEPLAY, ABOUT, SETTING };

#endif // _GLOBAL_H_
