#include "settings.h"
#include "raylib.h"


AppSettings g_settings;      
static int settingCursor = 0; 
const int SETTING_COUNT = 3;  

static Texture2D texSettingBg; 


void InitSetting() {
    g_settings.inputMode = INPUT_KEYBOARD;
    g_settings.musicVolume = 50;
    g_settings.soundEnable = true;
    settingCursor = 0;
    texSettingBg = LoadTexture("assects/images/setting_bg.png");
    if (texSettingBg.id == 0) {
        texSettingBg = LoadTexture("assects/images/bg_about.png");
    }
}


void UpdateSetting(GameScreen& currentScreen) {
// Di chuyển lên xuống
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        settingCursor--;
        if (settingCursor < 0) settingCursor = SETTING_COUNT - 1;
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        settingCursor++;
        if (settingCursor >= SETTING_COUNT) settingCursor = 0;
    }
    // Di chuyển bằng bàn phím hoặc chuột
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        if (settingCursor == 0) {
            if (g_settings.inputMode == INPUT_KEYBOARD)
                g_settings.inputMode = INPUT_MOUSE;
            else
                g_settings.inputMode = INPUT_KEYBOARD;
        }
// Chưa add nhạc
        else if (settingCursor == 1) {
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
                g_settings.musicVolume -= 10;
            else
                g_settings.musicVolume += 10;


            if (g_settings.musicVolume < 0) g_settings.musicVolume = 0;
            if (g_settings.musicVolume > 100) g_settings.musicVolume = 100;
        }
 
        else if (settingCursor == 2) {
            g_settings.soundEnable = !g_settings.soundEnable;
        }
    }


    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}


void DrawSetting() {
    ClearBackground(WHITE);


    Rectangle source = { 0.0f, 0.0f, (float)texSettingBg.width, (float)texSettingBg.height };
    Rectangle dest = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    DrawTexturePro(texSettingBg, source, dest, Vector2{ 0, 0 }, 0.0f, WHITE);


    float panelWidth = 1000;
    float panelHeight = 600;
    float panelX = (SCREEN_WIDTH - panelWidth) / 2;
    float panelY = (SCREEN_HEIGHT - panelHeight) / 2;


    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(CLITERAL(Color) { 10, 20, 40, 255 }, 0.9f));


    DrawRectangleLinesEx(Rectangle{ panelX, panelY, panelWidth, panelHeight }, 4.0f, SKYBLUE);

 
    const char* title = "SETTINGS";
    int titleSize = 80; 
    int titleWidth = MeasureText(title, titleSize);


    DrawText(title, (SCREEN_WIDTH - titleWidth) / 2 + 4, panelY + 54, titleSize, BLACK);
    DrawText(title, (SCREEN_WIDTH - titleWidth) / 2, panelY + 50, titleSize, SKYBLUE);


    int contentStartY = panelY + 220; 
    int lineSpacing = 100;           
    int fontSize = 45;                
    int centerAxis = SCREEN_WIDTH / 2; 
    int padding = 40;                 

    Color labelColor0 = (settingCursor == 0) ? GOLD : LIGHTGRAY;
    Color valueColor0 = (settingCursor == 0) ? WHITE : GRAY;
    const char* label1 = "CONTROL MODE";
    const char* value1 = (g_settings.inputMode == INPUT_KEYBOARD) ? "< KEYBOARD >" : "< MOUSE >";

 
    DrawText(label1, centerAxis - MeasureText(label1, fontSize) - padding, contentStartY, fontSize, labelColor0);
    DrawText(value1, centerAxis + padding, contentStartY, fontSize, valueColor0);


    Color labelColor1 = (settingCursor == 1) ? GOLD : LIGHTGRAY;
    Color valueColor1 = (settingCursor == 1) ? WHITE : GRAY;
    const char* label2 = "MUSIC VOLUME";
    const char* value2 = TextFormat("< %d%% >", g_settings.musicVolume); 

    DrawText(label2, centerAxis - MeasureText(label2, fontSize) - padding, contentStartY + lineSpacing, fontSize, labelColor1);
    DrawText(value2, centerAxis + padding, contentStartY + lineSpacing, fontSize, valueColor1);
    Color labelColor2 = (settingCursor == 2) ? GOLD : LIGHTGRAY;
    Color valueColor2 = (settingCursor == 2) ? WHITE : GRAY;
    const char* label3 = "SOUND FX";
    const char* value3 = g_settings.soundEnable ? "< ON >" : "< OFF >";

    DrawText(label3, centerAxis - MeasureText(label3, fontSize) - padding, contentStartY + lineSpacing * 2, fontSize, labelColor2);
    DrawText(value3, centerAxis + padding, contentStartY + lineSpacing * 2, fontSize, valueColor2);
    const char* guide = "Use [W/S] to Move  -  [A/D] to Change  -  [ESC] to Back";
    int guideSize = 25;
    int guideWidth = MeasureText(guide, guideSize);
    DrawText(guide, (SCREEN_WIDTH - guideWidth) / 2, panelY + panelHeight - 60, guideSize, LIGHTGRAY);
}
//Dọn dep tài nguyên
void UnloadSetting() {
    UnloadTexture(texSettingBg);
}