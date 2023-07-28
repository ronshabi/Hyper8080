#include "CPU.hpp"

inline u8 CPU::GetByte(u16 address) const { return m_memory.at(address); }

inline u16 CPU::GetWord(u16 address) const
{
    return static_cast<u16>(m_memory.at(address + 1) << 8) | static_cast<u16>(m_memory.at(address));
}

inline void CPU::SetByte(u16 address, u8 value) { m_memory[address] = value; }

inline void CPU::SetWord(u16 address, u16 value)
{
    m_memory[address] = value & 0xff;
    m_memory[address + 1] = (value >> 8) & 0xff;
}

inline void CPU::SetBC(u16 to)
{
    m_B = static_cast<u8>((to >> 8) & 0xff);
    m_C = static_cast<u8>(to & 0xff);
}
inline void CPU::SetDE(u16 to)
{
    m_D = static_cast<u8>((to >> 8) & 0xff);
    m_E = static_cast<u8>(to & 0xff);
}

inline void CPU::SetHL(u16 to)
{
    m_H = static_cast<u8>((to >> 8) & 0xff);
    m_L = static_cast<u8>(to & 0xff);
}

u8 CPU::FLAGS() const
{
    u8 ret = 0;
    ret |= (m_flagS << 7);
    ret |= (m_flagZ << 6);
    ret |= (m_flagAC << 4);
    ret |= (m_flagP << 2);
    ret |= (1 << 1);
    ret |= (m_flagC);
    return ret;
}

inline void CPU::SetFlagsZSP(u8 value)
{
    m_flagZ = GetFlagZ(value);
    m_flagS = GetFlagS(value);
    m_flagP = GetFlagP(value);
}

inline bool CPU::GetFlagZ(u8 value) { return value == 0; }
inline bool CPU::GetFlagS(u8 value) { return (value & 0x80) == 0x80; }
inline bool CPU::GetFlagP(u8 value)
{
    u8 parity = 0;
    while (value) {
        parity ^= (value & 1);
        value >>= 1;
    }
    return 1 - parity;
}

inline void CPU::SetFlagC(u8 a, u8 b, u8 carry)
{
    u16 sum = a + b + carry;
    m_flagC = (sum > 0xff);
}

inline void CPU::SetFlagC(u16 value) { m_flagC = (value > 0xff); }

void CPU::Push(u16 value)
{
    m_SP -= 2;
    SetWord(m_SP, value);
}

u16 CPU::Pop()
{
    const auto ret = GetWord(m_SP);
    m_SP += 2;
    return ret;
}
void CPU::PopPSW()
{
    const auto psw = Pop();
    m_A = psw >> 8;
    m_flagS = psw >> 7 & 0x1;
    m_flagZ = psw >> 6 & 0x1;
    m_flagAC = psw >> 4 & 0x1;
    m_flagP = psw >> 2 & 0x1;
    m_flagC = psw & 0x1;
}
u8 CPU::Fetch()
{
    const auto ret = m_memory.at(m_PC);
    ++m_PC;
    return ret;
}

