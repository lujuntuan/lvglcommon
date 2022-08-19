/*! \file */
/*
================================================================================

File: wfdext2.h

Description:
  This file contains extensions for the OpenWF Display API.

  $File:  $

================================================================================
Copyright (c) 2018 - 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
================================================================================
*/

#ifndef __wfdext2_h_
#define __wfdext2_h_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__QNXNTO__)
#include <WF/wfdplatform.h>
#else
#include "wfdplatform.h"
#endif

#ifndef WFDuint32
typedef khronos_uint32_t WFDuint32;
#endif

#ifndef WFDint64
typedef khronos_int64_t WFDint64;
#endif

#ifndef WFDuint64
typedef khronos_uint64_t WFDuint64;
#endif

#define WFD_WFDEXT_VERSION 1

#ifndef WFD_error_timeout
/**
 * This extension provides a way to return the timeout error to the
 * application in case of any.
 */
#define WFD_error_timeout 1
/* Error extension */
#define WFD_ERROR_TIMEOUT   0x7518
#endif

#ifndef WFD_multi_client
#define WFD_multi_client 1

#define WFD_CLIENT_ID_MIN       0x7800
#define WFD_CLIENT_ID_MAX       0x78FF

#define WFD_DEVICE_CLIENT_TYPE  0x7561
typedef enum
{
  WFD_CLIENT_TYPE_NONE,
  WFD_CLIENT_TYPE_CLUSTER,
  WFD_CLIENT_TYPE_MONITOR,
  WFD_CLIENT_TYPE_TELLTALE,
  WFD_CLIENT_TYPE_RVC,
  WFD_CLIENT_TYPE_LA_GVM,
  WFD_CLIENT_TYPE_LV_GVM,
  WFD_CLIENT_TYPE_SPLASH,
  WFD_CLIENT_TYPE_MAX,
  WFD_CLIENT_TYPE_FORCE_32BIT = 0x7FFFFFFF
} WFDClientType;
#endif

#ifndef WFD_device_get_attribiv
#define WFD_device_get_attribiv 1
typedef enum
{
  WFD_DEVICE_MIN_MAX_WIDTH_HEIGHT   = 0x7561,
  WFD_DEVICE_ATTRIB_EXT_FORCE_32BIT = 0x7FFFFFFF
} WFDDeviceAttribEXT;
#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL void WFD_APIENTRY
  wfdGetDeviceAttribiv(WFDDevice device, WFDDeviceAttribEXT attrib, WFDint count, WFDint *value) WFD_APIEXIT;
#endif /* WFD_WFDEXT_PROTOTYPES */
typedef void (WFD_APIENTRY PFNWFDGETDEVICEATTRIBIV) (WFDDevice device, WFDDeviceAttribEXT attrib, WFDint count, WFDint *value);
#endif

#ifndef WFD_cbabc
#define WFD_cbabc 1
#define WFD_PORT_CBABC_MODE 0x7670
typedef enum
{
  WFD_PORT_CBABC_MODE_NONE  = 0x7671,
  WFD_PORT_CBABC_MODE_VIDEO = 0x7672,
  WFD_PORT_CBABC_MODE_UI    = 0x7673,
  WFD_PORT_CBABC_MODE_PHOTO = 0x7674,
  WFD_PORT_CBABC_MODE_32BIT = 0x7FFFFFFF
} WFDPortCBABCMode;
#endif

#ifndef WFD_port_types_ext
#define WFD_port_types_ext  1
#define WFD_PORT_TYPE_DSI   0x766A
#endif

