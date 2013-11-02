#include "PmSystem.h"
#include "Base/PmHardRegMap.h"

PmSystem::PmSystem()
    : m_pMem(GS_NULL)
    , m_pReg(GS_NULL)
    , m_pMapper(GS_NULL)
    , m_pCPU(GS_NULL)
    , m_pIO(GS_NULL)
    , m_pIRQ(GS_NULL)
    , m_pPRC(GS_NULL)
    , m_pLCD(GS_NULL)
    , m_pTimer(GS_NULL)
    , m_pAudio(GS_NULL)
    , m_lcdRefreshed(GS_FALSE)
{
}

PmSystem::~PmSystem()
{
    Destroy();
}

GS_BOOL PmSystem::Initialize(GS_BYTE *pRom, GS_UINT romSize, GS_BYTE *pSave)
{
    RETURN_FAIL_ON_FAIL(GS_NULL != pRom && 0x4000 <= romSize);

    m_pMem = new PmMemory();
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pMem);
    m_pMem->Init(pRom, romSize);

    m_pReg = m_pMem->reg;

    m_pMapper = PmMapper::Create(m_pMem, 0);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pMapper);

    m_pCPU = new PmCPU(this);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pCPU);
    RETURN_FAIL_ON_FAIL(m_pCPU->CheckReady());

    m_pIO = new PmIO(this, m_pReg, pSave);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pIO);
    RETURN_FAIL_ON_FAIL(m_pIO->CheckReady());

    m_pIRQ = new PmIRQ(this, m_pReg);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pIRQ);
    RETURN_FAIL_ON_FAIL(m_pIRQ->CheckReady());

    m_pPRC = new PmPRC(this, m_pReg, m_pMem->ram);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pPRC);
    RETURN_FAIL_ON_FAIL(m_pPRC->CheckReady());

    m_pLCD = new PmLCD(this);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pLCD);
    RETURN_FAIL_ON_FAIL(m_pLCD->CheckReady());

    m_pTimer = new PmTimer(this, m_pReg);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pTimer);
    RETURN_FAIL_ON_FAIL(m_pTimer->CheckReady());

    m_pAudio = new PmAudio(this, m_pReg);
    RETURN_FAIL_ON_FAIL(GS_NULL != m_pAudio);
    RETURN_FAIL_ON_FAIL(m_pAudio->CheckReady());

    memcpy(m_pMem->bios, PmBase::PM_FREE_BIOS, PmBase::PM_BIOS_SIZE);
    Reset(GS_TRUE);

    return GS_TRUE;
}

GS_VOID PmSystem::Reset(GS_BOOL hardReset)
{
    if (hardReset)
    {
        memset(m_pMem->ram, 0xFF, PmBase::PM_RAM_SIZE);
        memcpy(m_pMem->reg, PmBase::PM_REG_INIT, PmBase::PM_REG_SIZE);
    }

    m_pAudio->Reset(hardReset);
    m_pTimer->Reset(hardReset);
    m_pLCD->Reset(hardReset);
    m_pPRC->Reset(hardReset);
    m_pIRQ->Reset(hardReset);
    m_pIO->Reset(hardReset);
    m_pCPU->Reset(hardReset);

    m_lcdRefreshed = GS_FALSE;
    m_pIO->SetRealTimeClock();
}

GS_VOID PmSystem::Destroy()
{
    SafeDeletePointer(m_pAudio);
    SafeDeletePointer(m_pTimer);
    SafeDeletePointer(m_pLCD);
    SafeDeletePointer(m_pPRC);
    SafeDeletePointer(m_pIRQ);
    SafeDeletePointer(m_pIO);
    SafeDeletePointer(m_pCPU);
    SafeDeletePointer(m_pMapper);
    SafeDeletePointer(m_pMem);
}

GS_UINT PmSystem::EmulateFrame()
{
    GS_UINT totalCycles = 0;
    m_lcdRefreshed = GS_FALSE;

    while (!m_lcdRefreshed)
    {
        GS_UINT cycles = m_pCPU->Exec();
        m_pTimer->Sync(cycles);
        m_pPRC->Sync(cycles);
        m_pAudio->Sync(cycles);
        totalCycles += cycles;
    }

    return totalCycles;
}

GS_VOID PmSystem::Render(GS_BYTE *pScreen, GS_UINT rowBytes) const
{
    m_pLCD->RenderScreen(pScreen, rowBytes);
}

GS_UINT PmSystem::SoundDataCount() const
{
    return m_pAudio->SoundDataCount();
}

GS_VOID PmSystem::OutputSound(GS_BYTE *pBuffer, GS_UINT size)
{
    m_pAudio->OutputSound(pBuffer, size);
}

GS_BOOL PmSystem::Rumbling() const
{
    return m_pIO->Rumbling();
}

GS_VOID PmSystem::OnKeyUp(PM_KEY key)
{
    m_pIO->OnKeyUp(GetInnerKey(key));
}

GS_VOID PmSystem::OnKeyDown(PM_KEY key)
{
    m_pIO->OnKeyDown(GetInnerKey(key));
}

GS_VOID PmSystem::OnShock()
{
    m_pIO->OnShock();
}

