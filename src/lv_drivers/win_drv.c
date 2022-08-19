/**
 * @file win_drv.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "win_drv.h"
#if USE_WINDOWS

#include "lv_custom/lv_custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>

#define FCACHE_COUNT 255

typedef struct {
    int x;
    int y;
    int width;
    int height;
    uint32_t* fbp;
    HBITMAP bmp;
} Frame_cache;

#if LV_COLOR_DEPTH < 16
#error Windows driver only supports true RGB colors at this time
#endif

/**********************
 *       DEFINES
 **********************/

#define WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void win_update(lv_task_t* t);
static void do_register(void);
static void win_drv_flush(struct _disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
//static void win_drv_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
static void win_drv_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p);
static bool win_drv_read(struct _lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
static void msg_handler(struct _lv_task_t* task_t);

static COLORREF lv_color_to_colorref(const lv_color_t color);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**********************
 *  GLOBAL VARIABLES
 **********************/

lv_disp_t* lv_windows_disp;

/**********************
 *  STATIC VARIABLES
 **********************/
static HWND hwnd = NULL;
static bool mouse_pressed;
static int mouse_x, mouse_y;
static Frame_cache fc[FCACHE_COUNT];
static uint16_t fc_index = 0;
static uint8_t fc_isfull = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
LPCWSTR g_szClassName = L"LittlevGL";

HWND windrv_init(void)
{
    WNDCLASSEX wc;
    HICON lvgl_icon;

    //Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    lvgl_icon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIcon = lvgl_icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = lvgl_icon;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassEx(&wc)) {
        return NULL;
    }
    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        g_szClassName,
        L"GDI Simulator",
        WS_POPUP | WS_VISIBLE,
        0, 0, WINDOW_HOR_RES, WINDOW_VER_RES,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        return NULL;
    }

    lv_task_create(msg_handler, 0, LV_TASK_PRIO_HIGHEST, NULL);
    lv_task_create(win_update, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_HIGH, NULL);
    do_register();

    return NULL;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void win_update(lv_task_t* t)
{
    (void)t;
    InvalidateRect(hwnd, NULL, FALSE);
    SendMessage(hwnd, WM_PAINT, 0, 0);
}

static void do_register(void)
{
    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /* LittlevGL requires a buffer where it draw the objects. The buffer's has to be greater than 1 display row
     *
     * There are three buffering configurations:
     * 1. Create ONE buffer some rows: LittlevGL will draw the display's content here and writes it to your display
     * 2. Create TWO buffer some rows: LittlevGL will draw the display's content to a buffer and writes it your display.
     *                                 You should use DMA to write the buffer's content to the display.
     *                                 It will enable LittlevGL to draw the next part of the screen to the other buffer while
     *                                 the data is being sent form the first buffer. It makes rendering and flushing parallel.
     * 3. Create TWO screen buffer: Similar to 2) but the buffer have to be screen sized. When LittlevGL is ready it will give the
     *                              whole frame to display. This way you only need to change the frame buffer's address instead of
     *                              copying the pixels.
     * */

    /* Example for 1) */
    static lv_disp_buf_t disp_buf_1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10]; /*A buffer for 10 rows*/
    //static lv_color_t buf1_2[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10]; /*A buffer for 10 rows*/
    lv_disp_buf_init(&disp_buf_1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX / 10); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = WINDOW_HOR_RES;
    disp_drv.ver_res = WINDOW_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = win_drv_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf_1;

    /*Finally register the driver*/
    lv_windows_disp = lv_disp_drv_register(&disp_drv);
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = win_drv_read;
    lv_indev_drv_register(&indev_drv);
}