// clang-format off
void CPU::Decode()
{
    switch (Fetch()) {
    case 0x00:
    case 0x08:
    case 0x10:
    case 0x18:
    case 0x20:
    case 0x28:
    case 0x30:
    case 0xcb:
    case 0xdd:
    case 0xed:
    case 0xfd:
    case 0x38:
        break; // NOP
    case Instructions::stc: m_flagC = 1; break;
    case Instructions::cma: m_A = ~m_A; break;
    case Instructions::cmc: m_flagC ^= 1; break;
    case Instructions::jmp: JMP(Read16()); break;
    case Instructions::jc: JMP(Read16(), m_flagC); break;
    case Instructions::jnc: JMP(Read16(), !m_flagC); break;
    case Instructions::jz: JMP(Read16(), m_flagZ); break;
    case Instructions::jnz: JMP(Read16(), !m_flagZ); break;
    case Instructions::jm: JMP(Read16(), m_flagS); break;
    case Instructions::jp: JMP(Read16(), !m_flagS); break;
    case Instructions::jpe: JMP(Read16(), m_flagP); break;
    case Instructions::jpo: JMP(Read16(), !m_flagP); break;
    case Instructions::call: CALL(Read16()); break;
    case Instructions::cc: CALL(Read16(), m_flagC); break;
    case Instructions::cnc: CALL(Read16(), !m_flagC); break;
    case Instructions::cz: CALL(Read16(), m_flagZ); break;
    case Instructions::cnz: CALL(Read16(), !m_flagZ); break;
    case Instructions::cm: CALL(Read16(), m_flagS); break;
    case Instructions::cp: CALL(Read16(), !m_flagS); break;
    case Instructions::cpe: CALL(Read16(), m_flagP); break;
    case Instructions::cpo: CALL(Read16(), !m_flagP); break;

    case Instructions::ret: RET(); break;
    case Instructions::rc: RET(m_flagC); break;
    case Instructions::rnc: RET(!m_flagC); break;
    case Instructions::rz: RET(m_flagZ); break;
    case Instructions::rnz: RET(!m_flagZ); break;
    case Instructions::rm: RET(m_flagS); break;
    case Instructions::rp: RET(!m_flagS); break;
    case Instructions::rpe: RET(m_flagP); break;
    case Instructions::rpo: RET(!m_flagP); break;

    case Instructions::lxi_b: SetBC(ReadAndAdvance16()); break;
    case Instructions::lxi_d: SetDE(ReadAndAdvance16()); break;
    case Instructions::lxi_h: SetHL(ReadAndAdvance16()); break;
    case Instructions::lxi_sp: m_SP = ReadAndAdvance16(); break;
    case Instructions::mvi_a: MVI(m_A); break;
    case Instructions::mvi_b: MVI(m_B); break;
    case Instructions::mvi_c: MVI(m_C); break;
    case Instructions::mvi_d: MVI(m_D); break;
    case Instructions::mvi_e: MVI(m_E); break;
    case Instructions::mvi_h: MVI(m_H); break;
    case Instructions::mvi_l: MVI(m_L); break;
    case Instructions::mvi_m: SetByte(HL(), Read8()); m_PC += 1; break;

    case Instructions::adi: {
        const u16 result = static_cast<u16>(m_A + Read8());
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::aci: {
        const u16 result = static_cast<u16>(m_A + Read8() + m_flagC);
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::sui: {
        const u16 result = static_cast<u16>(m_A + TwoComp8(Read8()));
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::sbi: {
        const u16 result = static_cast<u16>(m_A + TwoComp8(Read8()) + TwoComp8(m_flagC));
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::ani: {
        const u16 result = static_cast<u16>(m_A & Read8());
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::xri: {
        const u16 result = static_cast<u16>(m_A ^ Read8());
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::ori: {
        const u16 result = static_cast<u16>(m_A | Read8());
        m_A = result & 0xff;
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::cpi: {
        // A is not changed by this operation, only the FLAGS
        const u16 result = static_cast<u16>(m_A + TwoComp8(Read8()));
        SetFlagC(result);
        SetFlagsZSP(m_A);
        m_PC += 1;
        break;
    }
    case Instructions::ldax_b: m_A = GetByte(BC()); break;
    case Instructions::ldax_d: m_A = GetByte(DE()); break;
    case Instructions::mov_a_a: MOV(m_A, m_A); break;
    case Instructions::mov_a_b: MOV(m_A, m_B); break;
    case Instructions::mov_a_c: MOV(m_A, m_C); break;
    case Instructions::mov_a_d: MOV(m_A, m_D); break;
    case Instructions::mov_a_e: MOV(m_A, m_E); break;
    case Instructions::mov_a_h: MOV(m_A, m_H); break;
    case Instructions::mov_a_l: MOV(m_A, m_L); break;
    case Instructions::mov_a_m: MOV(m_A, GetByte(HL())); break;
    case Instructions::mov_b_a: MOV(m_B, m_A); break;
    case Instructions::mov_b_b: MOV(m_B, m_B); break;
    case Instructions::mov_b_c: MOV(m_B, m_C); break;
    case Instructions::mov_b_d: MOV(m_B, m_D); break;
    case Instructions::mov_b_e: MOV(m_B, m_E); break;
    case Instructions::mov_b_h: MOV(m_B, m_H); break;
    case Instructions::mov_b_l: MOV(m_B, m_L); break;
    case Instructions::mov_b_m: MOV(m_B, GetByte(HL())); break;
    case Instructions::mov_c_a: MOV(m_C, m_A); break;
    case Instructions::mov_c_b: MOV(m_C, m_B); break;
    case Instructions::mov_c_c: MOV(m_C, m_C); break;
    case Instructions::mov_c_d: MOV(m_C, m_D); break;
    case Instructions::mov_c_e: MOV(m_C, m_E); break;
    case Instructions::mov_c_h: MOV(m_C, m_H); break;
    case Instructions::mov_c_l: MOV(m_C, m_L); break;
    case Instructions::mov_c_m: MOV(m_C, GetByte(HL())); break;
    case Instructions::mov_d_a: MOV(m_D, m_A); break;
    case Instructions::mov_d_b: MOV(m_D, m_B); break;
    case Instructions::mov_d_c: MOV(m_D, m_C); break;
    case Instructions::mov_d_d: MOV(m_D, m_D); break;
    case Instructions::mov_d_e: MOV(m_D, m_E); break;
    case Instructions::mov_d_h: MOV(m_D, m_H); break;
    case Instructions::mov_d_l: MOV(m_D, m_L); break;
    case Instructions::mov_d_m: MOV(m_D, GetByte(HL())); break;
    case Instructions::mov_e_a: MOV(m_E, m_A); break;
    case Instructions::mov_e_b: MOV(m_E, m_B); break;
    case Instructions::mov_e_c: MOV(m_E, m_C); break;
    case Instructions::mov_e_d: MOV(m_E, m_D); break;
    case Instructions::mov_e_e: MOV(m_E, m_E); break;
    case Instructions::mov_e_h: MOV(m_E, m_H); break;
    case Instructions::mov_e_l: MOV(m_E, m_L); break;
    case Instructions::mov_e_m: MOV(m_E, GetByte(HL())); break;
    case Instructions::mov_h_a: MOV(m_H, m_A); break;
    case Instructions::mov_h_b: MOV(m_H, m_B); break;
    case Instructions::mov_h_c: MOV(m_H, m_C); break;
    case Instructions::mov_h_d: MOV(m_H, m_D); break;
    case Instructions::mov_h_e: MOV(m_H, m_E); break;
    case Instructions::mov_h_h: MOV(m_H, m_H); break;
    case Instructions::mov_h_l: MOV(m_H, m_L); break;
    case Instructions::mov_h_m: MOV(m_H, GetByte(HL())); break;
    case Instructions::mov_l_a: MOV(m_L, m_A); break;
    case Instructions::mov_l_b: MOV(m_L, m_B); break;
    case Instructions::mov_l_c: MOV(m_L, m_C); break;
    case Instructions::mov_l_d: MOV(m_L, m_D); break;
    case Instructions::mov_l_e: MOV(m_L, m_E); break;
    case Instructions::mov_l_h: MOV(m_L, m_H); break;
    case Instructions::mov_l_l: MOV(m_L, m_L); break;
    case Instructions::mov_l_m: MOV(m_L, GetByte(HL())); break;
    case Instructions::mov_m_a: MOVM(m_A); break;
    case Instructions::mov_m_b: MOVM(m_B); break;
    case Instructions::mov_m_c: MOVM(m_C); break;
    case Instructions::mov_m_d: MOVM(m_D); break;
    case Instructions::mov_m_e: MOVM(m_E); break;
    case Instructions::mov_m_h: MOVM(m_H); break;
    case Instructions::mov_m_l: MOVM(m_L); break;
    case Instructions::stax_b: SetByte(BC(), m_A); break;
    case Instructions::stax_d: SetByte(DE(), m_A); break;
    case Instructions::push_b: Push(BC()); break;
    case Instructions::push_d: Push(DE()); break;
    case Instructions::push_h: Push(HL()); break;
    case Instructions::push_psw: Push(PSW()); break;
    case Instructions::pop_b: SetBC(Pop()); break;
    case Instructions::pop_d: SetDE(Pop()); break;
    case Instructions::pop_h: SetHL(Pop()); break;
    case Instructions::pop_psw: PopPSW(); break;
    case Instructions::dad_b: DAD(BC()); break;
    case Instructions::dad_d: DAD(DE()); break;
    case Instructions::dad_h: DAD(HL()); break;
    case Instructions::dad_sp: DAD(m_SP); break;
    case Instructions::inx_b: SetBC(BC() + 1); break;
    case Instructions::inx_d: SetDE(DE() + 1); break;
    case Instructions::inx_h: SetHL(HL() + 1); break;
    case Instructions::inx_sp: m_SP += 1; break;
    case Instructions::dcx_b: SetBC(BC() - 1); break;
    case Instructions::dcx_d: SetDE(DE() - 1); break;
    case Instructions::dcx_h: SetHL(BC() - 1); break;
    case Instructions::dcx_sp: m_SP -= 1; break;
    case Instructions::xchg: {
        u8 temp = m_H;
        m_H = m_D;
        m_D = temp;

        temp = m_L;
        m_L = m_E;
        m_E = temp;
        break;
    }
    case Instructions::xthl: {
        u8 tempL = m_L;
        u8 tempH = m_H;
        m_L = GetByte(m_SP);
        m_H = GetByte(m_SP + 1);
        SetByte(m_SP, tempL);
        SetByte(m_SP+1, tempH);
        break;
    }

    case Instructions::sphl: m_SP = HL(); break;

    case Instructions::inr_a: INR(m_A); break;
    case Instructions::inr_b: INR(m_B); break;
    case Instructions::inr_c: INR(m_C); break;
    case Instructions::inr_d: INR(m_D); break;
    case Instructions::inr_e: INR(m_E); break;
    case Instructions::inr_h: INR(m_H); break;
    case Instructions::inr_l: INR(m_L); break;
    case Instructions::inr_m:  {
        u16 result = HL() + 1;
        SetByte(HL(), result & 0xff);
        SetFlagsZSP(result & 0xff);
        SetFlagC(result);
        break;
    }
    case Instructions::dcr_a: DCR(m_A);break;
    case Instructions::dcr_b: DCR(m_B);break;
    case Instructions::dcr_c: DCR(m_C);break;
    case Instructions::dcr_d: DCR(m_D);break;
    case Instructions::dcr_e: DCR(m_E);break;
    case Instructions::dcr_h: DCR(m_H);break;
    case Instructions::dcr_l: DCR(m_L);break;
    case Instructions::dcr_m: {
        const u16 result = GetByte(HL()) + TwoComp8(1);
        SetFlagsZSP(result & 0xff);
        SetFlagC(result);
        SetByte(HL(), result & 0xff);
        break;
    }
    case Instructions::daa: DAA(); break;
    case Instructions::rlc: RLC(); break;
    case Instructions::rrc: RRC(); break;
    case Instructions::rar: RAR(); break;
    case Instructions::ral: RAL(); break;

    case Instructions::in: {
        u8 deviceNumber = ReadAndAdvance8();
        if (deviceNumber == DEVICE_SHIFT_IN) {
            m_A  = ((m_shiftRegister >> (8 - m_shiftRegisterAmount)) & 0xff);
        } else {
            m_A = m_inputPorts.at(deviceNumber);
        }

        break;
    }

    case Instructions::out: {
        u8 deviceNumber = ReadAndAdvance8();

        if (deviceNumber == DEVICE_SHIFT_AMT) {
            m_shiftRegisterAmount = (m_A & 7);
        } else if (deviceNumber == DEVICE_SHIFT_DATA) {
            m_shiftRegister >>= 8;
            m_shiftRegister |= (static_cast<u16>(m_A) << 8);
        } else {
            m_outputPorts[deviceNumber] = m_A;
        }

        break;
    }

    case Instructions::hlt:
        m_isHalted = true;
        break;

    case Instructions::add_a: SetAWithFlags(static_cast<u16>(m_A) + m_A); break;
    case Instructions::add_b: SetAWithFlags(static_cast<u16>(m_A) + m_B); break;
    case Instructions::add_c: SetAWithFlags(static_cast<u16>(m_A) + m_C); break;
    case Instructions::add_d: SetAWithFlags(static_cast<u16>(m_A) + m_D); break;
    case Instructions::add_e: SetAWithFlags(static_cast<u16>(m_A) + m_E); break;
    case Instructions::add_h: SetAWithFlags(static_cast<u16>(m_A) + m_H); break;
    case Instructions::add_l: SetAWithFlags(static_cast<u16>(m_A) + m_L); break;
    case Instructions::add_m: SetAWithFlags(static_cast<u16>(m_A) + GetByte(HL())); break;

    case Instructions::adc_a: SetAWithFlags(static_cast<u16>(m_A) + m_A + m_flagC); break;
    case Instructions::adc_b: SetAWithFlags(static_cast<u16>(m_A) + m_B + m_flagC); break;
    case Instructions::adc_c: SetAWithFlags(static_cast<u16>(m_A) + m_C + m_flagC); break;
    case Instructions::adc_d: SetAWithFlags(static_cast<u16>(m_A) + m_D + m_flagC); break;
    case Instructions::adc_e: SetAWithFlags(static_cast<u16>(m_A) + m_E + m_flagC); break;
    case Instructions::adc_h: SetAWithFlags(static_cast<u16>(m_A) + m_H + m_flagC); break;
    case Instructions::adc_l: SetAWithFlags(static_cast<u16>(m_A) + m_L + m_flagC); break;
    case Instructions::adc_m: SetAWithFlags(static_cast<u16>(m_A) + GetByte(HL()) + m_flagC); break;

    case Instructions::sub_a: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_A)); break;
    case Instructions::sub_b: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_B)); break;
    case Instructions::sub_c: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_C)); break;
    case Instructions::sub_d: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_D)); break;
    case Instructions::sub_e: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_E)); break;
    case Instructions::sub_h: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_H)); break;
    case Instructions::sub_l: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_L)); break;
    case Instructions::sub_m: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(GetByte(HL()))); break;

    case Instructions::sbb_a: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_A) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_b: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_B) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_c: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_C) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_d: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_D) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_e: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_E) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_h: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_H) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_l: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(m_L) + TwoComp8(m_flagC)); break;
    case Instructions::sbb_m: SetAWithFlags(static_cast<u16>(m_A) + TwoComp8(GetByte(HL())) + TwoComp8(m_flagC)); break;

    case Instructions::ana_a: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_A)); break;
    case Instructions::ana_b: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_B)); break;
    case Instructions::ana_c: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_C)); break;
    case Instructions::ana_d: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_D)); break;
    case Instructions::ana_e: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_E)); break;
    case Instructions::ana_h: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_H)); break;
    case Instructions::ana_l: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(m_L)); break;
    case Instructions::ana_m: SetAWithFlags(static_cast<u16>(m_A) & TwoComp8(GetByte(HL()))); break;

    case Instructions::xra_a: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_A)); break;
    case Instructions::xra_b: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_B)); break;
    case Instructions::xra_c: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_C)); break;
    case Instructions::xra_d: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_D)); break;
    case Instructions::xra_e: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_E)); break;
    case Instructions::xra_h: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_H)); break;
    case Instructions::xra_l: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(m_L)); break;
    case Instructions::xra_m: SetAWithFlags(static_cast<u16>(m_A) ^ TwoComp8(GetByte(HL()))); break;

    case Instructions::ora_a: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_A)); break;
    case Instructions::ora_b: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_B)); break;
    case Instructions::ora_c: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_C)); break;
    case Instructions::ora_d: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_D)); break;
    case Instructions::ora_e: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_E)); break;
    case Instructions::ora_h: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_H)); break;
    case Instructions::ora_l: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(m_L)); break;
    case Instructions::ora_m: SetAWithFlags(static_cast<u16>(m_A) | TwoComp8(GetByte(HL()))); break;

    case Instructions::cmp_a: CMP(m_A); break;
    case Instructions::cmp_b: CMP(m_B); break;
    case Instructions::cmp_c: CMP(m_C); break;
    case Instructions::cmp_d: CMP(m_D); break;
    case Instructions::cmp_e: CMP(m_E); break;
    case Instructions::cmp_h: CMP(m_H); break;
    case Instructions::cmp_l: CMP(m_L); break;
    case Instructions::cmp_m: CMP(GetByte(HL())); break;

    case Instructions::sta: SetByte(ReadAndAdvance16(), m_A); break;
    case Instructions::lda: m_A = GetByte(ReadAndAdvance16()); break;
    case Instructions::shld: SetWord(ReadAndAdvance16(), HL()); break;
    case Instructions::lhld: SetHL(GetWord(ReadAndAdvance16())); break;
    case Instructions::pchl: m_PC = HL(); break;

    case Instructions::ei: m_isInterruptsEnabled = true; break;
    case Instructions::di: m_isInterruptsEnabled = false; break;

    case Instructions::interrupt0: CALL(0x00); break;
    case Instructions::interrupt1: CALL(0x08); break;
    case Instructions::interrupt2: CALL(0x10); break;
    case Instructions::interrupt3: CALL(0x18); break;
    case Instructions::interrupt4: CALL(0x20); break;
    case Instructions::interrupt5: CALL(0x28); break;
    case Instructions::interrupt6: CALL(0x30); break;
    case Instructions::interrupt7: CALL(0x38); break;
    }
}
// clang-format on

void CPU::LoadProgram(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::in);
    if (!file.is_open() || file.bad())
        throw std::runtime_error("Could not open executable to be emulated");

    // Get the file size
    file.seekg(0, std::ios::end);
    const auto file_size = file.tellg();

    if (file_size > m_memory.size())
        throw std::runtime_error("File to be loaded is too big for emulated memory pool");

    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(m_memory.data()), file_size);

    spdlog::debug("Loaded program {} to emulator memory (size {:x} bytes)", path, file_size);
}

inline u8 CPU::GetInputPort(int portNumber) const { return m_inputPorts.at(portNumber); }
inline void CPU::SetInputPort(int portNumber, u8 value) { m_inputPorts[portNumber] = value; }

void CPU::JMP(u16 address) { JMP(address, true); }

void CPU::JMP(u16 address, bool condition)
{
    m_PC += 2;
    if (condition) {
        m_PC = address;
    }
}
void CPU::CALL(u16 address) { CALL(address, true); }

void CPU::CALL(u16 address, bool condition)
{
    m_PC += 2;
    if (condition) {
        Push(m_PC);
        m_PC = address;
    }
}

void CPU::RET() { RET(true); }

void CPU::RET(bool condition)
{
    if (condition) {
        m_PC = Pop();
    }
}

inline u8 CPU::Read8() const { return GetByte(m_PC); }
inline u16 CPU::Read16() const { return GetWord(m_PC); }

