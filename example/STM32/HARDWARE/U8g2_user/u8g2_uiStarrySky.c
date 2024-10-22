#include "u8g2_ui.h"
#ifdef u8g2Ui_starrySky_import
#include <stdlib.h>

void u8g2Ui_starrySky_init(u8g2Ui_basic_t *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    u8g2_stars_t *stars = (u8g2_stars_t *)u8g2Ui_malloc(sizeof(u8g2_stars_t) * _p->maximumQuantity);
    if (!stars)
    {
        // todo
        _p->stars = NULL;
        return;
    }
    srand(_p->maximumQuantity);
    _p->stars = stars;
}
void u8g2Ui_starrySky_deInit(u8g2Ui_basic_t *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    if (_p->stars)
    {
        u8g2Ui_free(_p->stars);
    }
    _p->stars = NULL;
}
void u8g2Ui_starrySky_display(u8g2Ui_basic_t *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p || !_p->stars)
    {
        // todo
        return;
    }
    u8g2_t *u8g2 = u8g2Ui_getU8g2(p);
    if (!u8g2)
    {
        // todo
        return;
    }
    u8g2Ui_posSize_t posSize;
    u8g2Ui_getClipPosSize(p, &posSize);
    u8g2Ui_clipWindow(p);
    u8g2_SetFont(u8g2, u8g2_font_8x13_mf);
    posSize.y += u8g2_GetAscent(u8g2);

#ifdef u8g2Ui_fastMode
    if (u8g2->tile_curr_row == 0)
    {
#endif
        for (uint16_t i = 0; i < _p->maximumQuantity; i++)
        {
            if (!_p->stars[i].effective)
            {
                if (rand() % 100 < _p->genProb)
                {
                    _p->stars[i].effective = 1;
                    _p->stars[i].siz = rand() % (_p->maxSize + 1);
                    switch(_p->dir)
                    {
                        case 0:
                            _p->stars[i].y = rand() % posSize.h;
                            _p->stars[i].x = -_p->stars[i].siz;
                            break;
                        case 1:
                            _p->stars[i].y = -_p->stars[i].siz;
                            _p->stars[i].x = rand() % posSize.w;
                            break;
                        case 2:
                            _p->stars[i].y = rand() % posSize.h;
                            _p->stars[i].x = posSize.w + _p->stars[i].siz;
                            break;
                        case 3:
                            _p->stars[i].y = posSize.h + _p->stars[i].siz;
                            _p->stars[i].x = rand() % posSize.w;
                            break;
                    }
                }
            }
            else
            {
                switch(_p->dir)
                {
                    case 0:
                        _p->stars[i].x += (_p->stars[i].siz + 1) * _p->spe;
                        if (_p->stars[i].x > posSize.w + _p->stars[i].siz)
                        {
                            _p->stars[i].effective = 0;
                        }
                        break;
                    case 1:
                        _p->stars[i].y += (_p->stars[i].siz + 1) * _p->spe;
                        if (_p->stars[i].y > posSize.h + _p->stars[i].siz)
                        {
                            _p->stars[i].effective = 0;
                        }
                        break;
                    case 2:
                        _p->stars[i].x -= (_p->stars[i].siz + 1) * _p->spe;
                        if (_p->stars[i].x < -_p->stars[i].siz)
                        {
                            _p->stars[i].effective = 0;
                        }
                        break;
                    case 3:
                        _p->stars[i].y -= (_p->stars[i].siz + 1) * _p->spe;
                        if (_p->stars[i].y < -_p->stars[i].siz)
                        {
                            _p->stars[i].effective = 0;
                        }
                        break;
                }
            }
        }
#ifdef u8g2Ui_fastMode
    }
#endif
    u8g2_uint_t starsNum = 0;
    for (uint16_t i = 0; i < _p->maximumQuantity; i++)
    {
        if (_p->stars[i].effective)
        {
            starsNum++;
            u8g2_DrawDisc(u8g2, posSize.x + _p->stars[i].x, posSize.y + _p->stars[i].y, _p->stars[i].siz + 1, U8G2_DRAW_ALL);
        }
    }
    
    if(_p->starsNum != starsNum)
    {
        _p->starsNum = starsNum;
        u8g2Ui_callEvent(p,Ui_eType_starrySky_starsNumChange,starsNum);
    }
}
uint8_t u8g2Ui_starrySky_event(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p_receive, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return 0;
}

u8g2Ui_starrySky_t *new_u8g2Ui_starrySky(void *p, size_t maximumQuantity)
{
    if (!p || !maximumQuantity)
    {
        // todo
        return NULL;
    }
    u8g2Ui_starrySky_t *u8g2Ui_starrySky = (u8g2Ui_starrySky_t *)u8g2Ui_malloc(sizeof(u8g2Ui_starrySky_t));
    if (!u8g2Ui_starrySky)
    {
        // todo
        return NULL;
    }

    u8g2Ui_basic_init(
        &u8g2Ui_starrySky->basic,
        u8g2Ui_starrySky_init,
        u8g2Ui_starrySky_deInit,
        u8g2Ui_starrySky_display,
        u8g2Ui_starrySky_event,
        Ui_Type_ui_starrySky);

    u8g2Ui_starrySky->basic.posSize.h = 64;
    u8g2Ui_starrySky->basic.posSize.w = 128;

    u8g2Ui_starrySky->stars = NULL;
    u8g2Ui_starrySky->maximumQuantity = maximumQuantity;
    
    u8g2Ui_starrySky->dir = 0;
    u8g2Ui_starrySky->spe = 1;
    u8g2Ui_starrySky->genProb = 20;
    u8g2Ui_starrySky->maxSize = 3;
    u8g2Ui_starrySky->starsNum = 0;

    u8g2Ui_list_bind(p, &u8g2Ui_starrySky->basic);

    return u8g2Ui_starrySky;
}
size_t u8g2Ui_starrySky_getMaximumQuantity(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->maximumQuantity;
}
void u8g2Ui_starrySky_setMaximumQuantity(void *p, size_t maximumQuantity)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    _p->maximumQuantity = maximumQuantity;
    _p->stars = (u8g2_stars_t *)u8g2Ui_realloc(_p->stars, sizeof(u8g2_stars_t) * _p->maximumQuantity);
}
u8g2_uint_t u8g2Ui_starrySky_getDir(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->dir;
}
void u8g2Ui_starrySky_setDir(void *p, u8g2_uint_t dir)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    _p->dir = dir;
}
float u8g2Ui_starrySky_getSpe(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->spe;
}
void u8g2Ui_starrySky_setSpe(void *p, float spe)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    _p->spe = spe;
}
u8g2_uint_t u8g2Ui_starrySky_getGenProb(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->genProb;
}
void u8g2Ui_starrySky_setGenProb(void *p, u8g2_uint_t genProb)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    _p->genProb = genProb;
}
u8g2_uint_t u8g2Ui_starrySky_getMaxSize(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->maxSize;
}
void u8g2Ui_starrySky_setMaxSize(void *p, u8g2_uint_t maxSize)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return;
    }
    _p->maxSize = maxSize;
}
u8g2_uint_t u8g2Ui_starrySky_getStarsNum(void *p)
{
    u8g2Ui_starrySky_t *_p = TYPE_CAST(p, Ui_Type_ui_starrySky);
    if (!_p)
    {
        // todo
        return 0;
    }
    return _p->starsNum;
}
#endif