#ifndef WFD_port_brightness
/**
 * This extension provides a way of changing the perceived brightess of a port.
 *
 * WFD_PORT_CURRENT_BRIGHTNESS is a read-only property that returns the current
 * brightness as a normalized value between 0.0 and 1.0.
 *
 * WFD_PORT_BRIGHTNESS is a read/write property that is used to set the current
 * brightness. Values can be:
 *   -1.0       Indicates that the system has automatic control over
 *              the brightness
 *   [0.0,1.0]  An absolute brightness ranging from off to maximum.
 *
 * @note The use of 'brightness' in this extension is inconsistent with the
 *       'brightness' in the bchs extension, where brightness affects the
 *       color, not the way the color is displayed. In other words, pipeline
 *       brightness changes the color in the WFDDestination when writeback
 *       is enabled, whereas the port brightness doesn't.
 */
#define WFD_port_brightness 1
#define WFD_PORT_CURRENT_BRIGHTNESS   0x7650
#define WFD_PORT_BRIGHTNESS           0x7651
#endif

#ifndef WFD_port_color_profile
/**
 * This extension provides a way of querying a port's current color profile.
 *
 * WFD_PORT_COLOR_POINT_#### is a read-only property that returns the requested
 * x,y,Y values after any transformations by a HW CMU block (if present)
 *
 */
#define WFD_port_color_profile 1
#define WFD_PORT_COLOR_POINT_RED    0x7652
#define WFD_PORT_COLOR_POINT_GREEN  0x7653
#define WFD_PORT_COLOR_POINT_BLUE   0x7654
#define WFD_PORT_COLOR_POINT_WHITE  0x7655
#endif

#ifndef WFD_port_roi_crc_capture
/**
 * ROI CRC capture
 *
 * Note:
 *  Returned ROI CRC value corresponds to the frame for which the last v-sync
 *  has fired. V-sync signal indicates that a frame has finished rendering
 *  to the physical display.
 */
#define WFD_port_roi_crc_capture 1

/**
 * WFD_PORT_NUM_OF_ROI_CRC
 *  Returns number of ROI CRC engines supported for a given port.
 *
 * Get:
 *  value[0] - Number of ROI CRC supported by this port (N)
 */
#define WFD_PORT_NUM_OF_ROI_CRC     0x7656

/**
 * WFD_PORT_ROI_CRC_INFO
 *  Returns the ROI range of each ROI CRC engine.
 *
 * Get:
 * For each ROI(n), n=0...(N-1)
 *  value[4(n)+0] - X offset of ROI range
 *  value[4(n)+1] - Y offset of ROI range
 *  value[4(n)+2] - Width of ROI range
 *  value[4(n)+3] - Height of ROI range
 *
 * Alternatively, typecast value to WFDRect:
 *  (WFDRect)&value[4(n)], n=0...(N-1)
 */
#define WFD_PORT_ROI_CRC_INFO       0x7657

/**
 * WFD_PORT_ROI_CRC
 *  Used for configuring the ROI CRC engine and getting the CRC value.
 * 
 * Set:
 * For each ROI(n), n=0...(N-1),
 *  value[5(n)+0] - CRC enable
 *  value[5(n)+1] - CRC X offset of ROI range
 *  value[5(n)+2] - CRC Y offset of ROI range
 *  value[5(n)+3] - CRC Width of ROI range
 *  value[5(n)+4] - CRC Height of ROI range
 *
 * Alternatively, typecast value to WFDPortRoiCrcSetType:
 *  (WFDPortRoiCrcSetType)&value[5(n)], n=0...(N-1)
 *
 * Get:
 * For each ROI(n), n=0...(N-1),
 *  value[2(n)+0] - CRC value
 *  value[2(n)+1] - CRC validity
 *
 * Alternatively, typecast value to WFDPortRoiCrcGetType:
 *  (WFDPortRoiCrcGetType)&value[2(n)], n=0...(N-1)
 */
typedef struct
{
  WFDboolean  enable;
  WFDRect     rect;
} WFDPortRoiCrcSetType;
typedef struct
{
  WFDuint32   crc;
  WFDboolean  valid;
} WFDPortRoiCrcGetType;
#define WFD_PORT_ROI_CRC            0x7658
#endif /* #ifndef WFD_port_roi_crc_capture */