static void draw(HDC hdc)
{
    if (hwnd == NULL) {
        return;
    }
    HDC hdcMem = CreateCompatibleDC(hdc);
    if (fc_isfull == 1) {
        for (int i = fc_index; i < FCACHE_COUNT; i++) {
            HBITMAP hbmOld = SelectObject(hdcMem, fc[i].bmp);
            BitBlt(hdc, fc[i].x, fc[i].y, fc[i].width, fc[i].height, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, hbmOld);
            DeleteObject(fc[i].bmp);
            free(fc[i].fbp);
        }
    }
    for (int i = 0; i < fc_index; i++) {
        HBITMAP hbmOld = SelectObject(hdcMem, fc[i].bmp);
        BitBlt(hdc, fc[i].x, fc[i].y, fc[i].width, fc[i].height, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hbmOld);
        DeleteObject(fc[i].bmp);
        free(fc[i].fbp);
    }
    fc_index = 0;
    fc_isfull = 0;
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
}

/**
 * Flush a buffer to the marked area
 * @param x1 left coordinate
 * @param y1 top coordinate
 * @param x2 right coordinate
 * @param y2 bottom coordinate
 * @param color_p an array of colors
 */
static void win_drv_flush(struct _disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    win_drv_map(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    lv_disp_flush_ready(disp_drv);
}

/**
 * Put a color map to the marked area
 * @param x1 left coordinate
 * @param y1 top coordinate
 * @param x2 right coordinate
 * @param y2 bottom coordinate
 * @param color_p an array of colors
 */
static void win_drv_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p)
{
    if (hwnd == NULL) {
        return;
    }
    int width = x2 - x1;
    int height = y2 - y1;
    uint32_t* fbp = malloc(sizeof(uint32_t) * width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fbp[y * width + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
    if (fc_index >= FCACHE_COUNT) {
        fc_index = 0;
        fc_isfull = 1;
        lv_custom_print("fc_isfull=true");
    }
    if (fc_isfull == 1) {
        DeleteObject(fc[fc_index].bmp);
        free(fc[fc_index].fbp);
    }
    fc[fc_index].x = x1;
    fc[fc_index].y = y1;
    fc[fc_index].width = width;
    fc[fc_index].height = height;
    fc[fc_index].fbp = fbp;
    fc[fc_index].bmp = CreateBitmap(width, height, 1, 32, fbp);
    fc_index++;
}

static bool win_drv_read(struct _lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{
    (void)indev_drv;
    data->state = mouse_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    return false;
}

static void msg_handler(struct _lv_task_t* task_t)
{
    (void)task_t;

    MSG msg;
    BOOL bRet;
    if ((bRet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) != 0) {
        if (bRet == -1) {
            return;
        } else {
            if (msg.message == WM_QUIT) {
                lv_custom_exit(0);
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        //SetTimer(hwnd, 0, 10, (TIMERPROC)lv_task_handler);
        //SetTimer(hwnd, 1, 25, NULL);
        return 0;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        mouse_x = GET_X_LPARAM(lParam);
        mouse_y = GET_Y_LPARAM(lParam);
        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
            mouse_pressed = (msg == WM_LBUTTONDOWN);
        }
        return 0;
    case WM_CLOSE:
        if (hwnd) {
            DestroyWindow(hwnd);
            hwnd = NULL;
        }
        return 0;
    case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        draw(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_TIMER:
        //lv_tick_inc(25);
        return 0;
    case WM_DESTROY:
        if (hwnd) {
            for (int i = fc_index; i < (fc_isfull == 1 ? FCACHE_COUNT : fc_index); i++) {
                DeleteObject(fc[i].bmp);
                free(fc[i].fbp);
            }
            PostQuitMessage(0);
            DestroyWindow(hwnd);
            hwnd = NULL;
        }
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
static COLORREF lv_color_to_colorref(const lv_color_t color)
{
    uint32_t raw_color = lv_color_to32(color);
    lv_color32_t tmp;
    tmp.full = raw_color;
    uint32_t colorref = RGB(tmp.ch.red, tmp.ch.green, tmp.ch.blue);
    return colorref;
}
#endif
