/**
 * @file qnxscreen.h
 *
 */

#ifndef MTOUCH_H
#define MTOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_MTOUCH

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void mtouch_init(void);

bool mtouch_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);

void mtouch_exit(void);

/**********************
 *      MACROS
 **********************/

#endif /* USE_MTOUCH */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MTOUCH_H */