#ifndef WFD_port_gamma_curve
/**
 * WFD_PORT_###_GAMMA_CURVE is a port property for getting/setting gamma curve
 * values. Each R/G/B gamma curve is defined as an array of 256 elements of
 * integers, ranging from 0 to 65535.
 */
#define WFD_port_gamma_curve 1
#define WFD_PORT_RED_GAMMA_CURVE    0x7690
#define WFD_PORT_GREEN_GAMMA_CURVE  0x7691
#define WFD_PORT_BLUE_GAMMA_CURVE   0x7692
#endif

#ifndef WFD_port_mode_info
#define WFD_port_mode_info 1
#define WFD_PORT_MODE_ASPECT_RATIO  0x7606
#define WFD_PORT_MODE_PREFERRED     0x7607
#endif

#ifndef WFD_vsync
#define WFD_vsync 1
#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdWaitForVSync(WFDDevice device, WFDPort port) WFD_APIEXIT;
#endif /* WFD_WFDEXT_PROTOTYPES */
typedef WFDErrorCode (WFD_APIENTRY PFNWFDWAITFORVSYNC) (WFDDevice device, WFDPort port);
#endif

#ifndef WFD_write_back
#define WFD_write_back 1
typedef WFDHandle WFDDestination;
#define WFD_PORT_WRITEBACK_SUPPORT                0x7640
#define WFD_PORT_WRITEBACK_SCALE_RANGE            0x7641
#define WFD_PORT_WRITEBACK_SOURCE_RECTANGLE       0x7642
#define WFD_PORT_WRITEBACK_DESTINATION_RECTANGLE  0x7643
#define WFD_EVENT_PORT_BIND_DESTINATION_COMPLETE  0x7587
#define WFD_EVENT_PORT_BIND_PORT_ID               0x75C9
#define WFD_EVENT_PORT_BIND_DESTINATION           0x75CA
#define WFD_EVENT_PORT_BIND_QUEUE_OVERFLOW        0x75CB
#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDDestination WFD_APIENTRY
  wfdCreateDestinationFromImage(WFDDevice device, WFDPort port, WFDEGLImage image, const WFDint *attribList) WFD_APIEXIT;
WFD_API_CALL WFDDestination WFD_APIENTRY
  wfdCreateDestinationFromStream(WFDDevice device, WFDPort port, WFDNativeStreamType stream, const WFDint *attribList) WFD_APIEXIT;
WFD_API_CALL void WFD_APIENTRY
  wfdDestroyDestination(WFDDevice device, WFDDestination destination) WFD_APIEXIT;
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdBindDestinationToPort(WFDDevice device, WFDPort port, WFDDestination destination, WFDTransition transition) WFD_APIEXIT;
#endif
typedef WFDDestination (WFD_APIENTRY PFNWFDCREATEDESTINATIONFROMIMAGE) (WFDDevice device, WFDPort port, WFDEGLImage image, const WFDint *attribList);
typedef WFDDestination (WFD_APIENTRY PFNWFDCREATEDESTINATIONFROMSTREAM) (WFDDevice device, WFDPort port, WFDNativeStreamType stream, const WFDint *attribList);
typedef void (WFD_APIENTRY PFNWFDDESTROYDESTINATION) (WFDDevice device, WFDDestination destination);
typedef void (WFD_APIENTRY PFNWFDBINDDESTINATIONTOPORT) (WFDDevice device, WFDPort port, WFDDestination destination, WFDTransition transition);
#endif

#ifndef WFD_pipeline_attrib_ext
#define WFD_pipeline_attrib_ext 1
#define WFD_PIPELINE_TYPE                   0x7740
#define WFD_PIPELINE_POSSIBLE_PORTS         0x7741 /* list of port ids */
#define WFD_PIPELINE_PIXEL_FORMATS          0x7742
/**
 * CRC capture
 * Note: Returned CRC value corresponds to the frame for which the last v-sync
 *       has fired. V-sync signal indicates that a frame has finished
 *       rendering to the physical display.
 * value[0] - CRC validity
 *            0: CRC value NOT valid
 *            1: CRC value valid
 * value[1] - CRC value lower 32 bits
 * value[2] - CRC value upper 32 bits
 */
