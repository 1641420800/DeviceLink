#include "u8g2_ui.h"
#ifdef u8g2Ui_paintingFrame_import

void u8g2Ui_paintingFrame_display(u8g2Ui_basic_t *p)
{
    u8g2Ui_paintingFrame_t * _p = TYPE_CAST(p, Ui_Type_ui_paintingFrame);
    if (!_p || !_p->cb)
    {
        // todo
        return;
    }
    u8g2_t * u8g2 = u8g2Ui_getU8g2(p);
    if(!u8g2)
    {
    // todo
    return;
    }
    u8g2Ui_posSize_t posSize;
    u8g2Ui_getClipPosSize(p, &posSize);
    u8g2Ui_clipWindow(p);
    u8g2_SetFont(u8g2,u8g2_font_8x13_mf);
    posSize.y += u8g2_GetAscent(u8g2);
    
    _p->cb(p, &posSize);
}
uint8_t u8g2Ui_paintingFrame_event(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue)
{
    u8g2Ui_paintingFrame_t * _p = TYPE_CAST(p_receive, Ui_Type_ui_paintingFrame);
    if (!_p)
    {
        // todo
        return 0;
    }
    return 0;
}
u8g2Ui_paintingFrame_t *new_u8g2Ui_paintingFrame(void *p, PaintingFrame_cb cb)
{
    if(!p || !cb)
    {
        // todo
        return NULL;
    }
    u8g2Ui_paintingFrame_t *u8g2Ui_paintingFrame = (u8g2Ui_paintingFrame_t *)u8g2Ui_malloc(sizeof(u8g2Ui_paintingFrame_t));
    if (!u8g2Ui_paintingFrame)
    {
        // todo
        return NULL;
    }
    u8g2Ui_basic_init(
        &u8g2Ui_paintingFrame->basic,
        NULL,
        NULL,
        u8g2Ui_paintingFrame_display,
        u8g2Ui_paintingFrame_event,
        Ui_Type_ui_paintingFrame);

    u8g2Ui_paintingFrame->cb = cb;

    u8g2Ui_list_bind(p, &u8g2Ui_paintingFrame->basic);
		
    return u8g2Ui_paintingFrame;
}
PaintingFrame_cb u8g2Ui_paintingFrame_get_cb(void *p)
{
    u8g2Ui_paintingFrame_t * _p = TYPE_CAST(p, Ui_Type_ui_paintingFrame);
    if (!_p)
    {
        // todo
        return NULL;
    }
    return _p->cb;
}
void u8g2Ui_paintingFrame_set_cb(void *p, PaintingFrame_cb cb)
{
    u8g2Ui_paintingFrame_t * _p = TYPE_CAST(p, Ui_Type_ui_paintingFrame);
    if (!_p)
    {
        // todo
        return;
    }
    _p->cb = cb;
}

#endif
