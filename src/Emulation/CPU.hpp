#pragma once

#include <Core/Types.hpp>
#include <Emulation/Instructions.hpp>

#include <array>
#include <string>
#include <fstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

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

    [[nodiscard]] inline u8 GetByte(u16 address) const;
    [[nodiscard]] inline u16 GetWord(u16 address) const;
    inline void SetByte(u16 address, u8 value);
    inline void SetWord(u16 address, u16 value);
    inline void SetBC(u16 to);
    inline void SetDE(u16 to);
    inline void SetHL(u16 to);

    void LoadProgram(const std::string& path);
    [[nodiscard]] u8 GetInputPort(int portNumber) const;
    void SetInputPort(int portNumber, u8 value);
    [[nodiscard]] u8* GetMemoryAtOffset(u16 offset);

private:
    static constexpr auto DEVICE_SHIFT_IN = 3;
    static constexpr auto DEVICE_SHIFT_AMT = 2;
    static constexpr auto DEVICE_SHIFT_DATA = 2;

    u8 Fetch();
    void Decode();
    [[nodiscard]] inline u8 Read8() const;
    [[nodiscard]] inline u16 Read16() const;
    [[nodiscard]] inline u8 ReadAndAdvance8();
    [[nodiscard]] inline u16 ReadAndAdvance16();

    void SetAWithFlags(u16 value);

    void Push(u16 value);
    [[nodiscard]] u16 Pop();
    void PopPSW();
    void JMP(u16 address);
    void JMP(u16 address, bool condition);
    void CALL(u16 address);
    void CALL(u16 address, bool condition);
    void RET();
    void RET(bool condition);
    void MOV(u8& dest, u8 src);
    void MOVM(u8 srcRegister);
    void MVI(u8& dest);
    void DAD(u16 registerPair);
    void INR(u8& srcRegister);
    void DCR(u8& srcRegister);
    void DAA();
    void RLC();
    void RRC();
    void RAL();
    void RAR();
    void CMP(u8 srcRegister);

    static inline bool GetFlagZ(u8 value);
    static inline bool GetFlagS(u8 value);
    static inline bool GetFlagP(u8 value);
    inline void SetFlagC(u8 a, u8 b, u8 carry);
    inline void SetFlagC(u16 value);
    inline void SetFlagsZSP(u8 value);
    static inline u8 TwoComp8(u8 value);
    static inline u16 TwoComp16(u16 value);

    u8 m_A { 0 };
    u8 m_B { 0 };
    u8 m_C { 0 };
    u8 m_D { 0 };
    u8 m_E { 0 };
    u8 m_H { 0 };
    u8 m_L { 0 };
    u16 m_PC { 0 };
    u16 m_SP { 0 };
    u8 m_flagZ { 0 };
    u8 m_flagS { 0 };
    u8 m_flagP { 0 };
    u8 m_flagC { 0 };
    u8 m_flagAC { 0 };

    std::array<u8, 4> m_inputPorts {};
    std::array<u8, 6> m_outputPorts {};

    u64 m_cycles { 0 };
    u16 m_shiftRegister { 0 };
    u8 m_shiftRegisterAmount { 0 };
    bool m_isInterruptsEnabled { false };
    bool m_isHalted { false };

    static constexpr auto ALLOCATED_MEMORY = 0x10000;
    std::array<u8, ALLOCATED_MEMORY> m_memory {};
};
