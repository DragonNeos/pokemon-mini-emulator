#include "PmLCD.h"

PmLCD::PmLCD(PmSystem *pSystem)
    : PmUnit(pSystem)
    , m_changed(GS_FALSE)
    , m_pData(GS_NULL)
    , m_pScreen0(GS_NULL)
    , m_pScreen1(GS_NULL)
{
    m_pData = GS_MALLOCZ_OBJ_N(GS_BYTE, DATA_WIDTH * ((DATA_HEIGHT + 7) / 8));
    RETURN_ON_FAIL(GS_NULL != m_pData);

    m_pScreen0 = GS_MALLOCZ_OBJ_N(GS_BYTE, PmBase::PM_SCREEN_WIDTH * PmBase::PM_SCREEN_HEIGHT);
    RETURN_ON_FAIL(GS_NULL != m_pScreen0);

    m_pScreen1 = GS_MALLOCZ_OBJ_N(GS_BYTE, PmBase::PM_SCREEN_WIDTH * PmBase::PM_SCREEN_HEIGHT);
    RETURN_ON_FAIL(GS_NULL != m_pScreen1);

    SetReady();
}

PmLCD::~PmLCD()
{
    SafeFreePointer(m_pData);
    SafeFreePointer(m_pScreen0);
    SafeFreePointer(m_pScreen1);
}

GS_VOID PmLCD::Reset(GS_BOOL /*hardReset*/)
{
    gs_zero(m_pData, DATA_WIDTH * ((DATA_HEIGHT + 7) / 8));
    gs_zero(m_pScreen0, PmBase::PM_SCREEN_WIDTH * PmBase::PM_SCREEN_HEIGHT);
    gs_zero(m_pScreen1, PmBase::PM_SCREEN_WIDTH * PmBase::PM_SCREEN_HEIGHT);

    m_state.Reset();
    m_state.ucContrastLevel = 0x20;
    SetContrast(m_state.ucContrastLevel);
}

GS_BYTE PmLCD::ReadReg(GS_BYTE reg)
{
    GS_ASSERT(!m_state.bSetContrast);

    // 0xFE to 0xFF
    switch (reg)
    {
    case 0xFE: // Read from LCD with Control Activated
        return ReadCtrl();
    case 0xFF: // Read from LCD with Control Desactivated
        return Read();
    default:   // Unused
        GS_ASSERT(GS_FALSE);
        return 0x00;
    }
}

GS_VOID PmLCD::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    if (m_state.bSetContrast)
    {
        m_state.bSetContrast = GS_FALSE;
        m_state.ucContrastLevel = val & 0x3F;
        SetContrast(m_state.ucContrastLevel);
        return;
    }

    // 0xFE to 0xFF
    switch (reg)
    {
    case 0xFE: // Write to LCD with Control Activated
        WriteCtrl(val);
        return;
    case 0xFF: // Write to LCD with Control Desactivated
        Write(val);
        return;
    default:   // Unused
        GS_ASSERT(GS_FALSE);
        return;
    }
}

GS_VOID PmLCD::CopyFrameBuffer(const GS_BYTE *pFrameBuffer)
{
    const GS_BYTE *pSrc = pFrameBuffer;
    GS_BYTE *pDst = m_pData;
    if (m_state.bColumnReverse)
    {
        for (GS_INT page = 0; page < PmBase::PM_SCREEN_HEIGHT / 8; ++page)
        {
            pDst += PmBase::PM_SCREEN_WIDTH;
            for (GS_INT w = 0; w < PmBase::PM_SCREEN_WIDTH; ++w) { *(--pDst) = *(pSrc++); }
            pDst += DATA_WIDTH;
        }
    }
    else
    {
        for (GS_INT page = 0; page < PmBase::PM_SCREEN_HEIGHT / 8; ++page)
        {
            memcpy(pDst, pSrc, PmBase::PM_SCREEN_WIDTH);
            pSrc += PmBase::PM_SCREEN_WIDTH;
            pDst += DATA_WIDTH;
        }
    }

    m_state.ucPage = PmBase::PM_SCREEN_HEIGHT / 8 - 1;
    m_state.ucColumn = PmBase::PM_SCREEN_WIDTH;
    m_state.bRequireDummyRead = 1;
    m_state.bRmwMode = 0;
    m_changed = GS_TRUE;
}

