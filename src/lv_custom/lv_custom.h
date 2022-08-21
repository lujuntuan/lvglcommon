/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/02/27
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#ifndef LV_CUSTOM_H
#define LV_CUSTOM_H

#include "lvgl/lvgl.h"
#include <stdint.h>
#include <stdlib.h>

/***************************
 * FONTS
 ***************************/

LV_FONT_DECLARE(font_opposans_14)
LV_FONT_DECLARE(font_opposans_16)
LV_FONT_DECLARE(font_opposans_18)
LV_FONT_DECLARE(font_opposans_20)
LV_FONT_DECLARE(font_opposans_22)
LV_FONT_DECLARE(font_opposans_24)
LV_FONT_DECLARE(font_opposans_28)
LV_FONT_DECLARE(font_opposans_32)
LV_FONT_DECLARE(font_opposans_36)
LV_FONT_DECLARE(font_opposans_42)
LV_FONT_DECLARE(font_opposans_48)
LV_FONT_DECLARE(font_opposans_64)
#define lv_custom_font_14 font_opposans_14
#define lv_custom_font_16 font_opposans_16
#define lv_custom_font_18 font_opposans_18
#define lv_custom_font_20 font_opposans_20
#define lv_custom_font_22 font_opposans_22
#define lv_custom_font_24 font_opposans_24
#define lv_custom_font_28 font_opposans_28
#define lv_custom_font_32 font_opposans_32
#define lv_custom_font_36 font_opposans_36
#define lv_custom_font_42 font_opposans_42
#define lv_custom_font_48 font_opposans_48
#define lv_custom_font_64 font_opposans_64

/***************************
 * TYPEDEFS
 ***************************/

typedef enum {
    LV_CUSTOM_DISP = (1 << 1),
    LV_CUSTOM_INDEV = (1 << 2),
    LV_CUSTOM_CURSOR = (1 << 3),
} lv_custom_flags_t;

/***************************
 * FUNCTIONS
 ***************************/

uint32_t lv_custom_tick_get(void);
void lv_custom_init(lv_custom_flags_t flags);
void lv_custom_msleep(uint32_t ms);
void lv_custom_print(const char* s, ...);
void lv_custom_print_warn(const char* s, ...);
void lv_custom_exec(void);
void lv_custom_exit(int flag);

#endif // LV_CUSTOM_H