#define WFD_PIPELINE_CRC                    0x7743
//#define WFD_PIPELINE_HW_BLEND_SETTINGS      0x7744 /* See below @ WFD_pipeline_get_hw_blend_settings */
#define WFD_PIPELINE_VIRTUAL_PIPE_ID        0x7745
#define WFD_PIPELINE_PIXEL_FORMATS_COUNT    0x7746
#endif

#ifndef WFD_bchs_filter
#define WFD_bchs_filter 1
#define WFD_PIPELINE_BRIGHTNESS        0x7750
#define WFD_PIPELINE_CONTRAST          0x7751
#define WFD_PIPELINE_HUE               0x7752
#define WFD_PIPELINE_SATURATION        0x7753
#endif

#ifndef WFD_pipeline_color_space
#define WFD_pipeline_color_space 1
#define WFD_PIPELINE_COLOR_SPACE       0x77A0

#define WFD_COLOR_SPACE_UNCORRECTED    0x0
#define WFD_COLOR_SPACE_SRGB           0x1
#define WFD_COLOR_SPACE_LRGB           0x2
#define WFD_COLOR_SPACE_BT601          0x3
#define WFD_COLOR_SPACE_BT601_FULL     0x4
#define WFD_COLOR_SPACE_BT709          0x5
#define WFD_COLOR_SPACE_BT709_FULL     0x6
#endif

#ifndef WFD_source_translation_mode
#define WFD_source_translation_mode 1
#define WFD_SOURCE_TRANSLATION_MODE    0x7900
typedef enum {
  WFD_SOURCE_TRANSLATION_UNSECURED,
  WFD_SOURCE_TRANSLATION_SECURED,
  WFD_SOURCE_TRANSLATION_DIRECT_UNSECURED,
  WFD_SOURCE_TRANSLATION_DIRECT_SECURED,
  WFD_SOURCE_TRANSLATION_MAX,
  WFD_SOURCE_TRANSLATION_FORCE_32BIT = 0x7FFFFFFF
} WFDTranslationMode;
#endif

#ifndef WFD_egl_images
#define WFD_egl_images 1
#define WFD_USAGE_DISPLAY             (1 << 0)
#define WFD_USAGE_READ                (1 << 1)
#define WFD_USAGE_WRITE               (1 << 2)
#define WFD_USAGE_NATIVE              (1 << 3)
#define WFD_USAGE_OPENGL_ES1          (1 << 4)
#define WFD_USAGE_OPENGL_ES2          (1 << 5)
#define WFD_USAGE_OPENGL_ES3          (1 << 11)
#define WFD_USAGE_OPENVG              (1 << 6)
#define WFD_USAGE_VIDEO               (1 << 7)
#define WFD_USAGE_CAPTURE             (1 << 8)
#define WFD_USAGE_ROTATION            (1 << 9)
#define WFD_USAGE_OVERLAY             (1 << 10)
#define WFD_USAGE_COMPRESSION         (1 << 12)
#define WFD_USAGE_WRITEBACK           (1 << 31)
#define WFD_FORMAT_BYTE               1
#define WFD_FORMAT_RGBA4444           2
#define WFD_FORMAT_RGBX4444           3
#define WFD_FORMAT_RGBA5551           4
#define WFD_FORMAT_RGBX5551           5
#define WFD_FORMAT_RGB565             6
#define WFD_FORMAT_RGB888             7
#define WFD_FORMAT_RGBA8888           8
#define WFD_FORMAT_RGBX8888           9
#define WFD_FORMAT_YVU9               10
#define WFD_FORMAT_YUV420             11
#define WFD_FORMAT_NV12               12
#define WFD_FORMAT_YV12               13
#define WFD_FORMAT_UYVY               14
#define WFD_FORMAT_YUY2               15
#define WFD_FORMAT_YVYU               16
#define WFD_FORMAT_V422               17
#define WFD_FORMAT_AYUV               18
#define WFD_FORMAT_NV16               19
#define WFD_FORMAT_RGBX1010102        20
#define WFD_FORMAT_RGBA1010102        21
#define WFD_FORMAT_BGRX1010102        22
#define WFD_FORMAT_BGRA1010102        23
#define WFD_FORMAT_XBGR2101010        24
#define WFD_FORMAT_XRGB2101010        25
#define WFD_FORMAT_ARGB2101010        26
#define WFD_FORMAT_ABGR2101010        27
#define WFD_FORMAT_P010               28
#define WFD_FORMAT_TP10               29
#ifndef WFD_FORMAT_NV12_QC_SUPERTILE
#define WFD_FORMAT_NV12_QC_SUPERTILE  ((1 << 16) | WFD_FORMAT_NV12)
#endif
#ifndef WFD_FORMAT_NV12_QC_32M4KA
#define WFD_FORMAT_NV12_QC_32M4KA     ((2 << 16) | WFD_FORMAT_NV12)
#endif