inline u8 CPU::ReadAndAdvance8()
{
    u8 ret = GetByte(m_PC);
    m_PC += 1;
    return ret;
}

inline u16 CPU::ReadAndAdvance16()
{
    u16 ret = GetWord(m_PC);
    m_PC += 2;
    return ret;
}

inline void CPU::MOV(u8& dest, const u8 src) { dest = src; }

inline void CPU::MVI(u8& dest) { dest = ReadAndAdvance8(); }

inline u8 CPU::TwoComp8(const u8 value) { return static_cast<u8>(~value + 1); }

inline u16 CPU::TwoComp16(const u16 value) { return static_cast<u16>(~value + 1); }

void CPU::DAD(const u16 registerPair)
{
    const u32 result = static_cast<u32>(registerPair) + HL();
    SetFlagC(result & 0xffff);
    SetHL(result & 0xffff);
}

void CPU::MOVM(const u8 srcRegister) { SetByte(HL(), srcRegister); }

void CPU::INR(u8& srcRegister)
{
    const u16 result = static_cast<u16>(srcRegister) + 1;
    SetFlagsZSP(srcRegister);
    SetFlagC(result);
    srcRegister = result & 0xff;
}

void CPU::DCR(u8& srcRegister)
{
    const u16 result = static_cast<u16>(srcRegister) + TwoComp8(1);
    SetFlagsZSP(srcRegister);
    SetFlagC(result);
    srcRegister = result & 0xff;
}
void CPU::DAA()
{
    u16 result = 0;
    if ((m_A & 0x0f) > 9 || m_flagAC) {
        result += 6;
    }

    if ((m_A >> 4) > 9 || m_flagC) {
        result += 0x60;
        m_flagC = 1;
        result += m_A;
        SetFlagsZSP(result & 0xff);
        SetFlagC(result);
    }
}
void CPU::RLC()
{
    const u8 bit7 = (m_A & 0x80) >> 7;
    m_A <<= 1;
    m_A |= bit7;
    m_flagC = bit7;
}

