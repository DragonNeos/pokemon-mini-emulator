#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMREGMAP_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMREGMAP_H__

#define R_BA      (m_state.ba.W.W0)
#define R_B       (m_state.ba.B.B1)
#define R_A       (m_state.ba.B.B0)

#define R_IHL     (m_state.hl.I)
#define R_HL      (m_state.hl.W.W0)
#define R_I       (m_state.hl.B.B2)
#define R_H       (m_state.hl.B.B1)
#define R_L       (m_state.hl.B.B0)

#define R_IX      (m_state.x.I)
#define R_X       (m_state.x.W.W0)
#define R_XI      (m_state.x.B.B2)
#define R_X_H     (m_state.x.B.B1)
#define R_X_L     (m_state.x.B.B0)

#define R_IY      (m_state.y.I)
#define R_Y       (m_state.y.W.W0)
#define R_YI      (m_state.y.B.B2)
#define R_Y_H     (m_state.y.B.B1)
#define R_Y_L     (m_state.y.B.B0)

#define R_SP      (m_state.sp.W.W0)
#define R_SP_H    (m_state.sp.B.B1)
#define R_SP_L    (m_state.sp.B.B0)

#define R_PC      (m_state.pc.W.W0)
#define R_PC_V    (m_state.pc.B.B2)
#define R_PC_H    (m_state.pc.B.B1)
#define R_PC_L    (m_state.pc.B.B0)

#define R_OP      (m_state.op.I)
#define R_OP_WH   (m_state.op.W.W1)
#define R_OP_WL   (m_state.op.W.W0)
#define R_OP_BH   (m_state.op.B.B1)
#define R_OP_BL   (m_state.op.B.B0)

#define R_IN      (m_state.n.I)
#define R_N_I     (m_state.n.B.B2)
#define R_N       (m_state.n.B.B1)

#define R_U1      (m_state.u1)
#define R_U2      (m_state.u2)

#define R_E       (m_state.e)

#define R_IR      (m_state.ir)

#define R_F       (m_state.f)

#define Z_FLAG 0x01 // ZERO
#define C_FLAG 0x02 // CARRY
#define O_FLAG 0x04 // OVERFLOW
#define S_FLAG 0x08 // SIGN
#define B_FLAG 0x10 // BCD
#define N_FLAG 0x20 // NIBBLE
#define I_FLAG 0xC0 // INTFLG & INTOFF

#define Z_MOVE 0
#define C_MOVE 1
#define O_MOVE 2
#define S_MOVE 3
#define B_MOVE 4
#define N_MOVE 5

#define R_F_Z     ((R_F &   Z_FLAG) >> Z_MOVE)
#define SET_F_Z    (R_F |=  Z_FLAG)
#define CLEAR_F_Z  (R_F &= ~Z_FLAG)

#define R_F_C     ((R_F &   C_FLAG) >> C_MOVE)
#define SET_F_C    (R_F |=  C_FLAG)
#define CLEAR_F_C  (R_F &= ~C_FLAG)

#define R_F_O     ((R_F &   O_FLAG) >> O_MOVE)
#define SET_F_O    (R_F |=  O_FLAG)
#define CLEAR_F_O  (R_F &= ~O_FLAG)

#define R_F_S     ((R_F &   S_FLAG) >> S_MOVE)
#define SET_F_S    (R_F |=  S_FLAG)
#define CLEAR_F_S  (R_F &= ~S_FLAG)

#define R_F_B     ((R_F &   B_FLAG) >> B_MOVE)
#define SET_F_B    (R_F |=  B_FLAG)
#define CLEAR_F_B  (R_F &= ~B_FLAG)

#define R_F_N     ((R_F &   N_FLAG) >> N_MOVE)
#define SET_F_N    (R_F |=  N_FLAG)
#define CLEAR_F_N  (R_F &= ~N_FLAG)

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMREGMAP_H__
