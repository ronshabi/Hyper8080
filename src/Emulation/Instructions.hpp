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

constexpr auto ret = 0xc9;
constexpr auto rc = 0xd8;
constexpr auto rnc = 0xd0;
constexpr auto rz = 0xc8;
constexpr auto rnz = 0xc0;
constexpr auto rm = 0xf8;
constexpr auto rp = 0xf0;
constexpr auto rpe = 0xe8;
constexpr auto rpo = 0xe0;

constexpr auto lxi_b = 0x01;
constexpr auto lxi_d = 0x11;
constexpr auto lxi_h = 0x21;
constexpr auto lxi_sp = 0x31;

constexpr auto mvi_a = 0x3e;
constexpr auto mvi_b = 0x06;
constexpr auto mvi_c = 0x0e;
constexpr auto mvi_d = 0x16;
constexpr auto mvi_e = 0x1e;
constexpr auto mvi_h = 0x26;
constexpr auto mvi_l = 0x2e;
constexpr auto mvi_m = 0x36;

constexpr auto adi = 0xc6;
constexpr auto aci = 0xce;
constexpr auto sui = 0xd6;
constexpr auto sbi = 0xde;
constexpr auto ani = 0xe6;
constexpr auto xri = 0xee;
constexpr auto ori = 0xf6;
constexpr auto cpi = 0xfe;

constexpr auto ldax_b = 0x0a;
constexpr auto ldax_d = 0x1a;

constexpr auto mov_a_a = 0x7f;
constexpr auto mov_a_b = 0x78;
constexpr auto mov_a_c = 0x79;
constexpr auto mov_a_d = 0x7a;
constexpr auto mov_a_e = 0x7b;
constexpr auto mov_a_h = 0x7c;
constexpr auto mov_a_l = 0x7d;
constexpr auto mov_a_m = 0x7e;

constexpr auto mov_b_a = 0x47;
constexpr auto mov_b_b = 0x40;
constexpr auto mov_b_c = 0x41;
constexpr auto mov_b_d = 0x42;
constexpr auto mov_b_e = 0x43;
constexpr auto mov_b_h = 0x44;
constexpr auto mov_b_l = 0x45;
constexpr auto mov_b_m = 0x46;

constexpr auto mov_c_a = 0x4f;
constexpr auto mov_c_b = 0x48;
constexpr auto mov_c_c = 0x49;
constexpr auto mov_c_d = 0x4a;
constexpr auto mov_c_e = 0x4b;
constexpr auto mov_c_h = 0x4c;
constexpr auto mov_c_l = 0x4d;
constexpr auto mov_c_m = 0x4e;

constexpr auto mov_d_a = 0x57;
constexpr auto mov_d_b = 0x50;
constexpr auto mov_d_c = 0x51;
constexpr auto mov_d_d = 0x52;
constexpr auto mov_d_e = 0x53;
constexpr auto mov_d_h = 0x54;
constexpr auto mov_d_l = 0x55;
constexpr auto mov_d_m = 0x56;

constexpr auto mov_e_a = 0x5f;
constexpr auto mov_e_b = 0x58;
constexpr auto mov_e_c = 0x59;
constexpr auto mov_e_d = 0x5a;
constexpr auto mov_e_e = 0x5b;
constexpr auto mov_e_h = 0x5c;
constexpr auto mov_e_l = 0x5d;
constexpr auto mov_e_m = 0x5e;

constexpr auto mov_h_a = 0x67;
constexpr auto mov_h_b = 0x60;
constexpr auto mov_h_c = 0x61;
constexpr auto mov_h_d = 0x62;
constexpr auto mov_h_e = 0x63;
constexpr auto mov_h_h = 0x64;
constexpr auto mov_h_l = 0x65;
constexpr auto mov_h_m = 0x66;

constexpr auto mov_l_a = 0x6f;
constexpr auto mov_l_b = 0x68;
constexpr auto mov_l_c = 0x69;
constexpr auto mov_l_d = 0x6a;
constexpr auto mov_l_e = 0x6b;
constexpr auto mov_l_h = 0x6c;
constexpr auto mov_l_l = 0x6d;
constexpr auto mov_l_m = 0x6e;

constexpr auto mov_m_a = 0x77;
constexpr auto mov_m_b = 0x70;
constexpr auto mov_m_c = 0x71;
constexpr auto mov_m_d = 0x72;
constexpr auto mov_m_e = 0x73;
constexpr auto mov_m_h = 0x74;
constexpr auto mov_m_l = 0x75;

constexpr auto stax_b = 0x02;
constexpr auto stax_d = 0x12;

constexpr auto push_b = 0xc5;
constexpr auto push_d = 0xd5;
constexpr auto push_h = 0xe5;
constexpr auto push_psw = 0xf5;
constexpr auto pop_b = 0xc1;
constexpr auto pop_d = 0xd1;
constexpr auto pop_h = 0xe1;
constexpr auto pop_psw = 0xf1;

