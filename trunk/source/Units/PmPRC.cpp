#include "PmPRC.h"
#include "Base/PmHardRegMap.h"

PmPRC::PmPRC(PmSystem *pSystem, GS_BYTE *pReg, GS_BYTE *pRam)
    : PmUnit(pSystem)
    , m_pReg(pReg)
    , m_pFrameBuffer(pRam)
    , m_pSpriteAttributes(pRam + 0x0300)
    , m_pTileMap(pRam + 0x0360)
{
    InitInvBitTable();
    SetReady();
}

PmPRC::~PmPRC()
{
}

GS_VOID PmPRC::Reset(GS_BOOL /*hardReset*/)
{
    m_state.Reset();
    m_state.ucRateMatch = 0x10;
    SysSetCpuStall(GS_FALSE);
}

GS_BYTE PmPRC::ReadReg(GS_BYTE reg)
{
    // 0x80 to 0x8F
    switch(reg)
    {
    case 0x80: // PRC Stage Control
        return PMR_PRC_MODE & 0x3F;
    case 0x81: // PRC Rate Control
        return PMR_PRC_RATE;
    case 0x82: // PRC Map Tile Base (Lo)
        return PMR_PRC_MAP_LO & 0xF8;
    case 0x83: // PRC Map Tile Base (Med)
        return PMR_PRC_MAP_MID;
    case 0x84: // PRC Map Tile Base (Hi)
        return PMR_PRC_MAP_HI & 0x1F;
    case 0x85: // PRC Map Vertical Scroll
        return PMR_PRC_SCROLL_Y & 0x7F;
    case 0x86: // PRC Map Horizontal Scroll
        return PMR_PRC_SCROLL_X & 0x7F;
    case 0x87: // PRC Map Sprite Base (Lo)
        return PMR_PRC_SPR_LO & 0xC0;
    case 0x88: // PRC Map Sprite Base (Med)
        return PMR_PRC_SPR_MID;
    case 0x89: // PRC Map Sprite Base (Hi)
        return PMR_PRC_SPR_HI & 0x1F;
    case 0x8A: // PRC Counter
        return m_state.uiCounter >> 24;
    default:   // Unused
        return 0;
    }
}

GS_VOID PmPRC::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    // 0x80 to 0x8F
    switch(reg)
    {
    case 0x80: // PRC Stage Control
        PMR_PRC_MODE = val & 0x3F;
        if (val & 0x08)
        {
            m_state.emMode = (val & 0x06)? 2: 1;
        }
        else
        {
            m_state.emMode = 0;
        }
        switch (val & 0x30)
        {
        case 0x00: m_state.ucMapTileWidth = 12; m_state.ucMapTileHeight = 16; break;
        case 0x10: m_state.ucMapTileWidth = 16; m_state.ucMapTileHeight = 12; break;
        case 0x20: m_state.ucMapTileWidth = 24; m_state.ucMapTileHeight =  8; break;
        case 0x30: m_state.ucMapTileWidth = 24; m_state.ucMapTileHeight = 16; break;
        }
        return;
    case 0x81: // PRC Rate Control
        if ((PMR_PRC_RATE & 0x0E) != (val & 0x0E)) PMR_PRC_RATE = (val & 0x0F);
        else PMR_PRC_RATE = (PMR_PRC_RATE & 0xF0) | (val & 0x0F);
        switch (val & 0x0E)
        {
        case 0x00: m_state.ucRateMatch = 0x20; break; // Rate /3
        case 0x02: m_state.ucRateMatch = 0x50; break; // Rate /6
        case 0x04: m_state.ucRateMatch = 0x80; break; // Rate /9
        case 0x06: m_state.ucRateMatch = 0xB0; break; // Rate /12
        case 0x08: m_state.ucRateMatch = 0x10; break; // Rate /2
        case 0x0A: m_state.ucRateMatch = 0x30; break; // Rate /4
        case 0x0C: m_state.ucRateMatch = 0x50; break; // Rate /6
        case 0x0E: m_state.ucRateMatch = 0x70; break; // Rate /8
        }
        return;
    case 0x82: // PRC Map Tile Base Low
        PMR_PRC_MAP_LO = val & 0xF8;
        m_state.uiMapTileBase = (m_state.uiMapTileBase & 0x1FFF00) | PMR_PRC_MAP_LO;
        return;
    case 0x83: // PRC Map Tile Base Middle
        PMR_PRC_MAP_MID = val;
        m_state.uiMapTileBase = (m_state.uiMapTileBase & 0x1F00F8) | (PMR_PRC_MAP_MID << 8);
        return;
    case 0x84: // PRC Map Tile Base High
        PMR_PRC_MAP_HI = val & 0x1F;
        m_state.uiMapTileBase = (m_state.uiMapTileBase & 0x00FFF8) | (PMR_PRC_MAP_HI << 16);
        return;
    case 0x85: // PRC Map Vertical Scroll
        PMR_PRC_SCROLL_Y = val & 0x7F;
        if (PMR_PRC_SCROLL_Y <= (m_state.ucMapTileHeight * 8 - PmBase::PM_SCREEN_HEIGHT))
        {
            m_state.ucMapPixelY = PMR_PRC_SCROLL_Y;
        }
        return;
    case 0x86: // PRC Map Horizontal Scroll
        PMR_PRC_SCROLL_X = val & 0x7F;
        if (PMR_PRC_SCROLL_X <= (m_state.ucMapTileWidth * 8 - PmBase::PM_SCREEN_WIDTH))
        {
            m_state.ucMapPixelX = PMR_PRC_SCROLL_X;
        }
        return;
    case 0x87: // PRC Sprite Tile Base Low
        PMR_PRC_SPR_LO = val & 0xC0;
        m_state.uiSpriteBase = (m_state.uiSpriteBase & 0x1FFF00) | PMR_PRC_SPR_LO;
        return;
    case 0x88: // PRC Sprite Tile Base Middle
        PMR_PRC_SPR_MID = val;
        m_state.uiSpriteBase = (m_state.uiSpriteBase & 0x1F00C0) | (PMR_PRC_SPR_MID << 8);
        return;
    case 0x89: // PRC Sprite Tile Base High
        PMR_PRC_SPR_HI = val & 0x1F;
        m_state.uiSpriteBase = (m_state.uiSpriteBase & 0x00FFC0) | (PMR_PRC_SPR_HI << 16);
        return;
    case 0x8A: // PRC Counter
        return;
    }
}

