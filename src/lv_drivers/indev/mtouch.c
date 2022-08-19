/**
 * @file mtouch.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "mtouch.h"
#if USE_MTOUCH

#include "lv_custom/lv_custom.h"

#include "input/mtouch_client.h"
#include <libgen.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static struct input_descriptor* _mtouch_dev = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void mtouch_init(void)
{
    mtouch_scaling_params_t scaling_params;
    int reval = read_scaling_conf(DEFAULT_SCALING_CONF, &scaling_params, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    if (reval != 0) {
        lv_custom_print_warn("[mtouch] read_scaling_conf failed!");
    }
    mtouch_client_params_t touch_params;

    touch_params.min_event_interval = LV_INDEV_DEF_READ_PERIOD;
    touch_params.scaling = scaling_params;
    _mtouch_dev = mtouch_client_open(MTOUCH_CLASS_IDX, &touch_params);
    if (_mtouch_dev == NULL) {
        lv_custom_print_warn("[mtouch] mtouch_client_open failed!");
    }
}

bool mtouch_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{
    (void)indev_drv;
    if (!_mtouch_dev) {
        return false;
    }
    mtouch_data_t mtouch_data;
    int reval = mtouch_client_read(_mtouch_dev, &mtouch_data, sizeof(mtouch_data_t), 0, 0);
    if (reval != 0) {
        return false;
    }
    data->point.x = mtouch_data.x;
    data->point.y = mtouch_data.y;
    data->state = mtouch_data.touch_down > 0 ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    return true;
}

void mtouch_exit()
{
    if (_mtouch_dev != NULL) {
        int reval = mtouch_client_close(_mtouch_dev);
        if (reval != 0) {
            lv_custom_print_warn("[mtouch] mtouch_exit failed!");
        }
        _mtouch_dev = NULL;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif
