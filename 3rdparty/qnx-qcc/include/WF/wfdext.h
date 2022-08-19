/*! \file */
/*
================================================================================

File: wfdext.h

Description:
  This file contains extensions for the OpenWF Display API.

$File:

================================================================================
Copyright (c) 2018 - 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
================================================================================
*/

#ifndef __wfdext_h_
#define __wfdext_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <WF/wfdplatform.h>

#define WFD_WFDEXT_VERSION       1
#define WFD_FIRST_VALID_INDEX    1
#define WFD_INVALID_SOURCE_INDEX 0

#ifndef WFD_QNX_error_timeout
/**
 * This extension provides a way to return the timeout error to the
 * application in case of any.
 */
#define WFD_QNX_error_timeout 1
/* Error extension */
#define WFD_ERROR_TIMEOUT   0x7518
#endif

#ifndef WFD_QNX_cbabc
#define WFD_QNX_cbabc 1
#define WFD_PORT_CBABC_MODE_QNX 0x7670
typedef enum
{ WFD_PORT_CBABC_MODE_NONE_QNX   = 0x7671,
  WFD_PORT_CBABC_MODE_VIDEO_QNX  = 0x7672,
  WFD_PORT_CBABC_MODE_UI_QNX     = 0x7673,
  WFD_PORT_CBABC_MODE_PHOTO_QNX  = 0x7674,
  WFD_PORT_CBABC_MODE_32BIT_QNX  = 0x7FFFFFFF
} WFDPortCBABCModeQNX;
#endif

#ifndef WFD_QNX_port_brightness
/**
 * This extension provides a way of changing the perceived brightess of a port.
 *
 * WFD_PORT_CURRENT_BRIGHTNESS_QNX is a read-only property that returns the
 * current brightness as a normalized value between 0.0 and 1.0.
 *
 * WFD_PORT_BRIGHTNESS_QNX is a read/write property that is used to set
 * the current brightness. Values can be:
 *   -1.0       Indicates that the system has automatic control over
 *              the brightness
 *   [0.0,1.0]  An absolute brightness ranging from off to maximum.
 *
 * @note The use of 'brightness' in this extension is inconsistent with the
 *       'brightness' in the bchs extension, where brightness affects the
 *       color, not the way the color is displayed. In other words, pipeline
 *       brightness changes the color in the WFDDestinationQNX when writeback
 *       is enabled, whereas the port brightness doesn't.
 */
#define WFD_QNX_port_brightness 1
#define WFD_PORT_CURRENT_BRIGHTNESS_QNX   0x7650
#define WFD_PORT_BRIGHTNESS_QNX           0x7651
#endif

#ifndef WFD_QNX_port_color_profile
/**
 * This extension provides a way of querying a port's current color profile.
 *
 * WFD_PORT_COLOR_POINT_####_QNX is a read-only property that returns the
 * requested x,y,Y values after any transformations by a HW CMU block (if present)
 *
 */
#define WFD_QNX_port_color_profile 1
#define WFD_PORT_COLOR_POINT_RED_QNX    0x7652
#define WFD_PORT_COLOR_POINT_GREEN_QNX  0x7653
#define WFD_PORT_COLOR_POINT_BLUE_QNX   0x7654
#define WFD_PORT_COLOR_POINT_WHITE_QNX  0x7655
#define WFD_PORT_COLOR_POINT_RED        0x7652
#define WFD_PORT_COLOR_POINT_GREEN      0x7653
#define WFD_PORT_COLOR_POINT_BLUE       0x7654
#define WFD_PORT_COLOR_POINT_WHITE      0x7655
#endif

#ifndef WFD_QNX_port_gamma_curve
/**
 * WFD_PORT_###_GAMMA_CURVE is a port property for getting/setting gamma curve
 * values. Each R/G/B gamma curve is defined as an array of 256 elements of
 * integers, ranging from 0 to 65535.
 */
#define WFD_QNX_port_gamma_curve 1
#define WFD_PORT_RED_GAMMA_CURVE_QNX    0x7690
#define WFD_PORT_GREEN_GAMMA_CURVE_QNX  0x7691
#define WFD_PORT_BLUE_GAMMA_CURVE_QNX   0x7692
#endif

#ifndef WFD_QNX_port_mode_info
#define WFD_QNX_port_mode_info 1
#define WFD_PORT_MODE_ASPECT_RATIO_QNX  0x7606
#define WFD_PORT_MODE_PREFERRED_QNX     0x7607
#endif

#ifndef WFD_QNX_vsync
#define WFD_QNX_vsync 1
#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdWaitForVSyncQNX(WFDDevice device, WFDPort port) WFD_APIEXIT;
#endif /* WFD_WFDEXT_PROTOTYPES */
typedef WFDErrorCode (WFD_APIENTRY PFNWFDWAITFORVSYNCQNX) (WFDDevice device, WFDPort port);
#endif

#ifndef WFD_QNX_write_back
#define WFD_QNX_write_back 1
typedef WFDHandle WFDDestinationQNX;
#define WFD_PORT_WRITEBACK_SUPPORT_QNX               0x7640
#define WFD_PORT_WRITEBACK_SCALE_RANGE_QNX           0x7641
#define WFD_PORT_WRITEBACK_SOURCE_RECTANGLE_QNX      0x7642
#define WFD_PORT_WRITEBACK_DESTINATION_RECTANGLE_QNX 0x7643
#define WFD_EVENT_PORT_BIND_DESTINATION_COMPLETE_QNX 0x7587
#define WFD_EVENT_PORT_BIND_PORT_ID_QNX              0x75C9
#define WFD_EVENT_PORT_BIND_DESTINATION_QNX          0x75CA
#define WFD_EVENT_PORT_BIND_QUEUE_OVERFLOW_QNX       0x75CB
#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDDestinationQNX WFD_APIENTRY
  wfdCreateDestinationFromImageQNX(WFDDevice device, WFDPort port, WFDEGLImage image, const WFDint *attribList) WFD_APIEXIT;
WFD_API_CALL WFDDestinationQNX WFD_APIENTRY
  wfdCreateDestinationFromStreamQNX(WFDDevice device, WFDPort port, WFDNativeStreamType stream, const WFDint *attribList) WFD_APIEXIT;
WFD_API_CALL void WFD_APIENTRY
    wfdDestroyDestinationQNX(WFDDevice device, WFDDestinationQNX destination) WFD_APIEXIT;
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdBindDestinationToPortQNX(WFDDevice device, WFDPort port, WFDDestinationQNX destination, WFDTransition transition) WFD_APIEXIT;
#endif
typedef WFDDestinationQNX (WFD_APIENTRY PFNWFDCREATEDESTINATIONFROMIMAGEQNX) (WFDDevice device, WFDPort port, WFDEGLImage image, const WFDint *attribList);
typedef WFDDestinationQNX (WFD_APIENTRY PFNWFDCREATEDESTINATIONFROMSTREAMQNX) (WFDDevice device, WFDPort port, WFDNativeStreamType stream, const WFDint *attribList);
typedef void (WFD_APIENTRY PFNWFDDESTROYDESTINATIONQNX) (WFDDevice device, WFDDestinationQNX destination);
typedef void (WFD_APIENTRY PFNWFDBINDDESTINATIONTOPORTQNX) (WFDDevice device, WFDPort port, WFDDestinationQNX destination, WFDTransition transition);
#endif

#ifndef WFD_QNX_bchs_filter
#define WFD_QNX_bchs_filter 1
#define WFD_PIPELINE_BRIGHTNESS_QNX     0x7750
#define WFD_PIPELINE_CONTRAST_QNX       0x7751
#define WFD_PIPELINE_HUE_QNX            0x7752
#define WFD_PIPELINE_SATURATION_QNX     0x7753
#endif

#ifndef WFD_QNX_pipeline_color_space
#define WFD_QNX_pipeline_color_space 1
#define WFD_PIPELINE_COLOR_SPACE_QNX    0x77A0