void CPU::RRC()
{
    const u8 bit0 = (m_A & 0x01);
    m_A >>= 1;
    m_A |= (bit0 << 7);
    m_flagC = (bit0 != 0);
}
void CPU::RAL()
{
    const u8 bit7 = (m_A & 0x80) >> 7;
    m_A <<= 1;
    m_A |= m_flagC;
    m_flagC = bit7;
}

void CPU::RAR()
{
    const u8 bit0 = (m_A & 0x01);
    m_A >>= 1;
    m_A |= (m_flagC << 7);
    m_flagC = bit0;
}

void CPU::SetAWithFlags(u16 value)
{
    SetFlagsZSP(value & 0xff);
    SetFlagC(value);
    m_A = value & 0xff;
}

void CPU::CMP(u8 srcRegister)
{
    const u16 value = static_cast<u16>(m_A) + TwoComp8(srcRegister);
    SetFlagsZSP(value & 0xff);
    SetFlagC(value);
}
u8* CPU::GetMemoryAtOffset(u16 offset) {
    spdlog::debug("Returning memory at offset {} from CPU", reinterpret_cast<void*>(m_memory.data()+ offset));
    return m_memory.data() + offset;
}
void CPU::Execute() {
    Fetch();
    Decode();
}
void CPU::Interrupt(u8 number) {
    if (m_isInterruptsEnabled) {
        m_isInterruptsEnabled = false;
        Push(m_PC);
        m_PC = static_cast<u16>(number) * 8;
    }
}
