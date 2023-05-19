#pragma once

#include <array>
#include <core/types.h>
#include <emulation/instructions.h>
class CPU {
public:
    explicit CPU() = default;

public:
    [[nodiscard]] u8 A() const { return m_A; }
    [[nodiscard]] u8 B() const { return m_B; }
    [[nodiscard]] u8 C() const { return m_C; }
    [[nodiscard]] u8 D() const { return m_D; }
    [[nodiscard]] u8 E() const { return m_E; }
    [[nodiscard]] u8 H() const { return m_H; }
    [[nodiscard]] u8 L() const { return m_L; }
    [[nodiscard]] u16 SP() const { return m_SP; }
    [[nodiscard]] u16 PC() const { return m_PC; }
    [[nodiscard]] u16 BC() const { return static_cast<u16>(m_B << 8) | m_C; }
    [[nodiscard]] u16 DE() const { return static_cast<u16>(m_D << 8) | m_E; }
    [[nodiscard]] u16 HL() const { return static_cast<u16>(m_H << 8) | m_L; }
    [[nodiscard]] u16 PSW() const { return static_cast<u16>(m_A << 8) | static_cast<u16>(FLAGS()); }
    [[nodiscard]] u8 FLAGS() const;

    [[nodiscard]] inline u8 get_byte(u16 address) const;
    [[nodiscard]] inline u16 get_word(u16 address) const;

    inline void set_byte(u16 address, u8 value);
    void set_word(u16 address, u16 value);
    inline void set_BC(u16 to);
    inline void set_DE(u16 to);
    inline void set_HL(u16 to);
    inline void set_flags_ZSP(u8 value);

    void push(u16 value);
    [[nodiscard]] u16 pop();
    void pop_PSW();

private:
    u8 fetch();
    void decode();

    static inline bool flag_Z_check(u8 value);
    static inline bool flag_S_check(u8 value);
    static inline bool flag_P_check(u8 value);
    static inline bool flag_C_check(u8 a, u8 b, u8 carry);
    static inline bool flag_C_check(u16 value);

    u8 m_A { 0 };
    u8 m_B { 0 };
    u8 m_C { 0 };
    u8 m_D { 0 };
    u8 m_E { 0 };
    u8 m_H { 0 };
    u8 m_L { 0 };
    u16 m_PC { 0 };
    u16 m_SP { 0 };

    u8 m_flag_z { 0 };
    u8 m_flag_s { 0 };
    u8 m_flag_p { 0 };
    u8 m_flag_c { 0 };
    u8 m_flag_ac { 0 };

    std::array<u8, 4> m_input_ports {};
    std::array<u8, 6> m_output_ports {};

    u64 m_cycles { 0 };
    u16 m_shift_register { 0 };
    u8 m_shift_register_amt { 0 };
    bool m_interrupts_enabled { false };
    bool m_is_halted { false };

    static constexpr auto allocated_memory = 0x10000;
    std::array<u8, allocated_memory> m_memory {};
};
