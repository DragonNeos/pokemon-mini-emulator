#ifndef __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELBASETYPE_H__
#define __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELBASETYPE_H__

#include <vector>

typedef void               GS_VOID;
typedef bool               GS_BOOL;

typedef char               GS_CHAR;
typedef unsigned char      GS_BYTE;

typedef short              GS_SHORT;
typedef unsigned short     GS_USHORT;

typedef int                GS_INT;
typedef unsigned int       GS_UINT;

typedef long               GS_LONG;
typedef unsigned long      GS_ULONG;

typedef float              GS_FLOAT;
typedef double             GS_DOUBLE;

typedef GS_CHAR            GS_INT8;
typedef GS_BYTE            GS_UINT8;
typedef GS_SHORT           GS_INT16;
typedef GS_USHORT          GS_UINT16;
typedef GS_INT             GS_INT32;
typedef GS_UINT            GS_UINT32;

#ifdef _WIN32
typedef __int64            GS_INT64;
typedef unsigned __int64   GS_UINT64;
#else
typedef long long          GS_INT64;
typedef unsigned long long GS_UINT64;
#endif

#ifdef _ANDROID_LINUX
typedef GS_UINT16          GS_WCHAR;
#else
typedef wchar_t            GS_WCHAR;
#endif

typedef size_t             GS_SIZE_T;
typedef GS_INT64           GS_FILESIZE_T;

#define GS_NULL            0
#define GS_TRUE            true
#define GS_FALSE           false

#endif // __KERNEL_BASELIBS_KERNELBASE_EXPORT_GSKERNELBASETYPE_H__
