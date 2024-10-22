#include "u8g2_ui.h"
#ifdef u8g2Ui_layer_import

#include <stdlib.h>

uint8_t *layerBuff = NULL;
u8g2_t *layerU8g2 = NULL;
void u8g2Ui_startLayer(u8g2_t *u8g2)
{
    if (!u8g2)
        return;
    uint8_t *_layerBuff = (uint8_t *)u8g2Ui_malloc(u8g2->tile_buf_height * 128);
    if (!_layerBuff)
    {
        u8g2 = NULL;
        return;
    }
    layerBuff = u8g2_GetBufferPtr(u8g2);
    u8g2_GetBufferPtr(u8g2) = _layerBuff;
    u8g2_ClearBuffer(u8g2);
    layerU8g2 = u8g2;
}

void u8g2Ui_endLayer(Layer_t layer)
{
    if(!layerU8g2)
        return;
    uint8_t *_layerBuff = u8g2_GetBufferPtr(layerU8g2);

    switch (layer)
    {
    case LayerAND:
        for (int i = 0; i < layerU8g2->tile_buf_height * 128; ++i)
        {
            layerBuff[i] &= _layerBuff[i];
        }
        break;
    case LayerOR:
        for (int i = 0; i < layerU8g2->tile_buf_height * 128; ++i)
        {
            layerBuff[i] |= _layerBuff[i];
        }
        break;
    case LayerXOR:
        for (int i = 0; i < layerU8g2->tile_buf_height * 128; ++i)
        {
            layerBuff[i] ^= _layerBuff[i];
        }
        break;
    case LayerXNOR:
        for (int i = 0; i < layerU8g2->tile_buf_height * 128; ++i)
        {
            layerBuff[i] = (layerBuff[i] & _layerBuff[i]) | (~layerBuff[i] & ~_layerBuff[i]);
        }
        break;
    }
    u8g2_GetBufferPtr(layerU8g2) = layerBuff;
    u8g2Ui_free(_layerBuff);
    layerBuff = NULL;
    layerU8g2 = NULL;
}

#endif
