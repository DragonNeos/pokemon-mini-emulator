#include "PmIO.h"
#include "Base/PmHardRegMap.h"
#include <time.h>

PmIO::PmIO(PmSystem *pSystem, GS_BYTE *pReg, GS_BYTE *pEEPROM)
    : PmUnit(pSystem)
    , m_pReg(pReg)
    , m_pEEPROM(pEEPROM)
    , m_rumbling(GS_FALSE)
{
    SetReady();
}

PmIO::~PmIO()
{
}

GS_VOID PmIO::Reset(GS_BOOL /*hardReset*/)
{
    m_state.Reset();
    m_state.ListenState = PM_EEPROM_IDLE;
    m_state.OperateState = PM_EEPROM_DEVICE_ID;

    // Init variables
    PMR_SYS_BATT &= 0x1F;
    PMR_KEY_PAD = 0xFF;
    m_rumbling = GS_FALSE;
}

GS_BYTE PmIO::ReadReg(GS_BYTE reg)
{
    // 0x10, 0x52, 0x60 to 0x62
    switch (reg)
    {
    case 0x10: // Low Battery
        return PMR_SYS_BATT;
    case 0x44: // Unknown
        return PMR_REG_44;
    case 0x45: // Unknown
        return PMR_REG_45;
    case 0x46: // Unknown
        return PMR_REG_46;
    case 0x47: // Unknown
        return PMR_REG_47;
    case 0x50: // Unknown (related to power management?)
        return PMR_REG_50;
    case 0x51: // Unknown (related to power management?)
        return PMR_REG_51;
    case 0x52: // Keypad
        return PMR_KEY_PAD;
    case 0x53: // Unknown
        return PMR_REG_53;
    case 0x54: // Unknown
        return PMR_REG_54;
    case 0x55: // Unknown
        return PMR_REG_55;
    case 0x60: // I/O Direction Select ( 0 = Input, 1 = Output )
        return PMR_IO_DIR;
    case 0x61: // I/O Data Register
        return MinxIO_IODataRead();
    case 0x62: // Unknown
        return PMR_REG_62;
    default:   // Unused
        return 0;
    }
}

GS_VOID PmIO::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    // 0x10, 0x52, 0x60 to 0x62
    switch(reg)
    {
    case 0x10: // Low Battery
        PMR_SYS_BATT = (PMR_SYS_BATT & 0x20) | (val & 0x1F);
        return;
    case 0x44: // Unknown
        PMR_REG_44 = val & 0xF7;
        return;
    case 0x45: // Unknown
        PMR_REG_45 = val & 0x0F;
        return;
    case 0x46: // Unknown
        PMR_REG_46 = val;
        return;
    case 0x47: // Unknown
        PMR_REG_47 = val & 0x0F;
        return;
    case 0x50: // Unknown (related to power management?)
        PMR_REG_50 = val;
        return;
    case 0x51: // Unknown (related to power management?)
        PMR_REG_51 = val & 0x03;
        return;
    case 0x52: // Keypad
        return;
    case 0x53: // Unknown
        PMR_REG_53 = 0x00;
    case 0x54: // Unknown
        PMR_REG_54 = val & 0x77;
    case 0x55: // Unknown
        PMR_REG_55 = val & 0x07;
    case 0x60: // I/O Direction Select ( 0 = Input, 1 = Output )
        PMR_IO_DIR = val;
        MinxIO_IODataWrite();
        return;
    case 0x61: // I/O Data Register
        PMR_IO_DATA = val;
        MinxIO_IODataWrite();
        return;
    case 0x62: // Unknown
        PMR_REG_62 = val & 0xF0;
        return;
    }
}

GS_VOID PmIO::SetRealTimeClock()
{
    time_t realTime = time(GS_NULL);
    tm *now = localtime(&realTime);
    GS_BYTE year     = (GS_BYTE)(now->tm_year % 100);
    GS_BYTE month    = (GS_BYTE)(now->tm_mon + 1);
    GS_BYTE day      = (GS_BYTE)(now->tm_mday);
    GS_BYTE hour     = (GS_BYTE)(now->tm_hour);
    GS_BYTE min      = (GS_BYTE)(now->tm_min);
    GS_BYTE sec      = (GS_BYTE)(now->tm_sec);
    GS_BYTE checksum = year + month + day + hour + min + sec;

    PMR_SYS_CTRL3 |= 0x02;
    m_pEEPROM[0x1FF6] = 0x00;
    m_pEEPROM[0x1FF7] = 0x00;
    m_pEEPROM[0x1FF8] = 0x00;
    m_pEEPROM[0x1FF9] = year;
    m_pEEPROM[0x1FFA] = month;
    m_pEEPROM[0x1FFB] = day;
    m_pEEPROM[0x1FFC] = hour;
    m_pEEPROM[0x1FFD] = min;
    m_pEEPROM[0x1FFE] = sec;
    m_pEEPROM[0x1FFF] = checksum;
}

