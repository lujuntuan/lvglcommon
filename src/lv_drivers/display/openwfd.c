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

/*********************
 *      INCLUDES
 *********************/
#include "openwfd.h"
#if USE_OPENWFD

#include "lv_custom/lv_custom.h"

#define WFD_WFDEXT_PROTOTYPES
#include "WF/wfd.h"
#include "WF/wfdext2.h"
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

#define MAX_NUM_OF_WFD_BUFFERS 3
#define MAX_NUM_OF_WFD_DEVICES 4
#define MAX_NUM_OF_WFD_PIPELINES 16
#define MAX_NUM_OF_WFD_PORT_MODES 64
#define MAX_NUM_OF_WFD_PORTS 4

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static int openwfd_destory(void);
static int openwfd_init_port(void);
static int openwfd_init_pipeline(void);
static int openwfd_init_buffer(void);
static int openwfd_power_on(void);
static int openwfd_power_off(void);
static int openwfd_commit(WFDCommitType type);
static int openwfd_fill_color(lv_color_t color);

/***********************
 *   GLOBAL PROTOTYPES
 ***********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static WFDint _num_client_id = -1;
static WFDint _num_port_index = -1;
static WFDint _num_port_mode_index = -1;
static WFDint _num_pipeline_id = -1;
//
static WFDDevice _wfd_device = WFD_INVALID_HANDLE;
static WFDPort _wfd_port = WFD_INVALID_HANDLE;
static WFDPipeline _wfd_pipeline = WFD_INVALID_HANDLE;
static WFDSource _wfd_source = WFD_INVALID_HANDLE;
static WFD_EGLImageType* _wfd_image = NULL;
static WFDint _wfd_port_width = 0;
static WFDint _wfd_port_height = 0;
static lv_color_int_t* _wfd_fbp = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static int getid_for_env(const char* name, int type)
{
    char* value = NULL;
    value = getenv(name);
    int num = -1;
    if (value) {
        if (type == 10) {
            sscanf(value, "%d", &num);
        } else if (type == 16) {
            sscanf(value, "%x", &num);
        }
    }
    return num;
}

void openwfd_init(void)
{
    _num_client_id = getid_for_env("WFD_CLIENT_ID", 16);
    _num_port_index = getid_for_env("WFD_PORT_INDEX", 10);
    _num_port_mode_index = getid_for_env("WFD_PORT_MODE_INDEX", 10);
    _num_pipeline_id = getid_for_env("WFD_PIPELINE_ID", 10);

    waitfor("/dev/openwfd_server", 5000, 1);

    if (openwfd_init_port() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_init_port!");
        exit(-1);
        return;
    }
    if (openwfd_power_on() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_power_on!");
        exit(-1);
        return;
    }
    if (openwfd_init_pipeline() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_init_pipeline!");
        exit(-1);
        return;
    }
    if (openwfd_init_buffer() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_init_buffer!");
        exit(-1);
        return;
    }
    if (openwfd_fill_color(LV_COLOR_BLACK) != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_fill_color!");
        exit(-1);
        return;
    }
    if (openwfd_commit(WFD_COMMIT_ENTIRE_PORT) != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_commit WFD_COMMIT_ENTIRE_PORT!");
        exit(-1);
        return;
    }
}

void openwfd_exit()
{
    if (openwfd_power_off() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_power_off!");
        exit(-1);
        return;
    }
    if (openwfd_commit(WFD_COMMIT_ENTIRE_PORT) != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_commit WFD_COMMIT_ENTIRE_PORT!");
        exit(-1);
        return;
    }
    if (openwfd_destory() != 0) {
        lv_custom_print_warn("[openwfd] failed to openwfd_destory!");
        exit(-1);
        return;
    }
}

void openwfd_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    if (_wfd_fbp == NULL) {
        return;
    }
    lv_coord_t imageW = lv_area_get_width(area);
    for (int32_t y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
        memcpy(&_wfd_fbp[y * _wfd_port_width + area->x1], color_p, imageW * sizeof(lv_color_t));
        color_p += imageW;
    }
    lv_disp_flush_ready(disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int openwfd_destory(void)
{
    WFDErrorCode eError;
    if (_wfd_image != NULL) {
        wfdDestroyWFDEGLImages(_wfd_device, 1, (WFDEGLImage*)&(_wfd_image));
        eError = wfdGetError(_wfd_device);
        if (eError != WFD_ERROR_NONE) {
            lv_custom_print_warn("[openwfd] destroy image error!");
            return 2;
        }
        _wfd_image = NULL;
        _wfd_fbp = NULL;
    }
    if (_wfd_source != WFD_INVALID_HANDLE) {
        wfdDestroySource(_wfd_device, _wfd_source);
        eError = wfdGetError(_wfd_device);
        if (eError != WFD_ERROR_NONE) {
            lv_custom_print_warn("[openwfd] destroy source error!");
            return 3;
        }
        _wfd_source = WFD_INVALID_HANDLE;
    }
    if (_wfd_pipeline != WFD_INVALID_HANDLE) {
        wfdDestroyPipeline(_wfd_device, _wfd_pipeline);
        eError = wfdGetError(_wfd_device);
        if (eError != WFD_ERROR_NONE) {
            lv_custom_print_warn("[openwfd] destroy pipeline error!");
            return 4;
        }
        _wfd_pipeline = WFD_INVALID_HANDLE;
    }
    if (_wfd_port != WFD_INVALID_HANDLE) {
        wfdDestroyPort(_wfd_device, _wfd_port);
        eError = wfdGetError(_wfd_device);
        if (eError != WFD_ERROR_NONE) {
            lv_custom_print_warn("[openwfd] destroy port error!");
            return 5;
        }
        _wfd_port = WFD_INVALID_HANDLE;
    }
    if (_wfd_device != WFD_INVALID_HANDLE) {
        wfdDestroyDevice(_wfd_device);
        eError = wfdGetError(_wfd_device);
        if (eError != WFD_ERROR_NONE) {
            lv_custom_print_warn("[openwfd] destroy device error!");
            return 6;
        }
        _wfd_device = WFD_INVALID_HANDLE;
    }
    return 0;
}

static int openwfd_init_port(void)
{
    WFDint numDevs = wfdEnumerateDevices(0, 0, 0);
    WFDint devIds[MAX_NUM_OF_WFD_DEVICES];

    if (numDevs > 0) {
        wfdEnumerateDevices(devIds, numDevs, 0);
    }

    WFDint dev_attribs[3] = { WFD_DEVICE_CLIENT_TYPE,
        _num_client_id >= 0 ? _num_client_id : OPENWFD_DEF_CLIENT_ID,
        WFD_NONE };
    _wfd_device = wfdCreateDevice(WFD_DEFAULT_DEVICE_ID, dev_attribs);
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] create device error!");
        return 1;
    }
    WFDint portIds[MAX_NUM_OF_WFD_PORTS];
    WFDint numPorts = wfdEnumeratePorts(_wfd_device, 0, 0, 0);
    wfdEnumeratePorts(_wfd_device, portIds, numPorts, 0);

    if (numPorts <= 0) {
        lv_custom_print_warn("[openwfd] ports is empty!");
        return 2;
    }
    WFDint targetPort = (_num_port_index >= 0 && _num_port_index < numPorts) ? portIds[_num_port_index] : portIds[0];

    _wfd_port = wfdCreatePort(_wfd_device, targetPort, 0);

    if (_wfd_port == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] create port error!");
        return 3;
    }

    WFDint numPortModes = wfdGetPortModes(_wfd_device, _wfd_port, 0, 0);
    WFDPortMode portModes[MAX_NUM_OF_WFD_PORT_MODES];
    wfdGetPortModes(_wfd_device, _wfd_port, portModes, numPortModes);

    if (numPortModes <= 0) {
        lv_custom_print_warn("[openwfd] port modes is empty!");
        return 4;
    }

    WFDPortMode targetPortMode = (_num_port_mode_index >= 0 && _num_port_mode_index < numPortModes) ? portModes[_num_port_mode_index] : portModes[0];

    _wfd_port_width = wfdGetPortModeAttribi(_wfd_device, _wfd_port, targetPortMode, WFD_PORT_MODE_WIDTH);
    _wfd_port_height = wfdGetPortModeAttribi(_wfd_device, _wfd_port, targetPortMode, WFD_PORT_MODE_HEIGHT);

    if (_wfd_port_width <= 0 || _wfd_port_height <= 0) {
        lv_custom_print_warn("[openwfd] port size is null!");
        return 5;
    }

    wfdSetPortMode(_wfd_device, _wfd_port, targetPortMode);
    WFDErrorCode eError = wfdGetError(_wfd_device);

    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set port mode error!");
        return 6;
    }

    return 0;
}
static int openwfd_init_pipeline(void)
{
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] device is null!");
        return 1;
    }
    if (_wfd_port == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] port is null!");
        return 2;
    }
    WFDint numPipelines = wfdEnumeratePipelines(_wfd_device, 0, 0, 0);

    WFDint pipelineIds[MAX_NUM_OF_WFD_PIPELINES];
    wfdEnumeratePipelines(_wfd_device, pipelineIds, numPipelines, 0);

    if (numPipelines <= 0) {
        lv_custom_print_warn("[openwfd] pipelines is empty!");
        return 3;
    }
    WFDint targetPipelineID = -1;
    if (_num_pipeline_id >= 0) {
        for (int i = 0; i < numPipelines; i++) {
            if (pipelineIds[i] == _num_pipeline_id) {
                targetPipelineID = _num_pipeline_id;
                break;
            }
        }
        if (targetPipelineID < 0) {
            lv_custom_print_warn("[openwfd] has not default pipeline!");
            targetPipelineID = pipelineIds[0];
        }
    } else {
        targetPipelineID = pipelineIds[0];
    }

    _wfd_pipeline = wfdCreatePipeline(_wfd_device, targetPipelineID, 0);
    if (_wfd_pipeline == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] set pipeline error!");
        return 4;
    }

    wfdSetPipelineAttribi(_wfd_device, _wfd_pipeline, WFD_PIPELINE_TRANSPARENCY_ENABLE,
        (WFD_TRANSPARENCY_SOURCE_ALPHA | WFD_TRANSPARENCY_GLOBAL_ALPHA));

    WFDErrorCode eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set WFD_PIPELINE_TRANSPARENCY_ENABLE error!");
        return 5;
    }

    wfdSetPipelineAttribi(_wfd_device, _wfd_pipeline, WFD_PIPELINE_GLOBAL_ALPHA, 255);
    eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set WFD_PIPELINE_GLOBAL_ALPHA error!");
        return 6;
    }

    WFDint piplineRect[4] = { 0, 0, _wfd_port_width, _wfd_port_height };
    wfdSetPipelineAttribiv(_wfd_device, _wfd_pipeline, WFD_PIPELINE_DESTINATION_RECTANGLE, 4, piplineRect);
    eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set WFD_PIPELINE_DESTINATION_RECTANGLE error!");
        return 7;
    }

    wfdSetPipelineAttribiv(_wfd_device, _wfd_pipeline, WFD_PIPELINE_SOURCE_RECTANGLE, 4, piplineRect);
    eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set WFD_PIPELINE_SOURCE_RECTANGLE error!");
        return 8;
    }

    wfdBindPipelineToPort(_wfd_device, _wfd_port, _wfd_pipeline);
    eError = wfdGetError(_wfd_device);
    if (WFD_ERROR_NONE != eError) {
        lv_custom_print_warn("[openwfd] set port to pipeline error!");
        return 9;
    }

    return 0;
}

static int openwfd_init_buffer(void)
{
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] device is null!");
        return 1;
    }
    if (_wfd_port == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] port is null!");
        return 2;
    }
    if (_wfd_pipeline == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] pipeline is null!");
        return 3;
    }

    WFDEGLImage eglImageHandle = NULL;

    wfdCreateWFDEGLImages(_wfd_device, _wfd_port_width, _wfd_port_height,
        WFD_FORMAT_RGBA8888, WFD_USAGE_OPENGL_ES2 | WFD_USAGE_DISPLAY,
        1, &(eglImageHandle), 0);

    _wfd_image = (WFD_EGLImageType*)eglImageHandle;

    if (_wfd_image == NULL) {
        lv_custom_print_warn("[openwfd] create WDFEGLImages error!");
        return 4;
    }

    _wfd_source = wfdCreateSourceFromImage(_wfd_device, _wfd_pipeline, _wfd_image, 0);
    if (_wfd_source == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] create source from EGLImage error!");
        return 5;
    }

    wfdBindSourceToPipeline(_wfd_device, _wfd_pipeline, _wfd_source, WFD_TRANSITION_AT_VSYNC, NULL);
    WFDErrorCode eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] bind source to pipline error!");
        return 6;
    }

    _wfd_fbp = (lv_color_int_t*)_wfd_image->vaddr;
    if (_wfd_fbp == NULL || _wfd_fbp == 0) {
        lv_custom_print_warn("[openwfd] _wfd_fbp is null!");
        return 7;
    }

    return 0;
}

static int openwfd_power_on(void)
{
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] device is null!");
        return 1;
    }
    if (_wfd_port == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] port is null!");
        return 2;
    }
    wfdSetPortAttribi(_wfd_device, _wfd_port, WFD_PORT_POWER_MODE, WFD_POWER_MODE_ON);
    WFDErrorCode eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set power on error!");
        return 3;
    }
    return 0;
}
static int openwfd_power_off(void)
{
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] device is null!");
        return 1;
    }
    if (_wfd_port == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] port is null!");
        return 2;
    }
    wfdSetPortAttribi(_wfd_device, _wfd_port, WFD_PORT_POWER_MODE, WFD_POWER_MODE_OFF);
    WFDErrorCode eError = wfdGetError(_wfd_device);
    if (eError != WFD_ERROR_NONE) {
        lv_custom_print_warn("[openwfd] set power on error!");
        return 3;
    }
    return 0;
}

static int openwfd_commit(WFDCommitType type)
{
    if (_wfd_device == WFD_INVALID_HANDLE) {
        lv_custom_print_warn("[openwfd] device is null!");
        return 1;
    }
    switch (type) {
    case WFD_COMMIT_ENTIRE_DEVICE:
        wfdDeviceCommit(_wfd_device, WFD_COMMIT_ENTIRE_DEVICE, _wfd_device);
        break;
    case WFD_COMMIT_ENTIRE_PORT:
        if (_wfd_port == WFD_INVALID_HANDLE) {
            lv_custom_print_warn("[openwfd] port is null!");
            return 2;
        }
        wfdDeviceCommit(_wfd_device, WFD_COMMIT_ENTIRE_PORT, _wfd_port);
        break;
    case WFD_COMMIT_PIPELINE:
        if (_wfd_port == WFD_INVALID_HANDLE) {
            lv_custom_print_warn("[openwfd] port is null!");
            return 2;
        }
        if (_wfd_pipeline == WFD_INVALID_HANDLE) {
            lv_custom_print_warn("[openwfd] pipeline is null!");
            return 3;
        }
        wfdDeviceCommit(_wfd_device, WFD_COMMIT_PIPELINE, _wfd_pipeline);
        break;
    case WFD_COMMIT_FORCE_32BIT:
        return 5;
        break;
    default:
        return 6;
        break;
    }
    WFDErrorCode eError = wfdGetError(_wfd_device);
    if (WFD_ERROR_NONE != eError) {
        lv_custom_print_warn("[openwfd] commit error!");
        return 7;
    }
    return 0;
}

static int openwfd_fill_color(lv_color_t color)
{
    if (_wfd_fbp == NULL) {
        lv_custom_print_warn("[openwfd] _wfd_fbp is null!");
        return 1;
    }
    lv_color_t alphaColor = color;
    LV_COLOR_SET_A(alphaColor, 0);
    for (int32_t y = 0; y <= _wfd_port_height; y++) {
        for (int32_t x = 0; x <= _wfd_port_width; x++) {
            _wfd_fbp[y * _wfd_port_width + x] = alphaColor.full;
        }
    }
    return 0;
}

#endif /*USE_OPENWFD*/