/* Those defines needs to match with screen.h */
#define WFD_COLOR_SPACE_UNCORRECTED_QNX 0x0
#define WFD_COLOR_SPACE_SRGB_QNX        0x1
#define WFD_COLOR_SPACE_LRGB_QNX        0x2
#define WFD_COLOR_SPACE_BT601_QNX       0x3
#define WFD_COLOR_SPACE_BT601_FULL_QNX  0x4
#define WFD_COLOR_SPACE_BT709_QNX       0x5
#define WFD_COLOR_SPACE_BT709_FULL_QNX  0x6
#endif

#ifndef WFD_QNX_egl_images
#define WFD_QNX_egl_images 1
#define WFD_USAGE_DISPLAY_QNX         (1 << 0)
#define WFD_USAGE_READ_QNX            (1 << 1)
#define WFD_USAGE_WRITE_QNX           (1 << 2)
#define WFD_USAGE_NATIVE_QNX          (1 << 3)
#define WFD_USAGE_OPENGL_ES1_QNX      (1 << 4)
#define WFD_USAGE_OPENGL_ES2_QNX      (1 << 5)
#define WFD_USAGE_OPENGL_ES3_QNX      (1 << 11)
#define WFD_USAGE_OPENVG_QNX          (1 << 6)
#define WFD_USAGE_VIDEO_QNX           (1 << 7)
#define WFD_USAGE_CAPTURE_QNX         (1 << 8)
#define WFD_USAGE_ROTATION_QNX        (1 << 9)
#define WFD_USAGE_OVERLAY_QNX         (1 << 10)
#define WFD_USAGE_COMPRESSION_QNX     (1 << 12)
#define WFD_USAGE_WRITEBACK_QNX       (1 << 31)
#define WFD_FORMAT_BYTE_QNX           1
#define WFD_FORMAT_RGBA4444_QNX       2
#define WFD_FORMAT_RGBX4444_QNX       3
#define WFD_FORMAT_RGBA5551_QNX       4
#define WFD_FORMAT_RGBX5551_QNX       5
#define WFD_FORMAT_RGB565_QNX         6
#define WFD_FORMAT_RGB888_QNX         7
#define WFD_FORMAT_RGBA8888_QNX       8
#define WFD_FORMAT_RGBX8888_QNX       9
#define WFD_FORMAT_YVU9_QNX           10
#define WFD_FORMAT_YUV420_QNX         11
#define WFD_FORMAT_NV12_QNX           12
#define WFD_FORMAT_YV12_QNX           13
#define WFD_FORMAT_UYVY_QNX           14
#define WFD_FORMAT_YUY2_QNX           15
#define WFD_FORMAT_YVYU_QNX           16
#define WFD_FORMAT_V422_QNX           17
#define WFD_FORMAT_AYUV_QNX           18
#define WFD_FORMAT_NV16_QNX           19
#ifndef WFD_FORMAT_NV12_QC_SUPERTILE
#define WFD_FORMAT_NV12_QC_SUPERTILE  ((1 << 16) | WFD_FORMAT_NV12_QNX)
#endif
#ifndef WFD_FORMAT_NV12_QC_32M4KA
#define WFD_FORMAT_NV12_QC_32M4KA     ((2 << 16) | WFD_FORMAT_NV12_QNX)
#endif

#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdCreateWFDEGLImagesQNX(WFDDevice device, WFDint width, WFDint height, WFDint format, WFDint usage, WFDint count, WFDEGLImage *images) WFD_APIEXIT;
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdDestroyWFDEGLImagesQNX(WFDDevice device, WFDint count, WFDEGLImage *images) WFD_APIEXIT;
#endif /* WFD_WFDEXT_PROTOTYPES */
typedef WFDErrorCode (WFD_APIENTRY PFNWFDCREATEWFDEGLIMAGESQNX) (WFDDevice device, WFDint width, WFDint height, WFDint usage, WFDint count, WFDEGLImage *images);
typedef WFDErrorCode (WFD_APIENTRY PFNWFDDESTROYWFDEGLIMAGESQNX) (WFDDevice device, WFDint count, WFDEGLImage *images);
#endif /* WFD_QNX_egl_images */

#ifdef __cplusplus
}
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/beta/lib/khronos/public/WF/wfdext.h $ $Rev: 800619 $")
#endif

#endif /* #ifndef __wfdext_h_ */
