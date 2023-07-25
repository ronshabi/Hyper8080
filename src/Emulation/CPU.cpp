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