constexpr auto dad_b = 0x09;
constexpr auto dad_d = 0x19;
constexpr auto dad_h = 0x29;
constexpr auto dad_sp = 0x39;
constexpr auto inx_b = 0x03;
constexpr auto inx_d = 0x13;
constexpr auto inx_h = 0x23;
constexpr auto inx_sp = 0x33;
constexpr auto dcx_b = 0x0b;
constexpr auto dcx_d = 0x1b;
constexpr auto dcx_h = 0x2b;
constexpr auto dcx_sp = 0x3b;
constexpr auto xchg = 0xeb;
constexpr auto xthl = 0xe3;
constexpr auto sphl = 0xf9;

constexpr auto inr_a = 0x3c;
constexpr auto inr_b = 0x04;
constexpr auto inr_c = 0x0c;
constexpr auto inr_d = 0x14;
constexpr auto inr_e = 0x1c;
constexpr auto inr_h = 0x24;
constexpr auto inr_l = 0x2c;
constexpr auto inr_m = 0x34;
constexpr auto dcr_a = 0x3d;
constexpr auto dcr_b = 0x05;
constexpr auto dcr_c = 0x0d;
constexpr auto dcr_d = 0x15;
constexpr auto dcr_e = 0x1d;
constexpr auto dcr_h = 0x25;
constexpr auto dcr_l = 0x2d;
constexpr auto dcr_m = 0x35;
constexpr auto daa = 0x27;

constexpr auto rlc = 0x07;
constexpr auto rrc = 0x0f;
constexpr auto rar = 0x1f;
constexpr auto ral = 0x17;
constexpr auto in = 0xdb;
constexpr auto out = 0xd3;
constexpr auto hlt = 0x76;

constexpr auto add_a=  0x87;
constexpr auto add_b=  0x80;
constexpr auto add_c=  0x81;
constexpr auto add_d=  0x82;
constexpr auto add_e=  0x83;
constexpr auto add_h=  0x84;
constexpr auto add_l=  0x85;
constexpr auto add_m = 0x86;

constexpr auto adc_a = 0x8f;

constexpr auto adc_b = 0x88;
constexpr auto adc_c = 0x89;
constexpr auto adc_d = 0x8a;
constexpr auto adc_e = 0x8b;
constexpr auto adc_h = 0x8c;
constexpr auto adc_l = 0x8d;
constexpr auto adc_m = 0x8e;
constexpr auto sub_a = 0x97;
constexpr auto sub_b = 0x90;
constexpr auto sub_c = 0x91;
constexpr auto sub_d = 0x92;
constexpr auto sub_e = 0x93;
constexpr auto sub_h = 0x94;
constexpr auto sub_l = 0x95;
constexpr auto sub_m = 0x96;
constexpr auto sbb_a = 0x9f;
constexpr auto sbb_b = 0x98;
constexpr auto sbb_c = 0x99;
constexpr auto sbb_d = 0x9a;
constexpr auto sbb_e = 0x9b;
constexpr auto sbb_h = 0x9c;
constexpr auto sbb_l = 0x9d;
constexpr auto sbb_m = 0x9e;
constexpr auto ana_a = 0xa7;
constexpr auto ana_b = 0xa0;
constexpr auto ana_c = 0xa1;
constexpr auto ana_d = 0xa2;
constexpr auto ana_e = 0xa3;
constexpr auto ana_h = 0xa4;
constexpr auto ana_l = 0xa5;
constexpr auto ana_m = 0xa6;
constexpr auto xra_a = 0xaf;
constexpr auto xra_b = 0xa8;
constexpr auto xra_c = 0xa9;
constexpr auto xra_d = 0xaa;
constexpr auto xra_e = 0xab;
constexpr auto xra_h = 0xac;
constexpr auto xra_l = 0xad;
constexpr auto xra_m = 0xae;
constexpr auto ora_a = 0xb7;
constexpr auto ora_b = 0xb0;
constexpr auto ora_c = 0xb1;
constexpr auto ora_d = 0xb2;
constexpr auto ora_e = 0xb3;
constexpr auto ora_h = 0xb4;
constexpr auto ora_l = 0xb5;
constexpr auto ora_m = 0xb6;
constexpr auto cmp_b = 0xb8;
constexpr auto cmp_c = 0xb9;
constexpr auto cmp_d = 0xba;
constexpr auto cmp_e = 0xbb;
constexpr auto cmp_h = 0xbc;
constexpr auto cmp_l = 0xbd;
constexpr auto cmp_m = 0xbe;
constexpr auto cmp_a = 0xbf;

constexpr auto sta = 0x32;
constexpr auto lda = 0x3a;
constexpr auto shld = 0x22;
constexpr auto lhld = 0x2a;
constexpr auto pchl = 0xe9;

constexpr auto ei = 0xfb;
constexpr auto di = 0xf3;

constexpr auto rsi0 = 0xc7;
constexpr auto rsi1 = 0xcf;
constexpr auto rsi2 = 0xd7;
constexpr auto rsi3 = 0xdf;
constexpr auto rsi4 = 0xe7;
constexpr auto rsi5 = 0xef;
constexpr auto rsi6 = 0xf7;
constexpr auto rsi7 = 0xff;
} // namespace Instructions