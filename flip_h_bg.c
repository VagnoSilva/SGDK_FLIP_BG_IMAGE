#include <genesis.h>
#include "flip_h_bg.h"


/**
 * @brief Carrega os tiles da imagem e desenha espelhada horizontalmente no background.
 *
 * - Carrega automaticamente os tiles da imagem para a VRAM (VDP_loadTileSet);
 * - Reordena a tilemap invertendo a ordem das colunas (flip horizontal);
 * - Aplica o bit HFLIP por tile sem duplicar tiles na VRAM;
 * - Funciona com qualquer largura/altura de imagem exportada pelo SGDK.
 *
 * @param plan           BG_A ou BG_B
 * @param img            Ponteiro para a imagem (Image*)
 * @param baseTileIndex  Índice base na VRAM onde os tiles serão armazenados
 * @param x              Posição X em tiles
 * @param y              Posição Y em tiles
 * @param palette        PAL0–PAL3
 */
void drawImageFlippedH(u16 plan, const Image *img,
                       u16 baseTileIndex, u16 x, u16 y, u16 palette)
{
    if (!img || !img->tileset || !img->tilemap) return;

    // 1.Carrega automaticamente os tiles da imagem para a VRAM
    VDP_loadTileSet(img->tileset, baseTileIndex, DMA);

    // 2.Configurações da tilemap
    TileMap *map    = img->tilemap;
    u16 width       = map->w;       // largura real em tiles
    u16 height      = map->h;       // altura real em tiles
    const u16 *src  = map->tilemap; // buffer contendo os índices relativos

    // 3.Percorre linha a linha e monta a imagem invertida
    for (u16 row = 0; row < height; row++)
    {
        for (u16 col = 0; col < width; col++)
        {
            // Calcula a coluna invertida da tilemap original (espelhamento)
            u16 srcCol = width - 1 - col;

            // Lê o índice relativo do tile (0,1,2,...)
            u16 relIndex = src[row * width + srcCol];

            // Converte para índice real na VRAM
            u16 tileIndex = baseTileIndex + relIndex;

            // Aplica o bit HFLIP (TRUE) e Paleta para o risco visual de espelhamento
            // TILE_ATTR(palette, priority, vflip, hflip)
            u16 attr = TILE_ATTR(palette, 0, FALSE, TRUE) | tileIndex;

            // Escreve o tile no background
            VDP_setTileMapXY(plan, attr, x + col, y + row);
        }
    }
}



