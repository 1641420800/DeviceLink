#include "u8g2_ui.h"
#ifdef u8g2Ui_text_import

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char *my_strtok(char *text, char c)
{
    static char *p;
    if (text)
        p = text;
    else
    {
        if (!p)
            return NULL;
        while (*p)
            p++;
        *p = c;
        p++;
    }
    char *q = p;
    while (*q && *q != c)
        q++;
    if (*q == '\0')
    {
        q = p;
        p = NULL;
    }
    else
    {
        *q = '\0';
        q = p;
    }
    return q;
}

void u8g2Ui_text_init(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return;
    }
}
void u8g2Ui_text_deInit(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return;
    }
    if (_p->textLen)
    {
        u8g2Ui_free(_p->text);
    }
}
void u8g2Ui_text_display(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p || !_p->isVisible)
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
    u8g2_SetFont(u8g2, p->font);
    posSize.y += u8g2_GetAscent(u8g2);
    if (_p->isMultiline)
    {
        char *token = my_strtok(_p->text, '\n');
        while (token != NULL)
        {
            u8g2_DrawUTF8(u8g2, posSize.x, posSize.y, token);
            posSize.y += u8g2_GetMaxCharHeight(u8g2);
            token = my_strtok(NULL, '\n');
        }
    }
    else
    {
        u8g2_DrawUTF8(u8g2, posSize.x, posSize.y, _p->text);
    }
}
uint8_t u8g2Ui_text_event(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p_receive, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return 0;
    }
    if (EType == Ui_eType_getContentH)
    {
        *(u8g2_uint_t*)EValue = u8g2Ui_text_get_contentH(_p);
        return 1;
    }
    if (EType == Ui_eType_getContentW)
    {
        *(u8g2_uint_t*)EValue = u8g2Ui_text_get_contentW(_p);
        return 1;
    }
    return u8g2Ui_basicEvent(p_receive, p_launch, EType, EValue);
}

u8g2Ui_text_t *new_u8g2Ui_text(void *p, char *text)
{
    if (!p || !text)
    {
        // todo
        return NULL;
    }
    u8g2Ui_text_t *u8g2Ui_text = (u8g2Ui_text_t *)u8g2Ui_malloc(sizeof(u8g2Ui_text_t));
    if (!u8g2Ui_text)
    {
        // todo
        return NULL;
    }

    u8g2Ui_basic_init(
        &u8g2Ui_text->basic,
        u8g2Ui_text_init,
        u8g2Ui_text_deInit,
        u8g2Ui_text_display,
        u8g2Ui_text_event,
        Ui_Type_ui_text);

    u8g2Ui_text->text = text;
    u8g2Ui_text->textLen = 0;
    u8g2Ui_text->isMultiline = 0;
    u8g2Ui_text->isVisible = 1;

    u8g2Ui_list_bind(p, &u8g2Ui_text->basic);

    if (u8g2Ui_text->basic.p_father)
    {
        u8g2Ui_text->basic.font = u8g2Ui_text->basic.p_father->font;
    }

    return u8g2Ui_text;
}
u8g2Ui_text_t *new_u8g2Ui_textBuff(void *p, uint16_t textLen)
{
    if (!p || !textLen)
    {
        // todo
        return NULL;
    }
    char *text = (char *)u8g2Ui_malloc(sizeof(char) * textLen);
    u8g2Ui_text_t *ret = new_u8g2Ui_text(p, text);
    if (!ret)
    {
        // todo
        u8g2Ui_free(text);
        return NULL;
    }
    ret->textLen = textLen;
    return ret;
}
char *u8g2Ui_text_get_text(void *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return NULL;
    }
    return _p->text;
}
void u8g2Ui_text_set_text(void *p, const char *text, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, text);
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return;
    }
    vsprintf(_p->text, text, arg_ptr);
    va_end(arg_ptr);
}
uint8_t u8g2Ui_text_get_isMultiline(void *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return (uint8_t)-1;
    }
    return _p->isMultiline;
}
void u8g2Ui_text_set_isMultiline(void *p, uint8_t isMultiline)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return;
    }
    _p->isMultiline = isMultiline;
}
uint8_t u8g2Ui_text_get_isVisible(void *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return (uint8_t)-1;
    }
    return _p->isVisible;
}
void u8g2Ui_text_set_isVisible(void *p, uint8_t isVisible)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return;
    }
    _p->isVisible = isVisible;
}
u8g2_uint_t u8g2Ui_text_get_contentW(void *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return 0;
    }
    u8g2_t *u8g2 = u8g2Ui_getU8g2(p);
    if (!u8g2)
    {
        // todo
        return 0;
    }
    u8g2_uint_t ret = 0;
    if (_p->isMultiline)
    {
        char *token = my_strtok(_p->text, '\n');
        while (token != NULL)
        {
            u8g2_uint_t w = u8g2_GetUTF8Width(u8g2, token);
            if(ret < w) ret = w;
        }
    }
    else
    {
        ret = u8g2_GetUTF8Width(u8g2, _p->text);
    }
    return ret;
}
u8g2_uint_t u8g2Ui_text_get_contentH(void *p)
{
    u8g2Ui_text_t *_p = TYPE_CAST(p, Ui_Type_ui_text);
    if (!_p)
    {
        // todo
        return 0;
    }
    u8g2_t *u8g2 = u8g2Ui_getU8g2(p);
    if (!u8g2)
    {
        // todo
        return 0;
    }
    u8g2_uint_t h = u8g2_GetMaxCharHeight(u8g2);
    u8g2_uint_t ret = h;
    if (_p->isMultiline)
    {
        while(_p)
        {
            if(*_p->text == '\n') ret += h;
        }
    }
    return ret;
}
#endif
