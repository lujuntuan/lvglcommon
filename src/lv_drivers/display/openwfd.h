/**
 * @file openwfd.h
 *
 */

#ifndef OPENWFD_H
#define OPENWFD_H

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

#if USE_OPENWFD

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
void openwfd_init(void);
void openwfd_exit(void);
void openwfd_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);

/**********************
 *      MACROS
 **********************/

#endif /* USE_OPENWFD */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OPENWFD_H */
