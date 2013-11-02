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

enum GS_IMAGE_FORMAT
{
    GS_IMAGE_FORMAT_BMP, 
    GS_IMAGE_FORMAT_GIF, 
    GS_IMAGE_FORMAT_ICO, 
    GS_IMAGE_FORMAT_JPEG, 
    GS_IMAGE_FORMAT_PNG, 
    GS_IMAGE_FORMAT_TIFF, 
    GS_IMAGE_FORMAT_WEBP, 

    GS_IMAGE_FORMAT_UNKNOWN
};

struct GS_COLOR
{
    GS_BYTE a;
    GS_BYTE r;
    GS_BYTE g;
    GS_BYTE b;

    GS_COLOR(GS_BYTE _a = 0, GS_BYTE _r = 0, GS_BYTE _g = 0, GS_BYTE _b = 0)
        : a(_a), r(_r), g(_g), b(_b) {}
};

// 整数点
struct GS_POINT
{
    GS_INT x, y;

    GS_POINT(GS_INT _x = 0, GS_INT _y = 0) : x(_x) , y(_y) {}

    GS_BOOL    operator ==(const GS_POINT &rhs) const { return (x == rhs.x && y == rhs.y); }
    GS_BOOL    operator !=(const GS_POINT &rhs) const { return (x != rhs.x || y != rhs.y); }
    GS_POINT   operator + (const GS_POINT &rhs) const { return GS_POINT(x + rhs.x, y + rhs.y); }
    GS_POINT   operator - (const GS_POINT &rhs) const { return GS_POINT(x - rhs.x, y - rhs.y); }
    GS_POINT & operator +=(const GS_POINT &rhs)       { x += rhs.x; y += rhs.y; return *this; }
    GS_POINT & operator -=(const GS_POINT &rhs)       { x -= rhs.x; y -= rhs.y; return *this; }
};

// 整数向量
typedef GS_POINT GS_VECTOR;

// 整数矩形，左上闭，右下开
struct GS_RECT
{
    GS_INT left, top, right, bottom;

    GS_RECT(GS_INT _left = 0, GS_INT _top = 0, GS_INT _right = 0, GS_INT _bottom = 0)
        : left(_left), top(_top), right(_right), bottom(_bottom) {}

    GS_RECT(const GS_POINT &p0, const GS_POINT &p1)
        : left(p0.x), top(p0.y), right(p1.x), bottom(p1.y) {}

    GS_INT Width() const { return (right - left); }
    GS_INT Height() const { return (bottom - top); }

    GS_BOOL Valid() const { return (Width() >= 0 && Height() >= 0); }

    GS_POINT LeftTop() const { return GS_POINT(left, top); }

    GS_RECT Cross(const GS_RECT &rhs) const
    {
        return GS_RECT((left   > rhs.left  )? left:   rhs.left, 
                       (top    > rhs.top   )? top:    rhs.top, 
                       (right  < rhs.right )? right:  rhs.right, 
                       (bottom < rhs.bottom)? bottom: rhs.bottom);
    }

    GS_UINT Size() const { return Valid()? (GS_UINT)(Width() * Height()): 0; }

    GS_BOOL operator ==(const GS_RECT &rhs) const
    {
        return left   == rhs.left 
            && top    == rhs.top 
            && right  == rhs.right 
            && bottom == rhs.bottom;
    }

    GS_BOOL operator !=(const GS_RECT &rhs) const { return !(*this == rhs); }

    GS_RECT & operator +=(const GS_RECT &rhs)
    {
        if (left   > rhs.left  ) { left   = rhs.left; }
        if (top    > rhs.top   ) { top    = rhs.top; }
        if (right  < rhs.right ) { right  = rhs.right; }
        if (bottom < rhs.bottom) { bottom = rhs.bottom; }
        return *this;
    }
};

// 逆时针顺序的边界点，首尾为同一点，区域包含边界
struct GS_EDGE
{
    std::vector<GS_POINT> points;
    GS_BOOL continuous; // 是否是8连通连续点列

    GS_EDGE() : continuous(GS_FALSE) {}

    GS_EDGE & operator +=(const GS_POINT &rhs)
    {
        for (GS_UINT i = 0; i < points.size(); ++i) { points[i] += rhs; }
        return *this;
    }

    GS_EDGE operator +(const GS_POINT &rhs) const { GS_EDGE ret(*this); ret += rhs; return ret; }

    GS_EDGE & operator -=(const GS_POINT &rhs)
    {
        for (GS_UINT i = 0; i < points.size(); ++i) { points[i] -= rhs; }
        return *this;
    }

    GS_EDGE operator -(const GS_POINT &rhs) const { GS_EDGE ret(*this); ret -= rhs; return ret; }

    GS_VOID Clear()      { points.clear(); continuous = GS_FALSE; }
    GS_UINT Size() const { return points.size(); }

    GS_VOID AddPoint(const GS_POINT &point)      { points.push_back(point); }
    const GS_POINT & operator[](GS_UINT i) const { return points[i]; }
};

typedef GS_EDGE GS_POLYGON;

struct GS_MATRIX
{
    GS_INT m11;
    GS_INT m12;
    GS_INT m21;
    GS_INT m22;
    GS_INT dx;
    GS_INT dy;
    GS_INT denom;

    GS_MATRIX(GS_INT _m11 = 1, GS_INT _m12 = 0, GS_INT _m21 = 0, GS_INT _m22 = 1, 
              GS_INT _dx = 0, GS_INT _dy = 0, GS_INT _denom = 1)
        : m11(_m11) , m12(_m12), m21(_m21) , m22(_m22)
        , dx(_dx) , dy(_dy), denom(_denom)
    {}

    GS_VOID Trans(GS_INT x0, GS_INT y0, GS_INT *x1, GS_INT *y1)
    {
        *x1 = (m11 * x0 + m12 * y0 + dx + (denom >> 1)) / denom;
        *y1 = (m21 * x0 + m22 * y0 + dy + (denom >> 1)) / denom;
    }

    GS_VOID Trans(GS_POINT p0, GS_POINT *p1)
    {
        Trans(p0.x, p0.y, &(p1->x), &(p1->y));
    }

    GS_VOID Trans(GS_RECT r0, GS_RECT *r1)
    {
        Trans(r0.left, r0.top, &(r1->left), &(r1->top));
        Trans(r0.right, r0.bottom, &(r1->right), &(r1->bottom));
        if (r1->left > r1->right) { GS_INT t = r1->left; r1->left = r1->right; r1->right = t; }
        if (r1->top > r1->bottom) { GS_INT t = r1->top; r1->top = r1->bottom; r1->bottom = t; }
    }
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
