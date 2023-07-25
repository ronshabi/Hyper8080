#pragma once

namespace Instructions {
constexpr auto stc = 0x37;
constexpr auto cma = 0x2f;
constexpr auto cmc = 0x3f;

constexpr auto jmp = 0xc3;
constexpr auto jc = 0xda;
constexpr auto jnc = 0xd2;
constexpr auto jz = 0xca;
constexpr auto jnz = 0xc2;
constexpr auto jm = 0xfa;
constexpr auto jp = 0xf2;
constexpr auto jpe = 0xea;
constexpr auto jpo = 0xe2;

constexpr auto call = 0xcd;
constexpr auto cc = 0xdc;
constexpr auto cnc = 0xd4;
constexpr auto cz = 0xcc;
constexpr auto cnz = 0xc4;
constexpr auto cm = 0xfc;
constexpr auto cp = 0xf4;
constexpr auto cpe = 0xec;
constexpr auto cpo = 0xe4;

const auto ret = 0xc9;
const auto rc = 0xd8;
const auto rnc = 0xd0;
const auto rz = 0xc8;
const auto rnz = 0xc0;
const auto rm = 0xf8;
const auto rp = 0xf0;
const auto rpe = 0xe8;
const auto rpo = 0xe0;

} // namespace Instructions