/* QTI extension definitions - start at 50 */
#define WFD_FORMAT_BGRA8888           50
#define WFD_FORMAT_BGRX8888           51
#define WFD_FORMAT_BGR565             52

#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdCreateWFDEGLImages(WFDDevice device, WFDint width, WFDint height, WFDint format, WFDint usage, WFDint count, WFDEGLImage *images, WFDint flags) WFD_APIEXIT;
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdDestroyWFDEGLImages(WFDDevice device, WFDint count, WFDEGLImage *images) WFD_APIEXIT;
#endif /* WFD_WFDEXT_PROTOTYPES */
typedef WFDErrorCode (WFD_APIENTRY PFNWFDCREATEWFDEGLIMAGES) (WFDDevice device, WFDint width, WFDint height, WFDint usage, WFDint count, WFDEGLImage *images, WFDint flags);
typedef WFDErrorCode (WFD_APIENTRY PFNWFDDESTROYWFDEGLIMAGES) (WFDDevice device, WFDint count, WFDEGLImage *images);
typedef struct
{
  WFDuint32 width;
  WFDuint32 height;
  WFDuint32 format;
  WFDuint32 usage;
  WFDuint32 flags;
  WFDuint32 fd;
  WFDuint64 offset;
  WFDuint32 size;
  WFDuint32 padding;
  WFDuint64 paddr;
  WFDuint32 strides[2];
  WFDuint64 vaddr;
  WFDuint64 cvaddr;
  WFDuint64 dvaddr;
  WFDuint32 planar_offsets[3];
  WFDuint64 pages;
  WFDuint32 addr_alignment;
  WFDuint64 image_handle;
  WFDuint32 buffer_allocator;
  WFDuint64 shmem_id;
  WFDuint32 shmem_type;
} WFD_EGLImageType;
#endif

#ifndef WFD_display_buffer_capture
#define WFD_display_buffer_capture 1
#define WFD_BUFFER_ATTRIB_COMPRESSED        (1<<(0))
#define WFD_BUFFER_ATTRIB_PROTECTED         (1<<(1))
#define WFD_BUFFER_ATTRIB_FLIPPED           (1<<(2))
#define WFD_BUFFER_ATTRIB_MIRRORED          (1<<(3))
#define WFD_BUFFER_ATTRIB_RETURNED_HANDLE   (1<<(4))

