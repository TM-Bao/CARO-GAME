#include "Game.h"
#include "raylib.h"
#include <fstream> 
#include <iostream> 
#include "settings.h"
#include "Audio.h"

int g_board[BOARD_SIZE][BOARD_SIZE];
bool g_turn;
GameStatus g_status;
int g_cursorX;
int g_cursorY;
int winStartX = -1;
int winStartY = -1;
int winEndX = -1;
int winEndY = -1;

void InitGame() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            g_board[i][j] = EMPTY;
        }
    }
    g_turn = true; // X đi trước
    g_status = PLAYING;
    //Reset con trỏ WASD
    g_cursorX = BOARD_SIZE / 2;
    g_cursorY = BOARD_SIZE / 2;
}

void UpdateGame(GameScreen& currentScreen) {
    Rectangle btnSetting = { (float)SCREEN_WIDTH - 170, 20, 150, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), btnSetting)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            g_previousScreen = GAMEPLAY;
            currentScreen = SETTING;
            return;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (g_status == PLAYING) {
            g_status = PAUSE;
        }
        else if (g_status == PAUSE) {
            g_status = PLAYING;
        }
        else if (g_status == X_WIN || g_status == O_WIN || g_status == DRAW) {
            currentScreen = MENU;
        }
    }

    if (g_status == PAUSE) {
        if (IsKeyPressed(KEY_N)) currentScreen = MENU;
        if (IsKeyPressed(KEY_C)) g_status = PLAYING;

        // Save slots
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
            saveGame("save1.txt");
            g_status = PLAYING; 
        }
        if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
            saveGame("save2.txt");
            g_status = PLAYING;
        }
        if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
            saveGame("save3.txt");
            g_status = PLAYING;
        }
        return; 
    }
    if (g_status != PLAYING && g_status != PAUSE) {
        if (IsKeyPressed(KEY_Y)) InitGame();
        return;
    }

    // 1. INPUT KEYBOARD
    if (g_settings.inputMode == INPUT_KEYBOARD) {
        if (IsKeyPressed(KEY_W) && g_cursorY > 0) g_cursorY--;
        if (IsKeyPressed(KEY_S) && g_cursorY < BOARD_SIZE - 1) g_cursorY++;
        if (IsKeyPressed(KEY_A) && g_cursorX > 0) g_cursorX--;
        if (IsKeyPressed(KEY_D) && g_cursorX < BOARD_SIZE - 1) g_cursorX++;
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (CheckAndPlace(g_cursorX, g_cursorY)) {
                if (g_settings.soundEnable) PlaySound(fxMove);
                g_status = TestBoard();
                if (g_status == PLAYING) g_turn = !g_turn;
                else if (g_settings.soundEnable && (g_status == X_WIN || g_status == O_WIN)) {
                    PlaySound(fxWin);
                }
            }
        }
    }
    // 2. INPUT MOUSE
    else if (g_settings.inputMode == INPUT_MOUSE) {
        Vector2 mousePos = GetMousePosition();
        int cellX = (mousePos.x - BOARD_OFFSET_X) / CELL_SIZE;
        int cellY = (mousePos.y - BOARD_OFFSET_Y) / CELL_SIZE;

        if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE) {
            g_cursorX = cellX;
            g_cursorY = cellY;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckAndPlace(cellX, cellY)) {
                    if (g_settings.soundEnable) PlaySound(fxMove);
                    g_status = TestBoard();
                    if (g_status == PLAYING) g_turn = !g_turn;
                    else if (g_settings.soundEnable && (g_status == X_WIN || g_status == O_WIN)) {
                        PlaySound(fxWin);
                    }
                }
            }
        }
    }
}

bool CheckAndPlace(int cellX, int cellY) {
    if (g_board[cellY][cellX] == EMPTY) {
        g_board[cellY][cellX] = (g_turn ? X : O);
        return true;
    }
    return false;
}

bool isFull() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (g_board[i][j] == EMPTY) return false;
    return true;
}

GameStatus TestBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Player c = (Player)g_board[i][j];
            if (c == EMPTY) continue;
            // Check ngang
            if (j + 4 < BOARD_SIZE && c == g_board[i][j + 1] && c == g_board[i][j + 2] && c == g_board[i][j + 3] && c == g_board[i][j + 4]) {
                winStartX = j; winStartY = i; winEndX = j + 4; winEndY = i;
                return (c == X) ? X_WIN : O_WIN;
            }
            // Check dọc
            if (i + 4 < BOARD_SIZE && c == g_board[i + 1][j] && c == g_board[i + 2][j] && c == g_board[i + 3][j] && c == g_board[i + 4][j]) {
                winStartX = j; winStartY = i; winEndX = j; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }
            // Check chéo chính
            if (i + 4 < BOARD_SIZE && j + 4 < BOARD_SIZE && c == g_board[i + 1][j + 1] && c == g_board[i + 2][j + 2] && c == g_board[i + 3][j + 3] && c == g_board[i + 4][j + 4]) {
                winStartX = j; winStartY = i; winEndX = j + 4; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }
            // Check chéo phụ
            if (i + 4 < BOARD_SIZE && j >= 4 && c == g_board[i + 1][j - 1] && c == g_board[i + 2][j - 2] && c == g_board[i + 3][j - 3] && c == g_board[i + 4][j - 4]) {
                winStartX = j; winStartY = i; winEndX = j - 4; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }
        }
    }
    return isFull() ? DRAW : PLAYING;
}

void saveGame(const std::string& filename) {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cout << "Error: Cannot create save file " << filename << std::endl;
        return;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) f << g_board[i][j] << " ";
        f << "\n";
    }
    f << (g_turn ? 1 : 0) << std::endl;
    f.close();
    std::cout << "Game saved to " << filename << std::endl;
}

bool loadGame(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "Cannot load: File not found " << filename << std::endl;
        return false;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int val;
            f >> val;
            g_board[i][j] = val;
        }
    }

    int turnVal;
    f >> turnVal;
    g_turn = (turnVal == 1);

    f.close();
    g_cursorX = BOARD_SIZE / 2;
    g_cursorY = BOARD_SIZE / 2;
    g_status = TestBoard();
    if (g_status != X_WIN && g_status != O_WIN && g_status != DRAW) {
        g_status = PLAYING;
    }
    return true;
}
