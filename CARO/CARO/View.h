#ifndef _VIEW_H_
#define _VIEW_H_

#include "Global.h"

// "Thủ thuật" GenImage, tương đương Font.cpp
// Chúng ta sẽ tự "vẽ" texture cho X và O
Texture2D GenXTexture(int size, Color color);
Texture2D GenOTexture(int size, Color color);

// Khởi tạo và hủy các tài nguyên đồ họa (textures)
void InitGameView();
void UnloadGameView();

// Hàm vẽ chính, thay thế tất cả các hàm Draw... cũ
void DrawGameView();

#endif // _VIEW_H_
