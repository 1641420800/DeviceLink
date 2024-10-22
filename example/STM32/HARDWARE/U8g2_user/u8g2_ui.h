#ifndef _U8G2_UI_
#define _U8G2_UI_

#include <stdlib.h>
#include <stdint.h>

#include "u8g2.h"
#include "IIC_OLED.h"

#define u8g2Ui_malloc malloc
#define u8g2Ui_free free
#define u8g2Ui_realloc realloc

#define u8g2Ui_fastMode

#define u8g2Ui_deviceLink_import
#define u8g2Ui_layer_import
#define u8g2Ui_text_import
#define u8g2Ui_paintingFrame_import
#define u8g2Ui_starrySky_import

#define TYPE_CAST(p, Type) ((void *)((p) ? (((u8g2Ui_basic_t *)p)->type) == (Type) ? (p) : NULL : NULL))
typedef enum
{
    Ui_Type_ui_basic,
    Ui_Type_ui,
    Ui_Type_ui_text,
    Ui_Type_ui_paintingFrame,
    Ui_Type_ui_starrySky,
} u8g2Ui_Type_t;

typedef enum
{
    Ui_eType_click,
    Ui_eType_delete,
    Ui_eType_getContentH,
    Ui_eType_getContentW,

    // 公共事件
    Ui_eType_setX,
    Ui_eType_setY,
    Ui_eType_setW,
    Ui_eType_setH,
    Ui_eType_setFont,
    Ui_eType_getX,
    Ui_eType_getY,
    Ui_eType_getW,
    Ui_eType_getH,
    Ui_eType_getFont,

    // 子模块事件
    Ui_eType_starrySky_starsNumChange,
} u8g2Ui_eType_t;

typedef enum
{
    LayerAND,
    LayerOR,
    LayerXOR,
    LayerXNOR,
} Layer_t;

typedef struct
{
    u8g2_long_t x;
    u8g2_long_t y;
    u8g2_uint_t w;
    u8g2_uint_t h;
} u8g2Ui_posSize_t;

typedef struct U8G2Ui_BASIC u8g2Ui_basic_t;

typedef void (*u8g2Ui_init_t)(u8g2Ui_basic_t *p);
typedef void (*u8g2Ui_deInit_t)(u8g2Ui_basic_t *p);
typedef void (*u8g2Ui_display_t)(u8g2Ui_basic_t *p);
typedef uint8_t (*u8g2Ui_event_t)(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue);

struct U8G2Ui_BASIC
{
    struct U8G2Ui_BASIC *p_next;
    struct U8G2Ui_BASIC *p_father;
    struct U8G2Ui_BASIC *p_son;

    u8g2Ui_Type_t type;
    u8g2Ui_posSize_t posSize;
    const uint8_t *font;

    u8g2Ui_init_t init;
    u8g2Ui_deInit_t deInit;
    u8g2Ui_display_t display;
    u8g2Ui_event_t event;
    u8g2Ui_event_t userEvent;
};
/*
void u8g2Ui_init(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t * _p = TYPE_CAST(p, Ui_Type_ui_basic);
    if (!_p)
    {
        // todo
        return;
    }
}
void u8g2Ui_deInit(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t * _p = TYPE_CAST(p, Ui_Type_ui_basic);
    if (!_p)
    {
        // todo
        return;
    }
}
void u8g2Ui_display(u8g2Ui_basic_t *p)
{
    u8g2Ui_text_t * _p = TYPE_CAST(p, Ui_Type_ui_basic);
    if (!_p)
    {
        // todo
        return;
    }
}
uint8_t u8g2Ui_event(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue)
{
    u8g2Ui_text_t * _p = TYPE_CAST(p_receive, Ui_Type_ui_basic);
    if (!_p)
    {
        // todo
        return 0;
    }
    return u8g2Ui_basicEvent(p_receive,p_launch,EType,EValue);
}
*/
/* ----------------------------| u8g2_uiList.c |---------------------------- */

uint8_t u8g2Ui_list_bind(u8g2Ui_basic_t *p_father, u8g2Ui_basic_t *p);
uint8_t u8g2Ui_list_unbind(u8g2Ui_basic_t *p);

/* ----------------------------| u8g2_ui.c |---------------------------- */

typedef struct
{
    u8g2Ui_basic_t basic;
    u8g2_t u8g2;
} u8g2Ui_t;

u8g2Ui_t *new_u8g2Ui(void);
u8g2_t *u8g2Ui_getU8g2(void *p);

/* ----------------------------| u8g2_uiLayer.c |---------------------------- */
#ifdef u8g2Ui_layer_import

#define layer(u8g2, layer, p)    \
    do                           \
    {                            \
        u8g2Ui_startLayer(u8g2); \
        p;                       \
        u8g2Ui_endLayer(layer);  \
    } while (0)
void u8g2Ui_startLayer(u8g2_t *u8g2);
void u8g2Ui_endLayer(Layer_t layer);

#endif
/* ----------------------------| u8g2_uiText.c |---------------------------- */
#ifdef u8g2Ui_text_import

typedef struct
{
    u8g2Ui_basic_t basic;
    char *text;
    uint16_t textLen;
    uint8_t isMultiline;
    uint8_t isVisible;
} u8g2Ui_text_t;

u8g2Ui_text_t *new_u8g2Ui_text(void *p, char *text);
u8g2Ui_text_t *new_u8g2Ui_textBuff(void *p, uint16_t textLen);

