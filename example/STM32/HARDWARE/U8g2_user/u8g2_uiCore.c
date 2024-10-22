#include "u8g2_ui.h"

void u8g2Ui_basic_init(u8g2Ui_basic_t *basic, u8g2Ui_init_t init, u8g2Ui_deInit_t deInit, u8g2Ui_display_t display, u8g2Ui_event_t event, u8g2Ui_Type_t type)
{

    basic->init = init;
    basic->deInit = deInit;
    basic->display = display;
    basic->event = event;
    basic->userEvent = NULL;
    basic->type = type;
    basic->p_father = NULL;
    basic->p_next = NULL;
    basic->p_son = NULL;
    basic->font = u8g2_font_8x13_mf;

    basic->posSize.x = 0;
    basic->posSize.y = 0;
    basic->posSize.w = (u8g2_uint_t) ~(u8g2_uint_t)0;
    basic->posSize.h = (u8g2_uint_t) ~(u8g2_uint_t)0;
}

void u8g2_ui_run_display(u8g2Ui_basic_t *p)
{
    while (p)
    {
        if (p->display)
            p->display(p);
        if (p->p_son)
            u8g2_ui_run_display(p->p_son);
        p = p->p_next;
    }
}

void u8g2_ui_init_call(u8g2Ui_basic_t *p)
{
    while (p)
    {
        if (p->init)
            p->init(p);
        if (p->p_son)
            u8g2_ui_init_call(p->p_son);
        p = p->p_next;
    }
}

void u8g2Ui_run(u8g2Ui_t *p)
{
#ifdef u8g2Ui_fastMode
    u8g2_ui_run_display(&p->basic);
    if (!u8g2_NextPage(&p->u8g2))
    {
        u8g2_FirstPage(&p->u8g2);
    }
#else
    u8g2_ClearBuffer(&p->u8g2);
    u8g2_ui_run_display(&p->basic);
    u8g2_SendBuffer(&p->u8g2);
#endif
}

void u8g2Ui_init(u8g2Ui_t *p)
{
    u8g2_ui_init_call(&p->basic);
}

void u8g2Ui_delete(void *p)
{
    u8g2Ui_basic_t *u8g2Ui_basic = (u8g2Ui_basic_t *)p;
    if (!u8g2Ui_basic)
        return;
    if (u8g2Ui_basic->p_father)
    {
        if (u8g2Ui_list_unbind(p))
            return;
    }
    u8g2Ui_basic_t *u8g2Ui_basic_temp;
    u8g2Ui_basic_t **u8g2Ui_basic_temp2;
    while (u8g2Ui_basic->p_son)
    {
        u8g2Ui_basic_temp = u8g2Ui_basic->p_son;
        while (u8g2Ui_basic_temp->p_son || u8g2Ui_basic_temp->p_next)
        {
            if (u8g2Ui_basic_temp->p_son)
            {
                u8g2Ui_basic_temp = u8g2Ui_basic_temp->p_son;
                u8g2Ui_basic_temp2 = &u8g2Ui_basic_temp->p_son;
            }
            else
            {
                u8g2Ui_basic_temp = u8g2Ui_basic_temp->p_next;
                u8g2Ui_basic_temp2 = &u8g2Ui_basic_temp->p_next;
            }
        }
        *u8g2Ui_basic_temp2 = NULL;
        u8g2Ui_basic_temp->event(u8g2Ui_basic_temp, u8g2Ui_basic, Ui_eType_delete, 0);
        u8g2Ui_basic_temp->deInit(u8g2Ui_basic_temp);
        u8g2Ui_free(u8g2Ui_basic_temp);
    }
    u8g2Ui_basic->event(u8g2Ui_basic, u8g2Ui_basic, Ui_eType_delete, 0);
    u8g2Ui_basic->deInit(u8g2Ui_basic);
    u8g2Ui_free(p);
}

void u8g2Ui_getClipPosSize(u8g2Ui_basic_t *p, u8g2Ui_posSize_t *posSize)
{
    if (!p)
        return;
    u8g2Ui_posSize_t _posSize = p->posSize;
    p = p->p_father;
    while (p)
    {
        _posSize.x += p->posSize.x;
        _posSize.y += p->posSize.y;
        p = p->p_father;
    }
    *posSize = _posSize;
}
void u8g2Ui_clipWindow(u8g2Ui_basic_t *p)
{
    if (!p)
        return;
    u8g2_t *u8g2 = u8g2Ui_getU8g2(p);
    if (!u8g2)
        return;
    u8g2Ui_posSize_t _posSize = p->posSize;
    u8g2_long_t x_max = _posSize.x > 0 ? _posSize.x : 0;
    u8g2_long_t y_max = _posSize.y > 0 ? _posSize.y : 0;
    p = p->p_father;
    while (p)
    {
        _posSize.x += p->posSize.x;
        _posSize.y += p->posSize.y;
        x_max += p->posSize.x;
        y_max += p->posSize.x;
        if(x_max < 0) x_max = 0;
        if(y_max < 0) y_max = 0;
        u8g2_long_t x0 = _posSize.x + _posSize.w;
        u8g2_long_t y0 = _posSize.y + _posSize.h;
        u8g2_long_t x1 = p->posSize.x + p->posSize.w;
        u8g2_long_t y1 = p->posSize.y + p->posSize.h;
        _posSize.w = (x0 < x1 ? x0 : x1) - _posSize.x;
        _posSize.h = (y0 < y1 ? y0 : y1) - _posSize.y;
        p = p->p_father;
    }
    _posSize.x = x_max;
    _posSize.y = y_max;
    u8g2_SetClipWindow(u8g2, _posSize.x, _posSize.y, _posSize.x + _posSize.w, _posSize.y + _posSize.h);
}

