#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMHARDREGMAP_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMHARDREGMAP_H__

#define PM_REG (m_pReg)

#define PMR_SYS_CTRL1       (PM_REG[0x00])
#define PMR_SYS_CTRL2       (PM_REG[0x01])
#define PMR_SYS_CTRL3       (PM_REG[0x02])

#define PMR_SEC_CTRL        (PM_REG[0x08])
#define PMR_SEC_CNT_LO      (PM_REG[0x09])
#define PMR_SEC_CNT_MID     (PM_REG[0x0A])
#define PMR_SEC_CNT_HI      (PM_REG[0x0B])

#define PMR_SYS_BATT        (PM_REG[0x10])

#define PMR_TMR1_SCALE      (PM_REG[0x18])
#define PMR_TMR1_OSC        (PM_REG[0x19])
#define PMR_TMR2_SCALE      (PM_REG[0x1A])
#define PMR_TMR2_OSC        (PM_REG[0x1B])
#define PMR_TMR3_SCALE      (PM_REG[0x1C])
#define PMR_TMR3_OSC        (PM_REG[0x1D])

#define PMR_IRQ_PRI1        (PM_REG[0x20])
#define PMR_IRQ_PRI2        (PM_REG[0x21])
#define PMR_IRQ_PRI3        (PM_REG[0x22])
#define PMR_IRQ_ENA1        (PM_REG[0x23])
#define PMR_IRQ_ENA2        (PM_REG[0x24])
#define PMR_IRQ_ENA3        (PM_REG[0x25])
#define PMR_IRQ_ENA4        (PM_REG[0x26])
#define PMR_IRQ_ACT1        (PM_REG[0x27])
#define PMR_IRQ_ACT2        (PM_REG[0x28])
#define PMR_IRQ_ACT3        (PM_REG[0x29])
#define PMR_IRQ_ACT4        (PM_REG[0x2A])

#define PMR_TMR1_CTRL_L     (PM_REG[0x30])
#define PMR_TMR1_CTRL_H     (PM_REG[0x31])
#define PMR_TMR1_PRE_L      (PM_REG[0x32])
#define PMR_TMR1_PRE_H      (PM_REG[0x33])
#define PMR_TMR1_PVT_L      (PM_REG[0x34])
#define PMR_TMR1_PVT_H      (PM_REG[0x35])
#define PMR_TMR1_CNT_L      (PM_REG[0x36])
#define PMR_TMR1_CNT_H      (PM_REG[0x37])

#define PMR_TMR2_CTRL_L     (PM_REG[0x38])
#define PMR_TMR2_CTRL_H     (PM_REG[0x39])
#define PMR_TMR2_PRE_L      (PM_REG[0x3A])
#define PMR_TMR2_PRE_H      (PM_REG[0x3B])
#define PMR_TMR2_PVT_L      (PM_REG[0x3C])
#define PMR_TMR2_PVT_H      (PM_REG[0x3D])
#define PMR_TMR2_CNT_L      (PM_REG[0x3E])
#define PMR_TMR2_CNT_H      (PM_REG[0x3F])

#define PMR_TMR256_CTRL     (PM_REG[0x40])
#define PMR_TMR256_CNT      (PM_REG[0x41])

#define PMR_REG_44          (PM_REG[0x44])
#define PMR_REG_45          (PM_REG[0x45])
#define PMR_REG_46          (PM_REG[0x46])
#define PMR_REG_47          (PM_REG[0x47])

#define PMR_TMR3_CTRL_L     (PM_REG[0x48])
#define PMR_TMR3_CTRL_H     (PM_REG[0x49])
#define PMR_TMR3_PRE_L      (PM_REG[0x4A])
#define PMR_TMR3_PRE_H      (PM_REG[0x4B])
#define PMR_TMR3_PVT_L      (PM_REG[0x4C])
#define PMR_TMR3_PVT_H      (PM_REG[0x4D])
#define PMR_TMR3_CNT_L      (PM_REG[0x4E])
#define PMR_TMR3_CNT_H      (PM_REG[0x4F])

#define PMR_REG_50          (PM_REG[0x50])
#define PMR_REG_51          (PM_REG[0x51])
#define PMR_KEY_PAD         (PM_REG[0x52])
#define PMR_REG_53          (PM_REG[0x53])
#define PMR_REG_54          (PM_REG[0x54])
#define PMR_REG_55          (PM_REG[0x55])

#define PMR_IO_DIR          (PM_REG[0x60])
#define PMR_IO_DATA         (PM_REG[0x61])
#define PMR_REG_62          (PM_REG[0x62])

#define PMR_AUD_CTRL        (PM_REG[0x70])
#define PMR_AUD_VOL         (PM_REG[0x71])

#define PMR_PRC_MODE        (PM_REG[0x80])
#define PMR_PRC_RATE        (PM_REG[0x81])
#define PMR_PRC_MAP_LO      (PM_REG[0x82])
#define PMR_PRC_MAP_MID     (PM_REG[0x83])
#define PMR_PRC_MAP_HI      (PM_REG[0x84])
#define PMR_PRC_SCROLL_Y    (PM_REG[0x85])
#define PMR_PRC_SCROLL_X    (PM_REG[0x86])
#define PMR_PRC_SPR_LO      (PM_REG[0x87])
#define PMR_PRC_SPR_MID     (PM_REG[0x88])
#define PMR_PRC_SPR_HI      (PM_REG[0x89])
#define PMR_PRC_CNT         (PM_REG[0x8A])

#define PMR_LCD_CTRL        (PM_REG[0xFE])
#define PMR_LCD_DATA        (PM_REG[0xFF])

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMHARDREGMAP_H__
