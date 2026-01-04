// flip_h_bg.h
#ifndef FLIP_H_BG_H
#define FLIP_H_BG_H

#include <genesis.h>
#include "gfx.h"  // Necessário para Image e TileMap, renomeie caso o do seu projeto seja diferente 

// Desenha uma imagem espelhada horizontalmente (reordena colunas + atributos)
void drawImageFlippedH(u16 plan, const Image *img, u16 baseTileIndex, u16 x, u16 y, u16 palette, u16 priority);

#endif // FLIP_H_BG_H