void u8g2Ui_getPosSize(void *p, u8g2Ui_posSize_t *posSize)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p || !posSize)
    {
        return;
    }
    *posSize = _p->posSize;
}
void u8g2Ui_setPosSize(void *p, u8g2Ui_posSize_t *posSize)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p || !posSize)
    {
        return;
    }
    _p->posSize = *posSize;
}
u8g2_long_t u8g2Ui_getPosSize_x(void *p)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    return posSize.x;
}
void u8g2Ui_setPosSize_x(void *p, u8g2_long_t x)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    posSize.x = x;
    u8g2Ui_setPosSize(p, &posSize);
}
u8g2_long_t u8g2Ui_getPosSize_y(void *p)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    return posSize.y;
}
void u8g2Ui_setPosSize_y(void *p, u8g2_long_t y)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    posSize.y = y;
    u8g2Ui_setPosSize(p, &posSize);
}
u8g2_uint_t u8g2Ui_getPosSize_h(void *p)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    return posSize.h;
}
void u8g2Ui_setPosSize_h(void *p, u8g2_uint_t h)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    posSize.h = h;
    u8g2Ui_setPosSize(p, &posSize);
}
u8g2_uint_t u8g2Ui_getPosSize_w(void *p)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    return posSize.w;
}
void u8g2Ui_setPosSize_w(void *p, u8g2_uint_t w)
{
    u8g2Ui_posSize_t posSize = {0};
    u8g2Ui_getPosSize(p, &posSize);
    posSize.w = w;
    u8g2Ui_setPosSize(p, &posSize);
}
const uint8_t *u8g2Ui_getFont(void *p)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p)
    {
        return NULL;
    }
    return _p->font;
}
void u8g2Ui_setFont(void *p, const uint8_t *font)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p || !font)
    {
        return;
    }
    _p->font = font;
}
void u8g2Ui_callEvent(u8g2Ui_basic_t *p, u8g2Ui_eType_t EType, int EValue)
{
    u8g2Ui_basic_t * p_launch = p;
    while (p)
    {
        if(p->userEvent && p->userEvent(p,p_launch,EType,EValue))
        {
            return;
        }
        if(p->event && p->event(p,p_launch,EType,EValue))
        {
            return;
        }
        p = p->p_father;
    }
}
uint8_t u8g2Ui_basicEvent(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue)
{
    if(!p_receive)
        return 0;
    if(EType == Ui_eType_setX)
    {
        u8g2Ui_setPosSize_x(p_receive,EValue);
        return 1;
    }
    if(EType == Ui_eType_setY)
    {
        u8g2Ui_setPosSize_y(p_receive,EValue);
        return 1;
    }
    if(EType == Ui_eType_setW)
    {
        u8g2Ui_setPosSize_w(p_receive,EValue);
        return 1;
    }
    if(EType == Ui_eType_setH)
    {
        u8g2Ui_setPosSize_h(p_receive,EValue);
        return 1;
    }
    if(EType == Ui_eType_setFont)
    {
        u8g2Ui_setFont(p_receive,(const uint8_t *)EValue);
        return 1;
    }
    if(EType == Ui_eType_getX)
    {
        *(u8g2_long_t*)EValue = u8g2Ui_getPosSize_x(p_receive);
        return 1;
    }
    if(EType == Ui_eType_getY)
    {
        *(u8g2_long_t*)EValue = u8g2Ui_getPosSize_y(p_receive);
        return 1;
    }
    if(EType == Ui_eType_getW)
    {
        *(u8g2_uint_t*)EValue = u8g2Ui_getPosSize_w(p_receive);
        return 1;
    }
    if(EType == Ui_eType_getH)
    {
        *(u8g2_uint_t*)EValue = u8g2Ui_getPosSize_h(p_receive);
        return 1;
    }
    if(EType == Ui_eType_getFont)
    {
        *(const uint8_t **)EValue = u8g2Ui_getFont(p_receive);
        return 1;
    }
    return 0;
}
void u8g2Ui_setUserEvent(void *p, u8g2Ui_event_t userEvent)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p)
    {
        return;
    }
    _p->userEvent = userEvent;
}
u8g2Ui_event_t u8g2Ui_getUserEvent(void *p)
{
    u8g2Ui_basic_t *_p = p;
    if (!_p)
    {
        return NULL;
    }
    return _p->userEvent;
}