GS_VOID PmLCD::RefreshScreen()
{
    RETURN_ON_FAIL(m_changed);
    m_changed = GS_FALSE;

    // Only support screen average by now
    gs_swap(m_pScreen0, m_pScreen1);

    if (!m_state.bDisplayOn)
    {
        gs_zero(m_pScreen0, PmBase::PM_SCREEN_WIDTH * PmBase::PM_SCREEN_HEIGHT);
        return;
    }

    GS_INT row = 0;
    GS_INT page = 0;
    GS_INT move = 0;
    GS_BYTE digital = 0;
    for (GS_INT h = 0; h < PmBase::PM_SCREEN_HEIGHT; ++h)
    {
        row = (m_state.ucStartRow + h) % PmBase::PM_SCREEN_HEIGHT;
        if (m_state.bRowReverse) { row = PmBase::PM_SCREEN_HEIGHT - 1 - row; }
        page = row >> 3;
        move = row & 7;
        for (GS_INT w = 0; w < PmBase::PM_SCREEN_WIDTH; ++w)
        {
            digital = (m_pData[page * DATA_WIDTH + w] >> move) & 1;
            digital ^= m_state.bInvertAllPixels;
            digital |= m_state.bSetAllPixels;
            m_pScreen0[h * PmBase::PM_SCREEN_WIDTH + w] = digital;
        }
    }
}

GS_VOID PmLCD::RenderScreen(GS_BYTE *pScreen, GS_UINT rowBytes) const
{
    // Only support screen average by now
    const GS_BYTE *pSrc0 = m_pScreen0;
    const GS_BYTE *pSrc1 = m_pScreen1;
    GS_BYTE *pDst = pScreen;
    GS_INT rowSkip = rowBytes - PmBase::PM_SCREEN_WIDTH;
    GS_BYTE midIntensity = (m_state.ucMinIntensity + m_state.ucMaxIntensity) / 2;
    for (GS_INT h = 0; h < PmBase::PM_SCREEN_HEIGHT; ++h)
    {
        for (GS_INT w = 0; w < PmBase::PM_SCREEN_WIDTH; ++w)
        {
            switch (*(pSrc0++) + *(pSrc1++))
            {
            case 0:
                *(pDst++) = m_state.ucMinIntensity;
                break;
            case 1:
                *(pDst++) = midIntensity;
                break;
            case 2:
                *(pDst++) = m_state.ucMaxIntensity;
                break;
            default:
                GS_ASSERT(GS_FALSE);
            }
        }

        pDst += rowSkip;
    }
}

GS_BYTE PmLCD::ReadCtrl()
{
    // Get status
    return 0x40 | (m_state.bDisplayOn? 0x20: 0x00);
}

GS_BYTE PmLCD::Read()
{
    // Get pixel
    if (m_state.bRequireDummyRead)
    {
        m_state.bRequireDummyRead = 0;
        return 0;
    }

    GS_UINT addr = m_state.ucPage * DATA_WIDTH + (m_state.bColumnReverse? (DATA_WIDTH - 1 - m_state.ucColumn): m_state.ucColumn);
    GS_BYTE val = m_pData[addr];
    if (m_state.ucPage >= 8) { val &= 0x01; }

    if (!m_state.bRmwMode)
    {
        m_state.ucColumn = (GS_BYTE)(gs_min(m_state.ucColumn + 1, DATA_WIDTH - 1));
        m_state.bRequireDummyRead = 1;
    }

    return val;
}

