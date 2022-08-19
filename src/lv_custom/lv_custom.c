#include "lv_custom.h"
#include "lv_drivers/display/drm.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/display/openwfd.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/mtouch.h"
#include "lv_drivers/win32drv/win32drv.h"
#include "lv_drivers/win_drv.h"
#include <signal.h>
#include <stdio.h>
#include <time.h>

#define CUSTOM_TASK_INTEVAL 5

LV_IMG_DECLARE(mouse_cursor_icon)
#define lv_custom_cursor mouse_cursor_icon

#ifdef WIN32
#include <Windows.h>
#include <sys/timeb.h>
#define SYS_MSLEEP(MS) Sleep(MS)
static uint32_t get_systime()
{
    static struct __timeb64 now;
    _ftime64_s(&now);
    return ((uint32_t)now.time * 1000U) + (uint32_t)now.millitm;
}
#elif defined(__unix)
#include <sys/time.h>
#include <unistd.h>
#define SYS_MSLEEP(MS) usleep(MS * 1000U)
static uint32_t get_systime()
{
    static struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return ((uint32_t)now.tv_sec * 1000U) + (uint32_t)(now.tv_nsec / 1000000U);
}
#else
//
#endif

static uint32_t _start_time = 0;
static volatile bool _exec_loop = false;

static void (*_disp_init_fun)() = NULL;
static void (*_disp_flush_fun)(struct _disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) = NULL;
static void (*_disp_exit_fun)() = NULL;

static void (*_indev_init_fun)() = NULL;
static bool (*_indev_read_fun)(struct _lv_indev_drv_t* indev_drv, lv_indev_data_t* data) = NULL;
static void (*_indev_exit_fun)() = NULL;

uint32_t lv_custom_tick_get(void)
{
    return get_systime() - _start_time;
}

void lv_custom_init(lv_custom_flags_t flags)
{
    _start_time = get_systime();
    _exec_loop = true;
    lv_init();
    if (flags & LV_CUSTOM_DISP) {
#if USE_WINDOWS
        windrv_init();
#elif USE_WIN32DRV
        lv_win32_init(NULL, 1, LV_HOR_RES_MAX, LV_VER_RES_MAX, NULL);
#elif USE_MONITOR
        _disp_init_fun = monitor_init;
        _disp_flush_fun = monitor_flush;
        _disp_exit_fun = monitor_exit;
#elif USE_FBDEV
        _disp_init_fun = fbdev_init;
        _disp_flush_fun = fbdev_flush;
        _disp_exit_fun = fbdev_exit;
#elif USE_DRM
        _disp_init_fun = drm_init;
        _disp_flush_fun = drm_flush;
        _disp_exit_fun = drm_exit;
#elif USE_OPENWFD
        _disp_init_fun = openwfd_init;
        _disp_flush_fun = openwfd_flush;
        _disp_exit_fun = openwfd_exit;
#else
#error Platform not supported!
#endif
        if (_disp_init_fun != NULL) {
            (*_disp_init_fun)();
            static lv_disp_buf_t disp_buf;
            static lv_color_t disp_buf1[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10];
            static lv_color_t disp_buf2[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10];
            lv_disp_buf_init(&disp_buf, disp_buf1, disp_buf2, LV_HOR_RES_MAX * LV_VER_RES_MAX / 10);
            lv_disp_drv_t disp_drv;
            lv_disp_drv_init(&disp_drv);
            disp_drv.hor_res = LV_HOR_RES_MAX;
            disp_drv.ver_res = LV_VER_RES_MAX;
            disp_drv.buffer = &disp_buf;
            disp_drv.flush_cb = _disp_flush_fun;
            lv_disp_t* disp = lv_disp_drv_register(&disp_drv);
            (void)disp;
        }
    }
    if (flags & LV_CUSTOM_INDEV) {
#if USE_MOUSE
        _indev_init_fun = mouse_init;
        _indev_read_fun = mouse_read;
#elif USE_EVDEV
        _indev_init_fun = evdev_init;
        _indev_read_fun = evdev_read;
#elif USE_MTOUCH
        _indev_init_fun = mtouch_init;
        _indev_read_fun = mtouch_read;
        _indev_exit_fun = mtouch_exit;
#else
#error
#endif
        if (_indev_init_fun != NULL) {
            (*_indev_init_fun)();
            lv_indev_drv_t indev_drv;
            lv_indev_drv_init(&indev_drv);
            indev_drv.type = LV_INDEV_TYPE_POINTER;
            indev_drv.read_cb = _indev_read_fun;
            lv_indev_t* indev = lv_indev_drv_register(&indev_drv);
            if (flags & LV_CUSTOM_CURSOR) {
                lv_obj_t* mouse_cursor = lv_img_create(lv_scr_act(), NULL);
                lv_img_set_src(mouse_cursor, &lv_custom_cursor);
                lv_indev_set_cursor(indev, mouse_cursor);
            }
        }
    }
#ifdef UNIX
    signal(SIGQUIT, lv_custom_exit);
    signal(SIGKILL, lv_custom_exit);
    signal(SIGINT, lv_custom_exit);
    signal(SIGTERM, lv_custom_exit);
#endif
}

void lv_custom_msleep(uint32_t ms)
{
    SYS_MSLEEP(ms);
}

void lv_custom_print(const char* s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stdout, s, args);
    fprintf(stdout, "\n");
    va_end(args);
    fflush(stdout);
}

void lv_custom_print_warn(const char* s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    fflush(stderr);
}

void lv_custom_exec(void)
{
    while (true == _exec_loop) {
        lv_custom_msleep(CUSTOM_TASK_INTEVAL);
        lv_task_handler();
    }
    if (_disp_exit_fun != NULL) {
        (*_disp_exit_fun)();
    }
    if (_indev_exit_fun != NULL) {
        (*_indev_exit_fun)();
    }
}

void lv_custom_exit(int flag)
{
    (void)flag;
    lv_custom_msleep(CUSTOM_TASK_INTEVAL);
    _exec_loop = false;
    lv_custom_msleep(CUSTOM_TASK_INTEVAL);
}