char *u8g2Ui_text_get_text(void *p);
void u8g2Ui_text_set_text(void *p, const char *text, ...);
uint8_t u8g2Ui_text_get_isMultiline(void *p);
void u8g2Ui_text_set_isMultiline(void *p, uint8_t isMultiline);
uint8_t u8g2Ui_text_get_isVisible(void *p);
void u8g2Ui_text_set_isVisible(void *p, uint8_t isVisible);
u8g2_uint_t u8g2Ui_text_get_contentH(void *p);
u8g2_uint_t u8g2Ui_text_get_contentW(void *p);

#endif
/* ----------------------------| u8g2_uiPaintingFrame.c |---------------------------- */
#ifdef u8g2Ui_paintingFrame_import

typedef void (*PaintingFrame_cb)(u8g2Ui_basic_t *p, u8g2Ui_posSize_t *posSize);
typedef struct
{
    u8g2Ui_basic_t basic;
    PaintingFrame_cb cb;
} u8g2Ui_paintingFrame_t;

u8g2Ui_paintingFrame_t *new_u8g2Ui_paintingFrame(void *p, PaintingFrame_cb cb);
PaintingFrame_cb u8g2Ui_paintingFrame_get_cb(void *p);
void u8g2Ui_paintingFrame_set_cb(void *p, PaintingFrame_cb cb);

#endif
/* ----------------------------| u8g2_uiStarrySky.c |---------------------------- */
#ifdef u8g2Ui_starrySky_import
typedef struct
{
    float x;
    float y;
    u8g2_int_t siz;

    u8g2_uint_t effective;
} u8g2_stars_t;

typedef struct
{
    u8g2Ui_basic_t basic;
    u8g2_stars_t *stars;
    size_t maximumQuantity;

    float spe;
    u8g2_uint_t dir;
    u8g2_uint_t genProb;
    u8g2_uint_t maxSize;

    u8g2_uint_t starsNum;
} u8g2Ui_starrySky_t;

u8g2Ui_starrySky_t *new_u8g2Ui_starrySky(void *p, size_t maximumQuantity);
size_t u8g2Ui_starrySky_getMaximumQuantity(void *p);
void u8g2Ui_starrySky_setMaximumQuantity(void *p, size_t maximumQuantity);
u8g2_uint_t u8g2Ui_starrySky_getDir(void *p);
void u8g2Ui_starrySky_setDir(void *p, u8g2_uint_t dir);
float u8g2Ui_starrySky_getSpe(void *p);
void u8g2Ui_starrySky_setSpe(void *p, float spe);
u8g2_uint_t u8g2Ui_starrySky_getGenProb(void *p);
void u8g2Ui_starrySky_setGenProb(void *p, u8g2_uint_t genProb);
u8g2_uint_t u8g2Ui_starrySky_getMaxSize(void *p);
void u8g2Ui_starrySky_setMaxSize(void *p, u8g2_uint_t maxSize);
u8g2_uint_t u8g2Ui_starrySky_getStarsNum(void *p);

#endif

/* ----------------------------| u8g2_uiDeviceLink.c |---------------------------- */
#ifdef u8g2Ui_deviceLink_import

void oled_u8g2Ui_init(u8g2Ui_t *p);

#endif

/* ----------------------------| u8g2_uiCore.c |---------------------------- */

void u8g2Ui_basic_init(u8g2Ui_basic_t *basic, u8g2Ui_init_t init, u8g2Ui_deInit_t deInit, u8g2Ui_display_t display, u8g2Ui_event_t event, u8g2Ui_Type_t type);
void u8g2Ui_init(u8g2Ui_t *p);
void u8g2Ui_run(u8g2Ui_t *p);
void u8g2Ui_delete(void *p);
void u8g2Ui_getClipPosSize(u8g2Ui_basic_t *p, u8g2Ui_posSize_t *posSize);
void u8g2Ui_clipWindow(u8g2Ui_basic_t *p);
void u8g2Ui_getPosSize(void *p, u8g2Ui_posSize_t *posSize);
void u8g2Ui_setPosSize(void *p, u8g2Ui_posSize_t *posSize);
u8g2_long_t u8g2Ui_getPosSize_x(void *p);
void u8g2Ui_setPosSize_x(void *p, u8g2_long_t x);
u8g2_long_t u8g2Ui_getPosSize_y(void *p);
void u8g2Ui_setPosSize_y(void *p, u8g2_long_t y);
u8g2_uint_t u8g2Ui_getPosSize_h(void *p);
void u8g2Ui_setPosSize_h(void *p, u8g2_uint_t h);
u8g2_uint_t u8g2Ui_getPosSize_w(void *p);
void u8g2Ui_setPosSize_w(void *p, u8g2_uint_t w);
const uint8_t *u8g2Ui_getFont(void *p);
void u8g2Ui_setFont(void *p, const uint8_t *font);
void u8g2Ui_callEvent(u8g2Ui_basic_t *p, u8g2Ui_eType_t EType, int EValue);
uint8_t u8g2Ui_basicEvent(u8g2Ui_basic_t *p_receive, u8g2Ui_basic_t *p_launch, u8g2Ui_eType_t EType, int EValue);
void u8g2Ui_setUserEvent(void *p, u8g2Ui_event_t userEvent);
u8g2Ui_event_t u8g2Ui_getUserEvent(void *p);

#endif