GS_VOID PmPRC::Sync(GS_UINT32 cycles)
{
    // Process PRC Counter
    m_state.uiCounter += (cycles * 0x01000000 / PmBase::PM_CPU_RATE) * PM_PRC_RATE;
    if ((PMR_PRC_RATE & 0xF0) >= m_state.ucRateMatch)
    {
        // Active frame
        if (m_state.uiCounter < 0x18000000)
        {
            // CPU Time
            m_state.emState = 0;
        }
        else if ((m_state.uiCounter & 0xFF000000) == 0x18000000)
        {
            // PRC BG&SPR Trigger
            if (m_state.emState == 1) { return; }
            if (m_state.emMode == 2)
            {
                SysSetCpuStall(GS_TRUE);
                Render();
                m_state.emState = 1;
            }
        }
        else if ((m_state.uiCounter & 0xFF000000) == 0x39000000)
        {
            // PRC Copy Trigger
            if (m_state.emState == 2) { return; }
            if (m_state.emMode)
            {
                SysSetCpuStall(GS_TRUE);
                SysCopyFrameBufferToLcd(m_pFrameBuffer);
                SysSetIrqAct(PM_INTR_03);
                m_state.emState = 2;
            }
        }
        else if (m_state.uiCounter >= 0x42000000)
        {
            // End-of-frame
            SysSetCpuStall(GS_FALSE);
            PMR_PRC_RATE &= 0x0F;
            m_state.uiCounter = 0x01000000;
            SysSetIrqAct(PM_INTR_04);
            SysRefreshLcd();
        }
    }
    else
    {
        // Non-active frame
        if (m_state.uiCounter >= 0x42000000)
        {
            PMR_PRC_RATE += 0x10;
            m_state.uiCounter = 0x01000000;
            SysRefreshLcd();
        }
    }
}

GS_VOID PmPRC::Render()
{
    // Draw background
    if (PMR_PRC_MODE & 0x02)
    {
        GS_BOOL needTileBelow = (0 != (m_state.ucMapPixelY & 7));
        GS_BYTE data = 0;
        GS_BYTE *pDst = m_pFrameBuffer;
        for (GS_INT y = 0; y < PmBase::PM_SCREEN_HEIGHT; y += 8)
        {
            GS_INT tileY = (m_state.ucMapPixelY + y) >> 3;
            GS_INT subY  = (m_state.ucMapPixelY + y) & 7;
            for (GS_INT x = 0; x < PmBase::PM_SCREEN_WIDTH; ++x)
            {
                GS_INT tileX = (m_state.ucMapPixelX + x) >> 3;
                GS_INT subX  = (m_state.ucMapPixelX + x) & 7;
                GS_INT mapAddr = tileY * m_state.ucMapTileWidth + tileX;
                GS_INT tileAddr = m_state.uiMapTileBase + m_pTileMap[mapAddr] * 8;
                data = SysRead(tileAddr + subX);

                if (needTileBelow)
                {
                    tileAddr = m_state.uiMapTileBase + m_pTileMap[mapAddr + m_state.ucMapTileWidth] * 8;
                    data = (data >> subY) | (SysRead(tileAddr + subX) << (8 - subY));
                }

                *(pDst++) = (PMR_PRC_MODE & 0x01)? ~data: data;
            }
        }
    }

    // Draw sprite
    if (PMR_PRC_MODE & 0x04)
    {
        const GS_BYTE *pSpriteAttr = m_pSpriteAttributes + 24 * 4;
        for (GS_UINT i = 0; i < 24; ++i)
        {
            GS_BYTE config = *(--pSpriteAttr);
            GS_UINT tilePos = *(--pSpriteAttr) * 8;
            GS_INT y = *(--pSpriteAttr) - 16;
            GS_INT x = *(--pSpriteAttr) - 16;
            if (0 == (config & 0x08)) { continue; }

            DrawSpriteTile(config, x + ((config & 0x01)? 8: 0), y + ((config & 0x02)? 8: 0), tilePos + 2, tilePos + 0);
            DrawSpriteTile(config, x + ((config & 0x01)? 8: 0), y + ((config & 0x02)? 0: 8), tilePos + 3, tilePos + 1);
            DrawSpriteTile(config, x + ((config & 0x01)? 0: 8), y + ((config & 0x02)? 8: 0), tilePos + 6, tilePos + 4);
            DrawSpriteTile(config, x + ((config & 0x01)? 0: 8), y + ((config & 0x02)? 0: 8), tilePos + 7, tilePos + 5);
        }
    }
}