PmIO::IO_KEY PmSystem::GetInnerKey(PM_KEY key) const
{
    switch (key)
    {
    case PM_KEY_A:
        return PmIO::IO_KEY_A;
    case PM_KEY_B:
        return PmIO::IO_KEY_B;
    case PM_KEY_C:
        return PmIO::IO_KEY_C;
    case PM_KEY_UP:
        return PmIO::IO_KEY_UP;
    case PM_KEY_DOWN:
        return PmIO::IO_KEY_DOWN;
    case PM_KEY_LEFT:
        return PmIO::IO_KEY_LEFT;
    case PM_KEY_RIGHT:
        return PmIO::IO_KEY_RIGHT;
    case PM_KEY_POWER:
        return PmIO::IO_KEY_POWER;
    case PM_KEY_NONE:
    default:
        return PmIO::IO_KEY_NONE;
    }
}

GS_BYTE PmSystem::Read(GS_UINT32 addr)
{
    if (0x002000 <= addr && addr < 0x002100)
    {
        // I/O Read
        GS_BYTE reg = addr & 0xFF;
        switch (reg)
        {
        // Misc interface
        case 0x00: // System Control 1
            return PMR_SYS_CTRL1;
        case 0x01: // System Control 2
            return PMR_SYS_CTRL2;
        case 0x02: // System Control 3
            return PMR_SYS_CTRL3;

        // IRQ interface
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x28: case 0x29: case 0x2A:
            return m_pIRQ->ReadReg(reg);

        // Timers interface
        case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x18: case 0x19: case 0x1A: case 0x1B:
        case 0x1C: case 0x1D: case 0x1E: case 0x1F:
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3A: case 0x3B:
        case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        case 0x40: case 0x41:
        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D: case 0x4E: case 0x4F:
            return m_pTimer->ReadReg(reg);

        // Parallel I/O interface & Power
        case 0x10:
        case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55:
        case 0x60: case 0x61: case 0x62:
            return m_pIO->ReadReg(reg);

        // Audio interface
        case 0x70: case 0x71:
            return m_pAudio->ReadReg(reg);

        // Program Rendering Chip interface
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8A: case 0x8B:
        case 0x8C: case 0x8D: case 0x8E: case 0x8F:
            return m_pPRC->ReadReg(reg);

        // LCD interface
        case 0xFE: case 0xFF:
            return m_pLCD->ReadReg(reg);

        // Open bus
        default:
            return 0x00;
        }
    }

    return m_pMapper->Read(addr);
}

GS_VOID PmSystem::Write(GS_UINT32 addr, GS_BYTE val)
{
    if (0x002000 <= addr && addr < 0x002100)
    {
        // I/O Write
        GS_BYTE reg = addr & 0xFF;
        switch(reg)
        {
        // Misc interface
        case 0x00: // System Control 1
            PMR_SYS_CTRL1 = val;
            return;
        case 0x01: // System Control 2
            PMR_SYS_CTRL2 = val;
            return;
        case 0x02: // System Control 3
            PMR_SYS_CTRL3 = val;
            return;

        // IRQ interface
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x28: case 0x29: case 0x2A:
            m_pIRQ->WriteReg(reg, val);
            return;

        // Timers interface
        case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x18: case 0x19: case 0x1A: case 0x1B:
        case 0x1C: case 0x1D: case 0x1E: case 0x1F:
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3A: case 0x3B:
        case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        case 0x40: case 0x41:
        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D: case 0x4E: case 0x4F:
            m_pTimer->WriteReg(reg, val);
            return;

        // Parallel I/O interface & Power
        case 0x10:
        case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55:
        case 0x60: case 0x61: case 0x62:
            m_pIO->WriteReg(reg, val);
            return;

        // Audio interface
        case 0x70: case 0x71:
            m_pAudio->WriteReg(reg, val);
            return;

        // Program Rendering Chip interface
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8A:
            m_pPRC->WriteReg(reg, val);
            return;

        // LCD interface
        case 0xFE: case 0xFF:
            m_pLCD->WriteReg(reg, val);
            return;

        default:
            return;
        }
    }

    m_pMapper->Write(addr, val);
}

GS_VOID PmSystem::SetCpuStall(GS_BOOL stall)
{
    m_pCPU->SetStall(stall);
}

GS_VOID PmSystem::CallIrq(GS_BYTE intr)
{
    m_pCPU->CallIrq(intr * 2);
}

GS_VOID PmSystem::SetIrq(GS_BOOL enable)
{
    m_pIRQ->SetEnable(enable);
    m_pIRQ->Process();
}

GS_VOID PmSystem::SetIrqAct(GS_BYTE intr)
{
    m_pIRQ->Set(intr);
    m_pIRQ->Process();
}

GS_VOID PmSystem::CopyFrameBufferToLcd(const GS_BYTE *pFrameBuffer)
{
    m_pLCD->CopyFrameBuffer(pFrameBuffer);
}

GS_VOID PmSystem::RefreshLcd()
{
    m_pLCD->RefreshScreen();
    m_lcdRefreshed = GS_TRUE;
}

GS_BOOL PmSystem::Timer3CheckPivot() const
{
    return m_pTimer->Timer3CheckPivot();
}
