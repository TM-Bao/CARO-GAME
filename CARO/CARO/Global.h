#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "raylib.h" 

//Kích thước màn hình
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 950;

//Logic Game
#define BOARD_SIZE 12 
enum Player { EMPTY = 0, X = -1, O = 1 };
enum GameStatus { PLAYING, X_WIN, O_WIN, DRAW };
enum InputMode { INPUT_KEYBOARD, INPUT_MOUSE };

//Set up bàn cờ
const int CELL_SIZE = 60;     
const int SPRITE_SIZE = 52;   
const int BOARD_WIDTH = BOARD_SIZE * CELL_SIZE; 
const int BOARD_OFFSET_X = (SCREEN_WIDTH - BOARD_WIDTH) / 2; 
const int BOARD_OFFSET_Y = (SCREEN_HEIGHT - BOARD_WIDTH) / 2;


//Tô màu
#define HSR_BG LIGHTGRAY 
#define HSR_FRAME CLITERAL(Color){ 45, 55, 75, 255 }       
#define HSR_SUB_FRAME CLITERAL(Color){ 70, 85, 110, 255 } 
#define HSR_SQUARE_DARK CLITERAL(Color){ 60, 70, 90, 255 }
#define HSR_SQUARE_LIGHT CLITERAL(Color){ 210, 220, 230, 255 }

enum GameScreen { MENU, GAMEPLAY, ABOUT, SETTING };

#endif 

