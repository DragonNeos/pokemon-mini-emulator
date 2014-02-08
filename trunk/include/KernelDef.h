#ifndef __KERNEL_BASELIBS_KERNELBASE_HEADER_KERNELDEF_H__
#define __KERNEL_BASELIBS_KERNELBASE_HEADER_KERNELDEF_H__

#include "GsKernelType.h"
#include <cstring> // for memset
#include <stdio.h> // for FILE

template <typename T>
inline T const & gs_max (T const &a, T const &b)
{
    return (a < b)? b: a;
}

template <typename T>
inline T const & gs_min (T const &a, T const &b)
{
    return (a < b)? a: b;
}

template <typename T>
inline GS_VOID gs_swap (T &a, T &b)
{
    T t = a; a = b; b = t;
}

//===========================================================================

inline GS_VOID gs_zero(GS_VOID *pBuffer, GS_SIZE_T size) {
    memset(pBuffer, 0, size);
}

//===========================================================================

#include <cstdlib>
#include <cstring>

#define GS_MALLOC  malloc
#define GS_FREE    free
#define GS_REALLOC realloc

inline void * GsMallocZ(GS_SIZE_T size)
{
    void *p = GS_MALLOC(size);
    if (GS_NULL != p)
    {
        memset(p, 0, size);
    }

    return p;
}

#define GS_MALLOCZ GsMallocZ

#define GS_MALLOC_OBJ(t)       ((t*)GS_MALLOC(sizeof(t)))
#define GS_MALLOC_OBJ_N(t, n)  ((t*)GS_MALLOC(sizeof(t) * (n)))
#define GS_MALLOCZ_OBJ(t)      ((t*)GS_MALLOCZ(sizeof(t)))
#define GS_MALLOCZ_OBJ_N(t, n) ((t*)GS_MALLOCZ(sizeof(t) * (n)))

template <typename T>
inline GS_VOID SafeFreePointer(T * &pointer)
{
    if (pointer)
    {
        GS_FREE(pointer);
        pointer = GS_NULL;
    }
}

template <typename T>
inline GS_VOID SafeDeletePointer(T * &pointer)
{
    if (pointer)
    {
        delete pointer;
        pointer = GS_NULL;
    }
}

template <typename T>
inline GS_VOID SafeDeletePointerArray(T * &pointer)
{
    if (pointer)
    {
        delete [] pointer;
        pointer = GS_NULL;
    }
}

class GsFreeScope
{
public:
    GsFreeScope(GS_VOID *pBuffer) : m_pBuffer(pBuffer) {}
    ~GsFreeScope() { SafeFreePointer(m_pBuffer); }
private:
    GS_VOID * m_pBuffer;
};

template <typename T>
class GsDeleteScope
{
public:
    GsDeleteScope(T pBuffer) : m_pBuffer(pBuffer) {}
    ~GsDeleteScope() { SafeDeletePointer(m_pBuffer); }
private:
    T m_pBuffer;
};

#define GS_FREE_SCOPE(pBuffer)   GsFreeScope                       pBuffer##Scope(pBuffer)
#define GS_DELETE_SCOPE(pBuffer) GsDeleteScope<GS_TYPEOF(pBuffer)> pBuffer##Scope(pBuffer)

//===========================================================================

#define DISALLOW_CONSTRUCT(TypeName) \
private: \
    TypeName();

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
private: \
    TypeName(const TypeName &); \
    TypeName & operator = (const TypeName &)

//===========================================================================

#include <cassert>

#ifdef _DEBUG
    #ifdef _WIN32
        #define GS_ASSERT(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), (unsigned int)__LINE__), 0) )
    #endif // _WIN32
    #ifdef _LINUX 
        #define GS_ASSERT assert
    #endif // _LINUX
#else
    #define GS_ASSERT(_Expression) ((void)0)
#endif // _DEBUG

//===========================================================================

#ifdef _WIN32
#pragma warning(disable: 4127) // 不报常量条件的警告
#pragma warning(disable: 4996) // 不报fopen的警告
#endif

//===========================================================================

#define RETURN_FAIL_ON_FAIL(ret) do \
{                                   \
    if (!(ret))                     \
    {                               \
        return GS_FALSE;            \
    }                               \
} while (0)

#define RETURN_NULL_ON_FAIL(ret) do \
{                                   \
    if (!(ret))                     \
    {                               \
        return GS_NULL;             \
    }                               \
} while (0)

#define RETURN_ON_FAIL(ret) do \
{                              \
    if (!(ret))                \
    {                          \
        return;                \
    }                          \
} while (0)

//===========================================================================

#endif // __KERNEL_BASELIBS_KERNELBASE_HEADER_KERNELDEF_H__
