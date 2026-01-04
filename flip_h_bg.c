#include <genesis.h>
#include "flip_h_bg.h"


/**
 * @brief Desenha uma imagem no plano de fundo (BG_A ou BG_B) com espelhamento horizontal (flip H).
 *
 * Esta função carrega os tiles da imagem na VRAM e depois os desenha invertidos horizontalmente,
 * sem modificar os dados originais da imagem na ROM. Ideal para reutilizar assets sem duplicar
 * tiles na VRAM.
 *
 * @param plan           Plano de fundo de destino (ex: BG_A, BG_B).
 * @param img            Ponteiro para a imagem a ser desenhada (não pode ser NULL).
 * @param baseTileIndex  Índice base na VRAM onde os tiles da imagem serão carregados.
 * @param x              Coordenada X (em tiles) do canto superior esquerdo no plano de fundo.
 * @param y              Coordenada Y (em tiles) do canto superior esquerdo no plano de fundo.
 * @param palette        Número da paleta (PAL0 a PAL3) a ser usada para os tiles.
 * @param priority       Prioridade do plano (0 = normal, 1 = acima dos sprites).
 *
 * @note Esta função funciona apenas com imagens baseadas em TileMap (como as geradas pelo rescomp).
 * @note O flip é feito lendo os tiles da imagem da direita para a esquerda, linha por linha.
 * @note Não há suporte para rolagem de tela automática — as coordenadas (x, y) devem estar dentro dos limites do plano.
 */
void drawImageFlippedH(u16 plan, const Image *img,
                       u16 baseTileIndex, u16 x, u16 y,
                       u16 palette, u16 priority)
{
    // Validação de entrada: evita crash se img ou seus componentes forem NULL
    if (!img || !img->tileset || !img->tilemap) return;

    // 1. Carrega os tiles da imagem na VRAM, começando no índice base fornecido
    //    Usa DMA para transferência eficiente (recomendado durante VBlank ou em inicialização)
    VDP_loadTileSet(img->tileset, baseTileIndex, DMA);

    // Acessa o tilemap da imagem (matriz de índices de tiles)
    TileMap *map    = img->tilemap;
    u16 width       = map->w;      // Largura da imagem em tiles
    u16 height      = map->h;      // Altura da imagem em tiles
    const u16 *src  = map->tilemap; // Ponteiro para o array linear de índices de tiles

    // 2. Desenha a imagem linha por linha, invertendo horizontalmente
    for (u16 row = 0; row < height; row++)
    {
        for (u16 col = 0; col < width; col++)
        {
            // Calcula a coluna de origem espelhada: última coluna - coluna atual
            u16 srcCol = width - 1 - col;

            // Obtém o índice do tile na imagem original (não modificado)
            u16 relIndex = src[row * width + srcCol];

            // Calcula o índice absoluto na VRAM (base + offset do tile)
            u16 tileIndex = baseTileIndex + relIndex;

            // Monta o atributo do tile com: paleta, prioridade, sem flip vertical, COM flip horizontal
            // TILE_ATTR(palette, priority, flipV, flipH)
            u16 attr = TILE_ATTR(palette, priority, FALSE, TRUE) | tileIndex;

            // Escreve o tile no plano de fundo, na posição (x + col, y + row)
            VDP_setTileMapXY(plan, attr, x + col, y + row);
        }
    }
}





