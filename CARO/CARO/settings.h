#ifndef _SETTING_H_
#define _SETTING_H_

#include "Global.h"


struct AppSettings {
    InputMode inputMode;    // Chế độ điều khiển
    int musicVolume;       // Chưa add
    bool soundEnable;       // Chưa add
};

extern AppSettings g_settings;
void InitSetting();
void UpdateSetting(GameScreen& currentScreen);
void DrawSetting();
void UnloadSetting();

#endif 
