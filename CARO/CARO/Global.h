#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "raylib.h" 

// === Kích thước màn hình ===
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 950;

// === Logic Game ===
#define BOARD_SIZE 12 
enum Player { EMPTY = 0, X = -1, O = 1 };
enum GameStatus { PLAYING, X_WIN, O_WIN, DRAW };

// === KÍCH THƯỚC ĐÃ NÂNG CẤP ===
const int CELL_SIZE = 60;     // Tăng từ 40 lên 60
const int SPRITE_SIZE = 52;   // Tăng từ 32/48 lên 52
const int BOARD_WIDTH = BOARD_SIZE * CELL_SIZE; // Tự động = 720
const int BOARD_OFFSET_X = (SCREEN_WIDTH - BOARD_WIDTH) / 2; // Tự động = 600
// Căn giữa theo chiều dọc
const int BOARD_OFFSET_Y = (SCREEN_HEIGHT - BOARD_WIDTH) / 2;

// === BẢNG MÀU HOÀN CHỈNH (Theo ảnh cuối) ===

// 1. Nền ngoài (Theo ảnh cuối của bạn)
#define HSR_BG LIGHTGRAY 

// 2. Viền bàn cờ (Thiết kế 2 lớp)
#define HSR_FRAME CLITERAL(Color){ 45, 55, 75, 255 }       // Viền chính, nền panel
#define HSR_SUB_FRAME CLITERAL(Color){ 70, 85, 110, 255 }  // Viền phụ

// 3. Màu ô cờ (Checkerboard)
#define HSR_SQUARE_DARK CLITERAL(Color){ 60, 70, 90, 255 }
#define HSR_SQUARE_LIGHT CLITERAL(Color){ 210, 220, 230, 255 }

// === Trạng thái Game (Giữ nguyên) ===
enum GameScreen { MENU, GAMEPLAY, ABOUT, SETTING };

#endif // GLOBAL_H_

