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
    ret |= (m_flag_s << 7);
    ret |= (m_flag_z << 6);
    ret |= (m_flag_ac << 4);
    ret |= (m_flag_p << 2);
    ret |= (1 << 1);
    ret |= (m_flag_c);
    return ret;
}

inline void CPU::SetFlagsZSP(u8 value)
{
    m_flag_z = flag_Z_check(value);
    m_flag_s = flag_S_check(value);
    m_flag_p = flag_P_check(value);
}

inline bool CPU::flag_Z_check(u8 value) { return value == 0; }
inline bool CPU::flag_S_check(u8 value) { return (value & 0x80) == 0x80; }
inline bool CPU::flag_P_check(u8 value)
{
    u8 parity = 0;
    while (value) {
        parity ^= (value & 1);
        value >>= 1;
    }
    return 1 - parity;
}

inline bool CPU::flag_C_check(u8 a, u8 b, u8 carry)
{
    u16 sum = a + b + carry;
    return (sum > 0xff);
}

inline bool CPU::flag_C_check(u16 value) { return (value > 0xff); }

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
    m_flag_s = psw >> 7 & 0x1;
    m_flag_z = psw >> 6 & 0x1;
    m_flag_ac = psw >> 4 & 0x1;
    m_flag_p = psw >> 2 & 0x1;
    m_flag_c = psw & 0x1;
}
u8 CPU::Fetch()
{
    const auto ret = m_memory.at(m_PC);
    ++m_PC;
    return ret;
}
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
    case Instructions::stc:
        m_flag_c = 1;
        break;
    case Instructions::cma:
        m_A = ~m_A;
        break;
    case Instructions::cmc:
        m_flag_c ^= 1;
        break;
    case Instructions::jmp:
        Jump(Read16());
        break;
    case Instructions::jc:
        Jump(Read16(), m_flag_c);
        break;
    case Instructions::jnc:
        Jump(Read16(), !m_flag_c);
        break;
    case Instructions::jz:
        Jump(Read16(), m_flag_z);
        break;
    case Instructions::jnz:
        Jump(Read16(), !m_flag_z);
        break;
    case Instructions::jm:
        Jump(Read16(), m_flag_s);
        break;
    case Instructions::jp:
        Jump(Read16(), !m_flag_s);
        break;
    case Instructions::jpe:
        Jump(Read16(), m_flag_p);
        break;
    case Instructions::jpo:
        Jump(Read16(), !m_flag_p);
        break;
    case Instructions::call:
        Call(Read16());
        break;
    case Instructions::cc:
        Call(Read16(), m_flag_c);
        break;
    case Instructions::cnc:
        Call(Read16(), !m_flag_c);
        break;
    case Instructions::cz:
        Call(Read16(), m_flag_z);
        break;
    case Instructions::cnz:
        Call(Read16(), !m_flag_z);
        break;
    case Instructions::cm:
        Call(Read16(), m_flag_s);
        break;
    case Instructions::cp:
        Call(Read16(), !m_flag_s);
        break;
    case Instructions::cpe:
        Call(Read16(), m_flag_p);
        break;
    case Instructions::cpo:
        Call(Read16(), !m_flag_p);
        break;
    case Instructions::ret:
        Ret();
    case Instructions::rc:
        Ret(m_flag_c);
        break ;
    case Instructions::rnc:
        Ret(!m_flag_c);
        break ;
    case Instructions::rz:
        Ret(m_flag_z);
        break ;
    case Instructions::rnz:
        Ret(!m_flag_z);
        break ;
    case Instructions::rm:
        Ret(m_flag_s);
        break ;
    case Instructions::rp:
        Ret(!m_flag_s);
        break ;
    case Instructions::rpe:
        Ret(m_flag_p);
        break ;
    case Instructions::rpo:
        Ret(!m_flag_p);
        break ;

    case Instructions::lxi_b: break;
    case Instructions::lxi_d: break;
    case Instructions::lxi_h: break;
    case Instructions::lxi_sp: break;
    case Instructions::mvi_a: break;
    case Instructions::mvi_b: break;
    case Instructions::mvi_c: break;
    case Instructions::mvi_d: break;
    case Instructions::mvi_e: break;
    case Instructions::mvi_h: break;
    case Instructions::mvi_l: break;
    case Instructions::mvi_m: break;
    case Instructions::adi: break;
    case Instructions::aci: break;
    case Instructions::sui: break;
    case Instructions::sbi: break;
    case Instructions::ani: break;
    case Instructions::xri: break;
    case Instructions::ori: break;
    case Instructions::cpi: break;
    case Instructions::ldax_b: break;
    case Instructions::ldax_d: break;
    case Instructions::mov_a_a: break;
    case Instructions::mov_a_b: break;
    case Instructions::mov_a_c: break;
    case Instructions::mov_a_d: break;
    case Instructions::mov_a_e: break;
    case Instructions::mov_a_h: break;
    case Instructions::mov_a_l: break;
    case Instructions::mov_a_m: break;
    case Instructions::mov_b_a: break;
    case Instructions::mov_b_b: break;
    case Instructions::mov_b_c: break;
    case Instructions::mov_b_d: break;
    case Instructions::mov_b_e: break;
    case Instructions::mov_b_h: break;
    case Instructions::mov_b_l: break;
    case Instructions::mov_b_m: break;
    case Instructions::mov_c_a: break;
    case Instructions::mov_c_b: break;
    case Instructions::mov_c_c: break;
    case Instructions::mov_c_d: break;
    case Instructions::mov_c_e: break;
    case Instructions::mov_c_h: break;
    case Instructions::mov_c_l: break;
    case Instructions::mov_c_m: break;
    case Instructions::mov_d_a: break;
    case Instructions::mov_d_b: break;
    case Instructions::mov_d_c: break;
    case Instructions::mov_d_d: break;
    case Instructions::mov_d_e: break;
    case Instructions::mov_d_h: break;
    case Instructions::mov_d_l: break;
    case Instructions::mov_d_m: break;
    case Instructions::mov_e_a: break;
    case Instructions::mov_e_b: break;
    case Instructions::mov_e_c: break;
    case Instructions::mov_e_d: break;
    case Instructions::mov_e_e: break;
    case Instructions::mov_e_h: break;
    case Instructions::mov_e_l: break;
    case Instructions::mov_e_m: break;
    case Instructions::mov_h_a: break;
    case Instructions::mov_h_b: break;
    case Instructions::mov_h_c: break;
    case Instructions::mov_h_d: break;
    case Instructions::mov_h_e: break;
    case Instructions::mov_h_h: break;
    case Instructions::mov_h_l: break;
    case Instructions::mov_h_m: break;
    case Instructions::mov_l_a: break;
    case Instructions::mov_l_b: break;
    case Instructions::mov_l_c: break;
    case Instructions::mov_l_d: break;
    case Instructions::mov_l_e: break;
    case Instructions::mov_l_h: break;
    case Instructions::mov_l_l: break;
    case Instructions::mov_l_m: break;
    case Instructions::mov_m_a: break;
    case Instructions::mov_m_b: break;
    case Instructions::mov_m_c: break;
    case Instructions::mov_m_d: break;
    case Instructions::mov_m_e: break;
    case Instructions::mov_m_h: break;
    case Instructions::mov_m_l: break;
    case Instructions::stax_b: break;
    case Instructions::stax_d: break;
    case Instructions::push_b: break;
    case Instructions::push_d: break;
    case Instructions::push_h: break;
    case Instructions::push_psw: break;
    case Instructions::pop_b: break;
    case Instructions::pop_d: break;
    case Instructions::pop_h: break;
    case Instructions::pop_psw: break;
    case Instructions::dad_b: break;
    case Instructions::dad_d: break;
    case Instructions::dad_h: break;
    case Instructions::dad_sp: break;
    case Instructions::inx_b: break;
    case Instructions::inx_d: break;
    case Instructions::inx_h: break;
    case Instructions::inx_sp: break;
    case Instructions::dcx_b: break;
    case Instructions::dcx_d: break;
    case Instructions::dcx_h: break;
    case Instructions::dcx_sp: break;
    case Instructions::xchg: break;
    case Instructions::xthl: break;
    case Instructions::sphl: break;
    case Instructions::inr_a: break;
    case Instructions::inr_b: break;
    case Instructions::inr_c: break;
    case Instructions::inr_d: break;
    case Instructions::inr_e: break;
    case Instructions::inr_h: break;
    case Instructions::inr_l: break;
    case Instructions::inr_m: break;
    case Instructions::dcr_a: break;
    case Instructions::dcr_b: break;
    case Instructions::dcr_c: break;
    case Instructions::dcr_d: break;
    case Instructions::dcr_e: break;
    case Instructions::dcr_h: break;
    case Instructions::dcr_l: break;
    case Instructions::dcr_m: break;
    case Instructions::daa: break;
    case Instructions::rlc: break;
    case Instructions::rrc: break;
    case Instructions::rar: break;
    case Instructions::ral: break;
    case Instructions::in: break;
    case Instructions::out: break;
    case Instructions::hlt: break;
    case Instructions::add_a: break;
    case Instructions::add_b: break;
    case Instructions::add_c: break;
    case Instructions::add_d: break;
    case Instructions::add_e: break;
    case Instructions::add_h: break;
    case Instructions::add_l: break;
    case Instructions::add_m: break;
    case Instructions::adc_a: break;
    case Instructions::adc_b: break;
    case Instructions::adc_c: break;
    case Instructions::adc_d: break;
    case Instructions::adc_e: break;
    case Instructions::adc_h: break;
    case Instructions::adc_l: break;
    case Instructions::adc_m: break;
    case Instructions::sub_a: break;
    case Instructions::sub_b: break;
    case Instructions::sub_c: break;
    case Instructions::sub_d: break;
    case Instructions::sub_e: break;
    case Instructions::sub_h: break;
    case Instructions::sub_l: break;
    case Instructions::sub_m: break;
    case Instructions::sbb_a: break;
    case Instructions::sbb_b: break;
    case Instructions::sbb_c: break;
    case Instructions::sbb_d: break;
    case Instructions::sbb_e: break;
    case Instructions::sbb_h: break;
    case Instructions::sbb_l: break;
    case Instructions::sbb_m: break;
    case Instructions::ana_a: break;
    case Instructions::ana_b: break;
    case Instructions::ana_c: break;
    case Instructions::ana_d: break;
    case Instructions::ana_e: break;
    case Instructions::ana_h: break;
    case Instructions::ana_l: break;
    case Instructions::ana_m: break;
    case Instructions::xra_a: break;
    case Instructions::xra_b: break;
    case Instructions::xra_c: break;
    case Instructions::xra_d: break;
    case Instructions::xra_e: break;
    case Instructions::xra_h: break;
    case Instructions::xra_l: break;
    case Instructions::xra_m: break;
    case Instructions::ora_a: break;
    case Instructions::ora_b: break;
    case Instructions::ora_c: break;
    case Instructions::ora_d: break;
    case Instructions::ora_e: break;
    case Instructions::ora_h: break;
    case Instructions::ora_l: break;
    case Instructions::ora_m: break;
    case Instructions::cmp_b: break;
    case Instructions::cmp_c: break;
    case Instructions::cmp_d: break;
    case Instructions::cmp_e: break;
    case Instructions::cmp_h: break;
    case Instructions::cmp_l: break;
    case Instructions::cmp_m: break;
    case Instructions::cmp_a: break;
    case Instructions::sta: break;
    case Instructions::lda: break;
    case Instructions::shld: break;
    case Instructions::lhld: break;
    case Instructions::pchl: break;
    case Instructions::ei: break;
    case Instructions::di: break;
    case Instructions::rsi0: break;
    case Instructions::rsi1: break;
    case Instructions::rsi2: break;
    case Instructions::rsi3: break;
    case Instructions::rsi4: break;
    case Instructions::rsi5: break;
    case Instructions::rsi6: break;
    case Instructions::rsi7: break;
    }
}

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

    spdlog::debug("Loaded program {} to emulator memory (size {} bytes)", path, file_size);
}

u8 CPU::GetInputPort(int portNumber) const { return m_input_ports.at(portNumber); }
void CPU::SetInputPort(int portNumber, u8 value) { m_input_ports[portNumber] = value; }

void CPU::Jump(u16 address) { Jump(address, true); }

void CPU::Jump(u16 address, bool condition)
{
    m_PC += 2;
    if (condition) {
        m_PC = address;
    }
}
void CPU::Call(u16 address) { Call(address, true); }

void CPU::Call(u16 address, bool condition)
{
    m_PC += 2;
    if (condition) {
        Push(m_PC);
        m_PC = address;
    }
}

void CPU::Ret() { Ret(true); }

void CPU::Ret(bool condition)
{
    if (condition) {
        m_PC = Pop();
    }
}

u8 CPU::Read8() const { return GetByte(m_PC); }
u8 CPU::Read16() const { return GetWord(m_PC); }
