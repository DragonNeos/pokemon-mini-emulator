#include "GsPokeMiniApi.h"
#include "System/PmSystem.h"

GS_HANDLE GsCreatePokeMiniEmuHandle()
{
    return new PmSystem();
}

GS_VOID GsReleasePokeMiniEmuHandle(GS_HANDLE handle)
{
    IGsPmSystem *p = (IGsPmSystem *)handle;
    SafeDeletePointer(p);
}