GS_VOID PmIO::OnKeyUp(IO_KEY key)
{
    RETURN_ON_FAIL(IO_KEY_NONE != key);
    GS_BYTE keyValue = (GS_BYTE)(1 << key);
    PMR_KEY_PAD |= keyValue;
}

GS_VOID PmIO::OnKeyDown(IO_KEY key)
{
    RETURN_ON_FAIL(IO_KEY_NONE != key);
    GS_BYTE keyValue = (GS_BYTE)(1 << key);
    GS_BYTE irqIntr = (GS_BYTE)(0x1C - key); // PM_INTR_1C ~ PM_INTR_15
    if (PMR_KEY_PAD & keyValue)
    {
        SysSetIrqAct(irqIntr);
        PMR_KEY_PAD &= ~keyValue;
    }
}

GS_VOID PmIO::OnShock()
{
    SysSetIrqAct(PM_INTR_10);
}

GS_BYTE PmIO::MinxIO_IODataRead()
{
    GS_BYTE input = PM_IO_PULL_UPS;

    // Update all I/Os
    input = MinxIO_EEPROM_REvent()? PM_EEPROM_DAT: 0;

    return (PMR_IO_DATA & PMR_IO_DIR) | (input & ~PMR_IO_DIR);
}

GS_VOID PmIO::MinxIO_IODataWrite()
{
    GS_BYTE output = (PMR_IO_DATA & PMR_IO_DIR) | (PM_IO_PULL_UPS & ~PMR_IO_DIR);

    // Update all I/Os
    m_rumbling = (0 != (output & 0x10));
    MinxIO_EEPROM_WEvent(output);
}

GS_BOOL PmIO::MinxIO_EEPROM_REvent()
{
    // If it's Idle, return high...
    if (m_state.ListenState == PM_EEPROM_IDLE)
    {
        return GS_TRUE;
    }

    RETURN_FAIL_ON_FAIL(0 <= m_state.EEPBit && m_state.EEPBit < 8);

    // Process read
    if (m_state.OperateState == PM_EEPROM_READ_BYTE)
    {
        return (0 != ((m_pEEPROM[m_state.EEPAddress & 0x1FFF] >> m_state.EEPBit) & 1));
    }

    return GS_TRUE;
}

GS_VOID PmIO::MinxIO_EEPROM_WEvent(GS_BYTE bits)
{
    GS_BYTE rise = bits & ~m_state.EEPLastPins;
    GS_BYTE fall = ~bits & m_state.EEPLastPins;
    m_state.EEPLastPins = bits;

    // "Start" Command
    if ((fall & PM_EEPROM_DAT) && (bits & PM_EEPROM_CLK))
    {
        m_state.ListenState = PM_EEPROM_LISTEN;
        m_state.OperateState = PM_EEPROM_DEVICE_ID;
        m_state.EEPBit = 8;
        m_state.EEPData = 0x00;
        return;
    }

    // "Stop" Command
    if ((rise & PM_EEPROM_DAT) && (bits & PM_EEPROM_CLK)) {
        m_state.ListenState = PM_EEPROM_IDLE;
        return;
    }

    // If it's Idle, there's nothing to do...
    if (m_state.ListenState == PM_EEPROM_IDLE) return;

    // Process each bit on clock rise
    if (rise & PM_EEPROM_CLK) {
        GS_BYTE dat = (bits & PM_EEPROM_DAT)? 1: 0;
        if (m_state.EEPBit < 0) {
            MinxIO_EEPROM_Write(m_state.EEPData);
            m_state.EEPBit = 8;
            m_state.EEPData = 0x00;
        } else m_state.EEPData = (m_state.EEPData << 1) | dat;
    } else if (fall & PM_EEPROM_CLK) {
        m_state.EEPBit = m_state.EEPBit - 1;
    }
}

GS_VOID PmIO::MinxIO_EEPROM_Write(GS_BYTE data)
{
    switch (m_state.OperateState)
    {
    case PM_EEPROM_DEVICE_ID:
        if ((data & 0xF0) == 0xA0)
        {
            // EEPROM Device
            m_state.OperateState = (GS_BYTE)((data & 0x01)? PM_EEPROM_READ_BYTE: PM_EEPROM_ADDR_HIGH);
        }
        else
        {
            // Unknown Devide
            GS_ASSERT(GS_FALSE);
        }
        break;
    case PM_EEPROM_ADDR_HIGH:
        m_state.EEPAddress = (m_state.EEPAddress & 0x00FF) | (data << 8);
        m_state.OperateState = PM_EEPROM_ADDR_LOW;
        break;
    case PM_EEPROM_ADDR_LOW:
        m_state.EEPAddress = (m_state.EEPAddress & 0xFF00) | data;
        m_state.OperateState = PM_EEPROM_WRITE_BYTE;
        break;
    case PM_EEPROM_WRITE_BYTE:
        m_pEEPROM[m_state.EEPAddress & 0x1FFF] = data;
        m_state.EEPAddress++;
        break;
    case PM_EEPROM_READ_BYTE:
        m_state.EEPAddress++;
        break;
    }
}
