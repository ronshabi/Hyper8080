#include "CPU.h"

inline u8 CPU::get_byte(u16 address) const { return m_memory.at(address); }

inline u16 CPU::get_word(u16 address) const
{
    return static_cast<u16>(m_memory.at(address + 1) << 8) | static_cast<u16>(m_memory.at(address));
}

inline void CPU::set_byte(u16 address, u8 value) { m_memory[address] = value; }

inline void CPU::set_word(u16 address, u16 value)
{
    m_memory[address] = value & 0xff;
    m_memory[address + 1] = (value >> 8) & 0xff;
}

inline void CPU::set_BC(u16 to)
{
    m_B = static_cast<u8>((to >> 8) & 0xff);
    m_C = static_cast<u8>(to & 0xff);
}
inline void CPU::set_DE(u16 to)
{
    m_D = static_cast<u8>((to >> 8) & 0xff);
    m_E = static_cast<u8>(to & 0xff);
}

inline void CPU::set_HL(u16 to)
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

inline void CPU::set_flags_ZSP(u8 value) {
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

inline bool CPU::flag_C_check(u8 a, u8 b, u8 carry) {
    u16 sum = a + b + carry;
    return (sum > 0xff);
}

inline bool CPU::flag_C_check(u16 value) {
    return (value > 0xff);
}

void CPU::push(u16 value) {
    m_SP -= 2;
    set_word(m_SP, value);
}

u16 CPU::pop() {
    const auto ret = get_word(m_SP);
    m_SP += 2;
    return ret;
}
void CPU::pop_PSW() {
    const auto psw = pop();
    m_A = psw >> 8;
    m_flag_s = psw >> 7 & 0x1;
    m_flag_z = psw >> 6 & 0x1;
    m_flag_ac = psw >> 4 & 0x1;
    m_flag_p = psw >> 2 & 0x1;
    m_flag_c = psw & 0x1;
}
u8 CPU::fetch() {
    const auto ret = m_memory.at(m_PC);
    ++m_PC;
    return ret;
}
void CPU::decode() {
    switch (fetch()) {
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
        break;

    case Instructions::stc:
        m_flag_c = 1;
        break;
    case Instructions::cma:
        m_A = ~m_A;
        break;
    case Instructions::cmc:
        m_flag_c ^= 1;
        break;

    }
}
