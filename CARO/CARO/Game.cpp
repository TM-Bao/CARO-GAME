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

//XỬ LÍ DI CHUYỂN ĐỘC LẬP: WADS_X , ↑ ↓ ← →_Y
void Movement_X(int& cursorX, int& cursorY) {
    static float time = 0;
    time -= GetFrameTime();

    if (time <= 0) {
        if (IsKeyDown(KEY_W) && cursorY > 0) {
            cursorY--;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_S) && cursorY < BOARD_SIZE - 1) {
            cursorY++;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_A) && cursorX > 0) {
            cursorX--;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_D) && cursorX < BOARD_SIZE - 1) {
            cursorX++;
            time = 0.12f;
        }
    }
}
void Movement_Y(int& cursorX, int& cursorY) {
    static float time = 0;
    time -= GetFrameTime();

    if (time <= 0) {
        if (IsKeyDown(KEY_UP) && cursorY > 0) {
            cursorY--;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_DOWN) && cursorY < BOARD_SIZE - 1) {
            cursorY++;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_LEFT) && cursorX > 0) {
            cursorX--;
            time = 0.12f;
        }
        if (IsKeyDown(KEY_RIGHT) && cursorX < BOARD_SIZE - 1) {
            cursorX++;
            time = 0.12f;
        }
    }
}

void UpdateGame(GameScreen& currentScreen) {
    Rectangle btnSetting = { (float)SCREEN_WIDTH - 170, 20, 150, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), btnSetting)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentScreen = SETTING;
            return;
        }
    }
    if (g_status != PLAYING && g_status != PAUSE) {
        if (IsKeyPressed(KEY_Y)) InitGame();
        return;
        if (IsKeyPressed(KEY_Y)) currentScreen = MENU;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        g_status = PAUSE;
    }
    if (g_status == PAUSE && IsKeyPressed(KEY_N)) {
        currentScreen = MENU;
    }
        saveGame("save3.txt");
        currentScreen = MENU;
    } 
//XỬ LÝ LOGIC CHƠI BẰNG BÀN PHÍM
    if (g_settings.inputMode == INPUT_KEYBOARD) {
        if (g_turn) Movement_X(g_cursorX, g_cursorY); //X
        else Movement_Y(g_cursorX, g_cursorY);  //O
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (CheckAndPlace(g_cursorX, g_cursorY)) {
                if (g_settings.soundEnable) {
                    PlaySound(fxMove);
                }
                g_status = TestBoard();
				if (g_settings.soundEnable) PlaySound(fxMove);
                if (g_status == PLAYING) g_turn = !g_turn;
                else if (g_settings.soundEnable && (g_status == X_WIN || g_status == O_WIN)) {
                    if (g_settings.soundEnable) {
                        PlaySound(fxWin);
                    }
				}
            }

        }
    }
//XỬ LÝ LOGIC CHƠI BẰNG CHUỘT
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
                        if (g_settings.soundEnable) {
                            PlaySound(fxWin);
                        }
                    }
                }
            }
        }
    }
    if (IsKeyPressed(KEY_L)) saveGame("save.txt");
}

bool CheckAndPlace(int cellX, int cellY) {
    if (g_board[cellY][cellX] == EMPTY) {
        g_board[cellY][cellX] = (g_turn ? X : O);
        return true; // Đặt cờ thành công
    }
    return false; // Ô đã có người đánh
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
            if (j + 4 < BOARD_SIZE &&
                c == g_board[i][j + 1] &&
                c == g_board[i][j + 2] &&
                c == g_board[i][j + 3] &&
                c == g_board[i][j + 4]) {
                winStartX = j; winStartY = i;
                winEndX = j + 4; winEndY = i;
                return (c == X) ? X_WIN : O_WIN;
            }
            if (i + 4 < BOARD_SIZE &&
                c == g_board[i + 1][j] &&
                c == g_board[i + 2][j] &&
                c == g_board[i + 3][j] &&
                c == g_board[i + 4][j]) {
                winStartX = j; winStartY = i;
                winEndX = j; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }

            if (i + 4 < BOARD_SIZE && j + 4 < BOARD_SIZE &&
                c == g_board[i + 1][j + 1] &&
                c == g_board[i + 2][j + 2] &&
                c == g_board[i + 3][j + 3] &&
                c == g_board[i + 4][j + 4]) {
                winStartX = j; winStartY = i;
                winEndX = j + 4; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }
            if (i + 4 < BOARD_SIZE && j >= 4 &&
                c == g_board[i + 1][j - 1] &&
                c == g_board[i + 2][j - 2] &&
                c == g_board[i + 3][j - 3] &&
                c == g_board[i + 4][j - 4]) {
                winStartX = j; winStartY = i;
                winEndX = j - 4; winEndY = i + 4;
                return (c == X) ? X_WIN : O_WIN;
            }
        }
    }
    return isFull() ? DRAW : PLAYING;
}

void saveGame(const std::string& filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) f << g_board[i][j] << " ";
        f << "\n";
    }
    f << g_turn;
    f.close();
}

bool loadGame(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "Khong tim thay file save: " << filename << std::endl;
        return false; 
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int val;
            f >> val;
            g_board[i][j] = val;
        }
    }
    f >> g_turn;
    f.close();
    g_cursorX = BOARD_SIZE / 2;
    g_cursorY = BOARD_SIZE / 2;
    g_status = TestBoard();
    if (g_status != X_WIN && g_status != O_WIN && g_status != DRAW) {
        g_status = PLAYING;
    }
    return true;
}