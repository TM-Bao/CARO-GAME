#ifndef _GAME_H_
#define _GAME_H_

#include "Global.h"
#include <string> 
#include <map>

extern int g_board[BOARD_SIZE][BOARD_SIZE];
extern bool g_turn;
extern GameStatus g_status;
extern int g_cursorX;
extern int g_cursorY;


void InitGame();      
void UpdateGame(GameScreen& currentScreen);    
GameStatus TestBoard();
bool CheckAndPlace(int cellX, int cellY); 


void saveGame(const std::string& filename);
void loadGame(const std::string& filename);

#endif // _GAME_H_