GS_VOID PmLCD::WriteCtrl(GS_BYTE val)
{
    switch (val)
    {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F:
        if (!m_state.bRmwMode)
        {
            m_state.ucColumn = (m_state.ucColumn & 0xF0) | (val & 0x0F);
            m_state.bRequireDummyRead = 1;
        }
        return;
    case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
        if (!m_state.bRmwMode)
        {
            m_state.ucColumn = (m_state.ucColumn & 0x0F) | ((val & 0x0F) << 4);
            m_state.bRequireDummyRead = 1;
        }
        return;
    case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
        // Do nothing?
        return;
    case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
        // Modify LCD voltage? (2F Default)
        // 0x28 = Blank
        // 0x29 = Blank
        // 0x2A = Blue screen then blank
        // 0x2B = Blank
        // 0x2C = Blank
        // 0x2D = Blank
        // 0x2E = Blue screen (overpower?)
        // 0x2F = Normal
        // User shouldn't mess with this ones as may damage the LCD
        return;
    case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        // Do nothing?
        return;
    case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F:
        // Set starting LCD scanline (cause warp around)
        m_state.ucStartRow = val - 0x40;
        m_changed = GS_TRUE;
        return;
    case 0x80:
        // Do nothing?
        return;
    case 0x81:
        // Set contrast at the next write
        m_state.bSetContrast = GS_TRUE;
        return;
    case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
        // Do nothing?
        return;
    case 0xA0:
        // Segment Driver Direction Select: Normal
        m_state.bColumnReverse = 0;
        m_changed = GS_TRUE;
        return;
    case 0xA1:
        // Segment Driver Direction Select: Reverse
        m_state.bColumnReverse = 1;
        m_changed = GS_TRUE;
        return;
    case 0xA2:
        // Max Contrast: Disable
        SetContrast(m_state.ucContrastLevel);
        return;
    case 0xA3:
        // Max Contrast: Enable
        SetContrast(0x3F);
        return;
    case 0xA4:
        // Set All Pixels: Disable
        m_state.bSetAllPixels = 0;
        m_changed = GS_TRUE;
        return;
    case 0xA5:
        // Set All Pixels: Enable
        m_state.bSetAllPixels = 1;
        m_changed = GS_TRUE;
        return;
    case 0xA6:
        // Invert All Pixels: Disable
        m_state.bInvertAllPixels = 0;
        m_changed = GS_TRUE;
        return;
    case 0xA7:
        // Invert All Pixels: Enable
        m_state.bInvertAllPixels = 1;
        m_changed = GS_TRUE;
        return;
    case 0xA8: case 0xA9: case 0xAA: case 0xAB:
        // Do nothing!?
        return;
    case 0xAC: case 0xAD:
        // User shouldn't mess with this ones as may damage the LCD
        return;
    case 0xAE:
        // Display Off
        m_state.bDisplayOn = 0;
        m_changed = GS_TRUE;
        return;
    case 0xAF:
        // Display On
        m_state.bDisplayOn = 1;
        m_changed = GS_TRUE;
        return;
    case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
    case 0xB8:
        // Set page (0-8, each page is 8px high)
        m_state.ucPage = val & 0x0F;
        return;
    case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
        // uh... do nothing?
        return;
    case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xC7:
        // Display rows from top to bottom as 0 to 63
        m_state.bRowReverse = 0;
        m_changed = GS_TRUE;
        return;
    case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF:
        // Display rows from top to bottom as 63 to 0
        m_state.bRowReverse = 1;
        m_changed = GS_TRUE;
        return;
    case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD6: case 0xD7:
    case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
        // Do nothing?
        return;
    case 0xE0:
        // Start "Read Modify Write"
        m_state.bRmwMode = 1;
        m_state.ucRmwColumn = m_state.ucColumn;
        return;
    case 0xE2:
        // Reset
        m_state.Reset();
        m_state.ucContrastLevel = 0x20;
        SetContrast(m_state.ucContrastLevel);
        m_state.bRequireDummyRead = 1;
        return;
    case 0xE3:
        // No operation
        return;
    case 0xEE:
        // End "Read Modify Write"
        m_state.bRmwMode = 0;
        m_state.ucColumn = m_state.ucRmwColumn;
        return;
    case 0xE1: case 0xE4: case 0xE5: case 0xE6: case 0xE7:
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEF:
        // User shouldn't mess with this ones as may damage the LCD
        return;
    case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7:
        // 0xF1 and 0xF5 freeze LCD and cause malfunction (need to power off the device to restore)
        // User shouldn't mess with this ones as may damage the LCD
        return;
    case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE: case 0xFF:
        // Contrast voltage control, FC = Default
        // User shouldn't mess with this ones as may damage the LCD
        return;
    }
}

GS_VOID PmLCD::Write(GS_BYTE val)
{
    // Set pixel
    GS_UINT addr = m_state.ucPage * DATA_WIDTH + (m_state.bColumnReverse? (DATA_WIDTH - 1 - m_state.ucColumn): m_state.ucColumn);
    m_pData[addr] = val;
    m_state.ucColumn = (GS_BYTE)(gs_min(m_state.ucColumn + 1, DATA_WIDTH - 1));
    m_state.bRequireDummyRead = 1;
    m_changed = GS_TRUE;
}

GS_VOID PmLCD::SetContrast(GS_BYTE level)
{
    level &= 0x3F;
    if (level < 0x20)
    {
        m_state.ucMinIntensity = (level + 1) >> 1;
        m_state.ucMaxIntensity = 0x0F + (level << 3) - level;
    }
    else
    {
        level -= 0x20;
        m_state.ucMinIntensity = 0x17 + (level << 3) - level;
        m_state.ucMaxIntensity = 0xEF + ((level + 1) >> 1);
    }

    m_changed = GS_TRUE;
}