typedef struct {
  void               *buffer;
  WFDint              size;
  WFDint              strides[2];
  WFDint              source_height;
  WFDint              source_width;
  WFDRect             source_rectangle;
  WFDRect             destination_rectangle;
  WFDint              pixel_format;
  WFDint              planar_offsets[3];
  WFDint              transparency;
  WFDint              global_alpha;
  WFDint              attribute_mask;
  WFDint              layer_order;
} WFD_DisplayBufferInfoType;

typedef struct {
  WFDint              width;
  WFDint              height;
  WFDint              pixel_format;
  WFDint              background_colour;
  WFDint              num_buffers_attached;
  WFDint              first_buffer_info_offset;
} WFD_DisplayInfoType;

typedef enum {
  WFD_DISPCOMP_ATTRIB_NONE          = 0x00,
  WFD_DISPCOMP_ATTRIB_RETURN_VA,
  WFD_DISPCOMP_ATTRIB_RETURN_HANDLE,
  WFD_DISPCOMP_ATTRIB_FORCE_32BIT   = 0x7FFFFFFF
} WFD_DisplayCompositionAttribs;

#ifdef WFD_WFDEXT_PROTOTYPES
WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdGetDisplayCompositionInfo(WFDDevice device, WFDint *size, WFDuint8 *info, WFDint *numActiveDisplays, WFDint attrib) WFD_APIEXIT;

WFD_API_CALL WFDErrorCode WFD_APIENTRY
  wfdSetLockState(WFDDevice device, WFDboolean lock) WFD_APIEXIT;
#endif

typedef WFDErrorCode (WFD_APIENTRY PFNWFDGETDISPLAYCOMPOSITIONINFO) (WFDDevice device, WFDint *size, WFDuint8 *info, WFDint *numActiveDisplays);
typedef WFDErrorCode (WFD_APIENTRY PFNWFDSETLOCKSTATE) (WFDDevice device, WFDboolean lock);

#endif /* #ifndef WFD_display_buffer_capture */

#ifndef WFD_pipeline_get_hw_blend_settings
/**
 * This extension provides a way for a privileged client to query the blend
 * settings directly from the HW corresponding to the pipeline associated to the
 * given pipeline handle.
 *
 * Used with wfdGetPipelineAttribiv(): Obtain a value array that matches the
 * layout of WFD_PipelineBlendSettingsType.
 * Attribute count is WFD_PIPELINE_HW_BLEND_SETTINGS_COUNT.
 */
#define WFD_pipeline_get_hw_blend_settings 1

#define WFD_PIPELINE_HW_BLEND_SETTINGS  0x7744

/**
 * Blend settings read directly from HW rather than SW contexts
 * Note: arrays of 2 elements in case of split DSI scenario
 */
typedef struct
{
  WFDuint32 dual_mixer_cfg;           /* Whether layer is in a dual mixer path */
  WFDuint32 cur_blend_stage[2];       /* Current blend stage on primary(0)/secondary(1) mixer */
                                      /* Note: A cur_blend_stage value higher than 
                                       *       the max_blend_stage indicates that
                                       *       the pipeline is not attached to a
                                       *       primary(0)/secondary(1) mixer
                                       */
  WFDuint32 max_blend_stage;          /* Maximum/upper most blend stage of a mixer */
  WFDuint32 blend_operation[2];       /* Blend operation on primary(0)/secondary(1) mixer */
  WFDuint32 alpha_value[2];           /* Pipeline constant alpha value on primary(0)/secondary(1) mixer */
  WFDuint32 post_process_enabled[2];  /* DSPP enable state on primary(0)/secondary(1) mixer path */
  WFDuint32 interface_id;             /* Display to which this pipeline is attached */
} WFD_PipelineHwBlendSettingsType;

#define WFD_PIPELINE_HW_BLEND_SETTINGS_COUNT \
  (sizeof(WFD_PipelineHwBlendSettingsType)/sizeof(WFDuint32))

#endif /* #ifndef WFD_pipeline_get_hw_blend_settings */


#ifdef __cplusplus
}
#endif

#endif /* #ifndef __wfdext2_h_ */

