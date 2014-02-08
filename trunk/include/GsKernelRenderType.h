#ifndef __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELRENDERTYPE_H__
#define __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELRENDERTYPE_H__

#include "GsKernelBaseType.h"

enum GS_COLOR_CONFIG
{
    GS_COLOR_CONFIG_GRAY, 

    GS_COLOR_CONFIG_RGB_565, 
    GS_COLOR_CONFIG_ARGB_4444, 
    GS_COLOR_CONFIG_RGBA_4444, 

    GS_COLOR_CONFIG_RGB_888, 
    GS_COLOR_CONFIG_BGR_888, 

    GS_COLOR_CONFIG_ARGB_8888, 
    GS_COLOR_CONFIG_BGRA_8888, 
    GS_COLOR_CONFIG_RGBA_8888, 
    GS_COLOR_CONFIG_rgbA_8888, 
    GS_COLOR_CONFIG_BGRX_8888, 
    GS_COLOR_CONFIG_CMYK_8888, 

    GS_COLOR_CONFIG_UNKNOWN
};

struct GsRenderDevice
{
    GS_UINT         width;
    GS_UINT         height;
    GS_COLOR_CONFIG config;
    GS_UINT         rowBytes;
    GS_VOID *       pData;

    GsRenderDevice()
        : width(0)
        , height(0)
        , config(GS_COLOR_CONFIG_UNKNOWN)
        , rowBytes(0)
        , pData(GS_NULL)
    {}
};

#endif // __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELRENDERTYPE_H__
