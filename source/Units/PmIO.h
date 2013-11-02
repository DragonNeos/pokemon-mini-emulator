#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIO_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIO_H__

#include "System/PmUnit.h"

// Input/Output
class PmIO : public PmUnit
{
public:
    PmIO(PmSystem *pSystem, GS_BYTE *pReg, GS_BYTE *pEEPROM);
    virtual ~PmIO();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID SetRealTimeClock();
    GS_BOOL Rumbling() const { return m_rumbling; }

    enum IO_KEY
    {
        IO_KEY_A     = 0, 
        IO_KEY_B     = 1, 
        IO_KEY_C     = 2, 
        IO_KEY_UP    = 3, 
        IO_KEY_DOWN  = 4, 
        IO_KEY_LEFT  = 5, 
        IO_KEY_RIGHT = 6, 
        IO_KEY_POWER = 7, 

        IO_KEY_NONE
    };

    GS_VOID OnKeyUp(IO_KEY key);
    GS_VOID OnKeyDown(IO_KEY key);
    GS_VOID OnShock();

private:
    GS_BYTE MinxIO_IODataRead();
    GS_VOID MinxIO_IODataWrite();
    GS_BOOL MinxIO_EEPROM_REvent();
    GS_VOID MinxIO_EEPROM_WEvent(GS_BYTE bits);
    GS_VOID MinxIO_EEPROM_Write(GS_BYTE data);

private:
    // Interrupt table
    enum
    {
        PM_INTR_0F = 0x0F,     // IR Receiver
        PM_INTR_10 = 0x10,     // Shock Sensor
        PM_INTR_15 = 0x15,     // Power key
        PM_INTR_16 = 0x16,     // Right key
        PM_INTR_17 = 0x17,     // Left key
        PM_INTR_18 = 0x18,     // Down key
        PM_INTR_19 = 0x19,     // Up key
        PM_INTR_1A = 0x1A,     // C key
        PM_INTR_1B = 0x1B,     // B key
        PM_INTR_1C = 0x1C,     // A key
    };

    #define PM_IO_PULL_UPS    0x40    // Pull-up when direction create a Z

    #define PM_EEPROM_DAT     0x04
    #define PM_EEPROM_CLK     0x08

    enum // LISTEN_STATE
    {
        PM_EEPROM_IDLE   = 0, 
        PM_EEPROM_LISTEN = 1, 
    };

    enum // DATA_OPRATE_STATE
    {
        PM_EEPROM_DEVICE_ID  = 0,  // Device ID
        PM_EEPROM_ADDR_HIGH  = 1,  // High Address Byte
        PM_EEPROM_ADDR_LOW   = 2,  // Low Address Byte
        PM_EEPROM_WRITE_BYTE = 3,  // Write Byte
        PM_EEPROM_READ_BYTE  = 4,  // Read Byte
    };

    struct State
    {
        GS_BYTE EEPLastPins;
        GS_BYTE ListenState;
        GS_BYTE OperateState;
        GS_BYTE EEPData;
        GS_INT32 EEPBit;
        GS_UINT16 EEPAddress;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;

private:
    GS_BYTE * m_pReg;
    GS_BYTE * m_pEEPROM; // Electrically Erasable Programmable Read-Only Memory
    GS_BOOL m_rumbling;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIO_H__