GS_VOID PmPRC::InitInvBitTable()
{
    gs_zero(m_invBitTable, 0x100);
    for (GS_UINT i = 0; i < 0x100; ++i)
    {
        if (i & 0x01) { m_invBitTable[i] |= 0x80; }
        if (i & 0x02) { m_invBitTable[i] |= 0x40; }
        if (i & 0x04) { m_invBitTable[i] |= 0x20; }
        if (i & 0x08) { m_invBitTable[i] |= 0x10; }
        if (i & 0x10) { m_invBitTable[i] |= 0x08; }
        if (i & 0x20) { m_invBitTable[i] |= 0x04; }
        if (i & 0x40) { m_invBitTable[i] |= 0x02; }
        if (i & 0x80) { m_invBitTable[i] |= 0x01; }
    }
}

GS_VOID PmPRC::DrawSpriteTile(GS_BYTE config, GS_INT x, GS_INT y, GS_UINT drawTileIndex, GS_UINT maskTileIndex)
{
    RETURN_ON_FAIL(0 < x + 8 && x < PmBase::PM_SCREEN_WIDTH);
    RETURN_ON_FAIL(0 < y + 8 && y < PmBase::PM_SCREEN_HEIGHT);

    GS_BYTE *pDstRow = m_pFrameBuffer + (y >> 3) * PmBase::PM_SCREEN_WIDTH;
    GS_BYTE data = 0;
    GS_UINT subX = 0;
    GS_UINT subY = y & 7;
    GS_BYTE draw = 0;
    GS_BYTE mask = 0;

    // Process top columns
    if (0 <= y)
    {
        for (GS_UINT i = 0; i < 8; ++i)
        {
            if (0 <= x && x < PmBase::PM_SCREEN_WIDTH)
            {
                data = *(pDstRow + x);
                subX = (config & 0x01)? (7 - i): i;

                draw = SysRead(m_state.uiSpriteBase + drawTileIndex * 8 + subX);
                mask = SysRead(m_state.uiSpriteBase + maskTileIndex * 8 + subX);

                if (config & 0x02)
                {
                    draw = m_invBitTable[draw];
                    mask = m_invBitTable[mask];
                }

                if (config & 0x04)
                {
                    draw = ~draw;
                }

                data &= ((mask << subY) | (0xFF >> (8 - subY)));
                data |= (draw & ~mask) << subY;

                *(pDstRow + x) = data;
            }

            ++x;
        }

        x -= 8;
    }

    pDstRow += PmBase::PM_SCREEN_WIDTH;

    // Process bottom columns
    if (y + 8 < PmBase::PM_SCREEN_HEIGHT && 0 != subY)
    {
        for (GS_UINT i = 0; i < 8; ++i)
        {
            if (0 <= x && x < PmBase::PM_SCREEN_WIDTH)
            {
                data = *(pDstRow + x);
                subX = (config & 0x01)? (7 - i): i;

                draw = SysRead(m_state.uiSpriteBase + drawTileIndex * 8 + subX);
                mask = SysRead(m_state.uiSpriteBase + maskTileIndex * 8 + subX);

                if (config & 0x02)
                {
                    draw = m_invBitTable[draw];
                    mask = m_invBitTable[mask];
                }

                if (config & 0x04)
                {
                    draw = ~draw;
                }

                data &= ((mask >> (8 - subY)) | (0xFF << subY));
                data |= (draw & ~mask) >> (8 - subY);

                *(pDstRow + x) = data;
            }

            ++x;
        }
    }
}
