#pragma once

#include <common.hpp>
#include <memory.hpp>

#include <map>

namespace mboy {

class CPU {
public:
	CPU();
	~CPU() = default;

	Memory *mem;

	void init_opcodes();

	/*************
	 * Registers *
	 *************/

	/**********************************************************************************
	 *  The GameBoy CPU-Registers have the following layout:
	 *
	 *  -------------
	 *  |  A  |  F  |
	 *  -------------
	 *  |  B  |  C  |
	 *  -------------
	 *  |  D  |  E  |
	 *  -------------
	 *  |  H  |  L  |
	 *  -------------
	 *
	 * Additionally, two register pairs can be combined to a 16-bit register.
	 * As the layout shows, combinable registers are AF, BC, DE and HL.
	 * Here, this behavior is emulated by using unions.
	 *
	 * Also note that F is the Flag-Register.
	 * To set single flag-bits in a convinient manner, another nested union is used.
	 *
	 **********************************************************************************/

	union { // AF - Registerpair
		struct {
			union { // F is the Flags-Register; give single bits their flag-name via union
				u8 F;
				struct {
				private:
					[[maybe_unused]] u8 reserved : 4;
				public:
					bool c : 1;
					bool h : 1;
					bool n : 1;
					bool z : 1;

				} flags;
			};
			u8 A = 0;
		};
		u16 AF;
	};

union { // BC - Registerpair
	struct {
		u8 C;
		u8 B;
		};
	u16 BC;
};

union { // DE - Registerpair
	struct {
		u8 E;
		u8 D;
	};
	u16 DE;
};

union { // HL - Registerpair
	struct {
		u8 L;
		u8 H;
	};
	u16 HL;
};

u16 SP; // stack pointer
u16 PC; // program counter

// using instruction = void (CPU::*)();
typedef void (CPU::*instruction)();
std::map<u8, instruction> opcodes;
std::map<u8, instruction> ext_opcodes;


/********************************************************************************
 *                                                                              *
 * CPU Instructions are listened in Section 3.3 / Page 65 of the GBC CPU manual *
 *                                                                              *
 ********************************************************************************/

	// opcode 0xCB is not directly mentionend in the manual, however it is the extension prefix-opcode,
	// meaning that it is followed by another opcode-byte.
	void cb(); // 0xCB

		// LD R,n
	void ld_b_n(); // 0x06
	void ld_c_n(); // 0x0E
	void ld_d_n(); // 0x16
	void ld_e_n(); // 0x1E
	void ld_h_n(); // 0x26
	void ld_l_n(); // 0x2E


	// LD R,R
	void ld_a_a(); // 0x7F
	void ld_b_a(); // 0x47
	void ld_c_a(); // 0x4F
	void ld_d_a(); // 0x57
	void ld_e_a(); // 0x5F
	void ld_h_a(); // 0x67
	void ld_l_a(); // 0x6F

	void ld_a_b(); // 0x78
	void ld_a_c(); // 0x79
	void ld_a_d(); // 0x7A
	void ld_a_e(); // 0x7B
	void ld_a_h(); // 0x7C
	void ld_a_l(); // 0x7D

	void ld_b_b(); // 0x40
	void ld_b_c(); // 0x41
	void ld_b_d(); // 0x42
	void ld_b_e(); // 0x43
	void ld_b_h(); // 0x44
	void ld_b_l(); // 0x45

	void ld_c_b(); // 0x48
	void ld_c_c(); // 0x49
	void ld_c_d(); // 0x4A
	void ld_c_e(); // 0x4B
	void ld_c_h(); // 0x4C
	void ld_c_l(); // 0x4D

	void ld_d_b(); // 0x50
	void ld_d_c(); // 0x51
	void ld_d_d(); // 0x52
	void ld_d_e(); // 0x53
	void ld_d_h(); // 0x54
	void ld_d_l(); // 0x55

	void ld_e_b(); // 0x58
	void ld_e_c(); // 0x59
	void ld_e_d(); // 0x5A
	void ld_e_e(); // 0x5B
	void ld_e_h(); // 0x5C
	void ld_e_l(); // 0x5D

	void ld_h_b(); // 0x60
	void ld_h_c(); // 0x61
	void ld_h_d(); // 0x62
	void ld_h_e(); // 0x63
	void ld_h_h(); // 0x64
	void ld_h_l(); // 0x65

	void ld_l_b(); // 0x68
	void ld_l_c(); // 0x69
	void ld_l_d(); // 0x6A
	void ld_l_e(); // 0x6B
	void ld_l_h(); // 0x6C
	void ld_l_l(); // 0x6D

	// LD R,(RR)ß
	void ld_a_hl(); // 0x7E
	void ld_b_hl(); // 0x46
	void ld_c_hl(); // 0x4E
	void ld_d_hl(); // 0x56
	void ld_e_hl(); // 0x5E
	void ld_h_hl(); // 0x66
	void ld_l_hl(); // 0x6E

	// LD (RR),R
	void ld_hl_a(); // 0x77
	void ld_hl_b(); // 0x70
	void ld_hl_c(); // 0x71
	void ld_hl_d(); // 0x72
	void ld_hl_e(); // 0x73
	void ld_hl_h(); // 0x74
	void ld_hl_l(); // 0x75
	void ld_hl_n(); // 0x36

	// LD R,(RR)
	void ld_a_bc();		  // 0x0A
	void ld_a_de();		  // 0x1A
	void ld_a_nn(); // 0xFA
	void ld_a_n() ; // 0x3E

	void ld_bc_a();		   // 0x02
	void ld_de_a();		   // 0x12
	void ld_nn_a();  // 0xEA
	void ldh_c_a();   // 0xE2
	void ldh_a_c(); // 0xF2

	void ld_a_hld();	 // 0x3A
	void ld_hld_a(); // 0x32
	void ld_a_hli();	  // 0x2A
	void ld_hli_a(); // 0x22

	void ldh_n_a(); // 0xE0
	void ldh_a_n();	 // 0xF0

	/**************************************
	 * Section 3.3.2, p. 76: 16-Bit Loads *
	 **************************************/

	// LD RR,nn
	void ld_bc_nn() ; // 0x01
	void ld_de_nn() ; // 0x11
	void ld_hl_nn() ; // 0x21
	void ld_sp_nn() ; // 0x31
	void ld_sp_hl(); // 0xF9


	/** Surprisingly, this one Load-Command affects the flag register
		while all other load-command don't care for the flags **/
	void ldhl_sp_n(); // 0xF8

	void ld_nn_sp(); // 0x08

	void push_af(); // 0xF5
	void push_bc(); // 0xC5
	void push_de(); // 0xD5
	void push_hl(); // 0xE5

	void pop_af(); // 0xF1
	void pop_bc(); // 0xC1
	void pop_de(); // 0xD1
	void pop_hl(); // 0xE1

	/***********************************
	 * Section 3.3.3, p. 80: 8-Bit ALU *
	 ***********************************/

	// ADD A,n
	void add_a_a(); // 0x87
	void add_a_b(); // 0x80
	void add_a_c(); // 0x81
	void add_a_d(); // 0x82
	void add_a_e(); // 0x83
	void add_a_h(); // 0x84
	void add_a_l(); // 0x85
	void add_a_hl_ref(); // 0x86
	void add_a_n(); // 0xC6

	// ADC A,n
	void adc_a_a(); // 0x8F
	void adc_a_b(); // 0x88
	void adc_a_c(); // 0x89
	void adc_a_d(); // 0x8A
	void adc_a_e(); // 0x8B
	void adc_a_h(); // 0x8C
	void adc_a_l(); // 0x8D
	void adc_a_hl_ref(); // 0x8E
	void adc_a_n(); // 0xCE

	// SUB A, n
	void sub_a_a() ; // 0x97
	void sub_a_b() ; // 0x90
	void sub_a_c() ; // 0x91
	void sub_a_d() ; // 0x92
	void sub_a_e() ; // 0x93
	void sub_a_h() ; // 0x94
	void sub_a_l() ; // 0x95
	void sub_a_hl_ref(); // 0x96
	void sub_a_n() ; // 0xD6

	// SBC A, n
	void sbc_a_a() ; // 0x9F
	void sbc_a_b() ; // 0x98
	void sbc_a_c() ; // 0x99
	void sbc_a_d() ; // 0x9A
	void sbc_a_e() ; // 0x9B
	void sbc_a_h() ; // 0x9C
	void sbc_a_l() ; // 0x9D
	void sbc_a_hl_ref(); // 0x9E
	void sbc_a_n() ; // 0xDE

	// AND A, n
	void and_a_a() ; // 0xA7
	void and_a_b() ; // 0xA0
	void and_a_c() ; // 0xA1
	void and_a_d() ; // 0xA2
	void and_a_e() ; // 0xA3
	void and_a_h() ; // 0xA4
	void and_a_l() ; // 0xA5
	void and_a_hl_ref(); // 0xA6
	void and_a_n() ; // 0xE6

	// OR A, n
	void or_a_a() ; // 0xB7
	void or_a_b() ; // 0xB0
	void or_a_c() ; // 0xB1
	void or_a_d() ; // 0xB2
	void or_a_e() ; // 0xB3
	void or_a_h() ; // 0xB4
	void or_a_l() ; // 0xB5
	void or_a_hl_ref(); // 0xB6
	void or_a_n() ; // 0xF6

	// XOR A, n
	void xor_a_a() ; // 0xAF
	void xor_a_b() ; // 0xA8
	void xor_a_c() ; // 0xA9
	void xor_a_d() ; // 0xAA
	void xor_a_e() ; // 0xAB
	void xor_a_h() ; // 0xAC
	void xor_a_l() ; // 0xAD
	void xor_a_hl_ref(); // 0xAE
	void xor_a_n() ; // 0xEE

	// CP A,n
	/* Description of CP command on p. 87:
	 * "This is basically an A - n  subtraction instruction but the results are thrown  away."
	 */
	void cp_a_a() ; // 0xBF
	void cp_a_b() ; // 0xB8
	void cp_a_c() ; // 0xB9
	void cp_a_d() ; // 0xBA
	void cp_a_e() ; // 0xBB
	void cp_a_h() ; // 0xBC
	void cp_a_l() ; // 0xBD
	void cp_a_hl_ref(); // 0xBE
	void cp_a_n() ; // 0xFE

	// INC R
	void inc_a() ; // 0x3C
	void inc_b() ; // 0x04
	void inc_c() ; // 0x0C
	void inc_d() ; // 0x14
	void inc_e() ; // 0x1C
	void inc_h() ; // 0x24
	void inc_l() ; // 0x2C
	void inc_hl_ref(); // 0x34

	// DEC R
	void dec_a() ; // 0x3D
	void dec_b() ; // 0x05
	void dec_c() ; // 0x0D
	void dec_d() ; // 0x15
	void dec_e() ; // 0x1D
	void dec_h() ; // 0x25
	void dec_l() ; // 0x2D
	void dec_hl_ref(); // 0x35


	/************************************
	 * Section 3.3.4, p. 90: 16-Bit ALU *
	 ************************************/

	// ADD HL,RR
	void add_hl_bc(); // 0x09
	void add_hl_de(); // 0x19
	void add_hl_hl(); // 0x29
	void add_hl_sp(); // 0x39

	// ADD SP,n
	void add_sp_n();
	// INC RR
	void inc_bc(); // 0x03
	void inc_de(); // 0x13
	void inc_hl(); // 0x23
	void inc_sp(); // 0x33

	// DEC RR
	void dec_bc(); // 0x0B
	void dec_de(); // 0x1B
	void dec_hl(); // 0x2B
	void dec_sp(); // 0x3B

	/***************************************
	 * Section 3.3.5, p. 94: Miscellaneous *
	 ***************************************/

	void swap_a(); // 0xCB 37
	void swap_b(); // 0xCB 30
	void swap_c(); // 0xCB 31
	void swap_d(); // 0xCB 32
	void swap_e(); // 0xCB 33
	void swap_h(); // 0xCB 34
	void swap_l(); // 0xCB 35
	void swap_hl_ref(); // 0xCB 36



	void daa();


	void cpl(); // 0x2F
	void ccf(); // 0x3F
	void scf(); // 0x37
	void nop(); // 0x00
	void halt(); // 0x76
	void stop(); // 0x10
	void di(); // 0xF3
	void ei(); // 0xFB

	/******************************************
	 * Section 3.3.6, p. 99: Rotates & Shifts *
	 ******************************************/

	void rlca(); // 0x07
	void rla(); // 0x17
	void rrca(); // 0x0F
	void rra(); // 0x1F

	// RLC n
	void rlc_a(); // 0xCB 07
	void rlc_b(); // 0xCB 00
	void rlc_c(); // 0xCB 01
	void rlc_d(); // 0xCB 02
	void rlc_e(); // 0xCB 03
	void rlc_h(); // 0xCB 04
	void rlc_l(); // 0xCB 05
	void rlc_hl_ref(); // 0xCB 06

	// RL n
	void rl_a(); // 0xCB 17
	void rl_b(); // 0xCB 10
	void rl_c(); // 0xCB 11
	void rl_d(); // 0xCB 12
	void rl_e(); // 0xCB 13
	void rl_h(); // 0xCB 14
	void rl_l(); // 0xCB 15
	void rl_hl_ref(); // 0xCB 16

	// RRC n
	void rrc_a(); // 0xCB 0F
	void rrc_b(); // 0xCB 08
	void rrc_c(); // 0xCB 09
	void rrc_d(); // 0xCB 0A
	void rrc_e(); // 0xCB 0B
	void rrc_h(); // 0xCB 0C
	void rrc_l(); // 0xCB 0D
	void rrc_hl_ref(); // 0xCB 0E

	// RR n
	void rr_a(); // 0xCB 1F
	void rr_b(); // 0xCB 18
	void rr_c(); // 0xCB 19
	void rr_d(); // 0xCB 1A
	void rr_e(); // 0xCB 1B
	void rr_h(); // 0xCB 1C
	void rr_l(); // 0xCB 1D
	void rr_hl_ref(); // 0xCB 1E

	// SLA n
	void sla_a(); // 0xCB 27
	void sla_b(); // 0xCB 20
	void sla_c(); // 0xCB 21
	void sla_d(); // 0xCB 22
	void sla_e(); // 0xCB 23
	void sla_h(); // 0xCB 24
	void sla_l(); // 0xCB 25
	void sla_hl_ref(); // 0xCB 26

	// SRA n
	void sra_a(); // 0xCB 2F
	void sra_b(); // 0xCB 28
	void sra_c(); // 0xCB 29
	void sra_d(); // 0xCB 2A
	void sra_e(); // 0xCB 2B
	void sra_h(); // 0xCB 2C
	void sra_l(); // 0xCB 2D
	void sra_hl_ref(); // 0xCB 2E

	// SRL n
	void srl_a(); // 0xCB 3F
	void srl_b(); // 0xCB 38
	void srl_c(); // 0xCB 39
	void srl_d(); // 0xCB 3A
	void srl_e(); // 0xCB 3B
	void srl_h(); // 0xCB 3C
	void srl_l(); // 0xCB 3D
	void srl_hl_ref(); // 0xCB 3E

	/**************************************
	 * Section 3.3.7, p. 108: Bit Opcodes *
	 **************************************/

	// BIT (test bit)
	void bit_a_0(); // 0xCB 47
	void bit_b_0(); // 0xCB 40
	void bit_c_0(); // 0xCB 41
	void bit_d_0(); // 0xCB 42
	void bit_e_0(); // 0xCB 43
	void bit_h_0(); // 0xCB 44
	void bit_l_0(); // 0xCB 45
	void bit_hl_ref_0(); // 0xCB 46

	void bit_a_1(); // 0xCB 4F
	void bit_b_1(); // 0xCB 48
	void bit_c_1(); // 0xCB 49
	void bit_d_1(); // 0xCB 4A
	void bit_e_1(); // 0xCB 4B
	void bit_h_1(); // 0xCB 4C
	void bit_l_1(); // 0xCB 4D
	void bit_hl_ref_1(); // 0xCB 4E

	void bit_a_2(); // 0xCB 57
	void bit_b_2(); // 0xCB 50
	void bit_c_2(); // 0xCB 51
	void bit_d_2(); // 0xCB 52
	void bit_e_2(); // 0xCB 53
	void bit_h_2(); // 0xCB 54
	void bit_l_2(); // 0xCB 55
	void bit_hl_ref_2(); // 0xCB 56

	void bit_a_3(); // 0xCB 5F
	void bit_b_3(); // 0xCB 58
	void bit_c_3(); // 0xCB 59
	void bit_d_3(); // 0xCB 5A
	void bit_e_3(); // 0xCB 5B
	void bit_h_3(); // 0xCB 5C
	void bit_l_3(); // 0xCB 5D
	void bit_hl_ref_3(); // 0xCB 5E

	void bit_a_4(); // 0xCB 67
	void bit_b_4(); // 0xCB 60
	void bit_c_4(); // 0xCB 61
	void bit_d_4(); // 0xCB 62
	void bit_e_4(); // 0xCB 63
	void bit_h_4(); // 0xCB 64
	void bit_l_4(); // 0xCB 65
	void bit_hl_ref_4(); // 0xCB 66

	void bit_a_5(); // 0xCB 6F
	void bit_b_5(); // 0xCB 68
	void bit_c_5(); // 0xCB 69
	void bit_d_5(); // 0xCB 6A
	void bit_e_5(); // 0xCB 6B
	void bit_h_5(); // 0xCB 6C
	void bit_l_5(); // 0xCB 6D
	void bit_hl_ref_5(); // 0xCB 6E

	void bit_a_6(); // 0xCB 77
	void bit_b_6(); // 0xCB 70
	void bit_c_6(); // 0xCB 71
	void bit_d_6(); // 0xCB 72
	void bit_e_6(); // 0xCB 73
	void bit_h_6(); // 0xCB 74
	void bit_l_6(); // 0xCB 75
	void bit_hl_ref_6(); // 0xCB 76

	void bit_a_7(); // 0xCB 7F
	void bit_b_7(); // 0xCB 78
	void bit_c_7(); // 0xCB 79
	void bit_d_7(); // 0xCB 7A
	void bit_e_7(); // 0xCB 7B
	void bit_h_7(); // 0xCB 7C
	void bit_l_7(); // 0xCB 7D
	void bit_hl_ref_7(); // 0xCB 7E

	// RES (reset bit)
	void res_a_0(); // 0xCB 87
	void res_b_0(); // 0xCB 80
	void res_c_0(); // 0xCB 81
	void res_d_0(); // 0xCB 82
	void res_e_0(); // 0xCB 83
	void res_h_0(); // 0xCB 84
	void res_l_0(); // 0xCB 85
	void res_hl_ref_0(); // 0xCB 86

	void res_a_1(); // 0xCB 8F
	void res_b_1(); // 0xCB 88
	void res_c_1(); // 0xCB 89
	void res_d_1(); // 0xCB 8A
	void res_e_1(); // 0xCB 8B
	void res_h_1(); // 0xCB 8C
	void res_l_1(); // 0xCB 8D
	void res_hl_ref_1(); // 0xCB 8E

	void res_a_2(); // 0xCB 97
	void res_b_2(); // 0xCB 90
	void res_c_2(); // 0xCB 91
	void res_d_2(); // 0xCB 92
	void res_e_2(); // 0xCB 93
	void res_h_2(); // 0xCB 94
	void res_l_2(); // 0xCB 95
	void res_hl_ref_2(); // 0xCB 96

	void res_a_3(); // 0xCB 9F
	void res_b_3(); // 0xCB 98
	void res_c_3(); // 0xCB 99
	void res_d_3(); // 0xCB 9A
	void res_e_3(); // 0xCB 9B
	void res_h_3(); // 0xCB 9C
	void res_l_3(); // 0xCB 9D
	void res_hl_ref_3(); // 0xCB 9E

	void res_a_4(); // 0xCB A7
	void res_b_4(); // 0xCB A0
	void res_c_4(); // 0xCB A1
	void res_d_4(); // 0xCB A2
	void res_e_4(); // 0xCB A3
	void res_h_4(); // 0xCB A4
	void res_l_4(); // 0xCB A5
	void res_hl_ref_4(); // 0xCB A6

	void res_a_5(); // 0xCB AF
	void res_b_5(); // 0xCB A8
	void res_c_5(); // 0xCB A9
	void res_d_5(); // 0xCB AA
	void res_e_5(); // 0xCB AB
	void res_h_5(); // 0xCB AC
	void res_l_5(); // 0xCB AD
	void res_hl_ref_5(); // 0xCB AE

	void res_a_6(); // 0xCB B7
	void res_b_6(); // 0xCB B0
	void res_c_6(); // 0xCB B1
	void res_d_6(); // 0xCB B2
	void res_e_6(); // 0xCB B3
	void res_h_6(); // 0xCB B4
	void res_l_6(); // 0xCB B5
	void res_hl_ref_6(); // 0xCB B6

	void res_a_7(); // 0xCB BF
	void res_b_7(); // 0xCB B8
	void res_c_7(); // 0xCB B9
	void res_d_7(); // 0xCB BA
	void res_e_7(); // 0xCB BB
	void res_h_7(); // 0xCB BC
	void res_l_7(); // 0xCB BD
	void res_hl_ref_7(); // 0xCB BE

	// SET (set bit)
	void set_a_0(); // 0xCB C7
	void set_b_0(); // 0xCB C0
	void set_c_0(); // 0xCB C1
	void set_d_0(); // 0xCB C2
	void set_e_0(); // 0xCB C3
	void set_h_0(); // 0xCB C4
	void set_l_0(); // 0xCB C5
	void set_hl_ref_0(); // 0xCB C6

	void set_a_1(); // 0xCB CF
	void set_b_1(); // 0xCB C8
	void set_c_1(); // 0xCB C9
	void set_d_1(); // 0xCB CA
	void set_e_1(); // 0xCB CB
	void set_h_1(); // 0xCB CC
	void set_l_1(); // 0xCB CD
	void set_hl_ref_1(); // 0xCB CE

	void set_a_2(); // 0xCB D7
	void set_b_2(); // 0xCB D0
	void set_c_2(); // 0xCB D1
	void set_d_2(); // 0xCB D2
	void set_e_2(); // 0xCB D3
	void set_h_2(); // 0xCB D4
	void set_l_2(); // 0xCB D5
	void set_hl_ref_2(); // 0xCB D6

	void set_a_3(); // 0xCB DF
	void set_b_3(); // 0xCB D8
	void set_c_3(); // 0xCB D9
	void set_d_3(); // 0xCB DA
	void set_e_3(); // 0xCB DB
	void set_h_3(); // 0xCB DC
	void set_l_3(); // 0xCB DD
	void set_hl_ref_3(); // 0xCB DE

	void set_a_4(); // 0xCB E7
	void set_b_4(); // 0xCB E0
	void set_c_4(); // 0xCB E1
	void set_d_4(); // 0xCB E2
	void set_e_4(); // 0xCB E3
	void set_h_4(); // 0xCB E4
	void set_l_4(); // 0xCB E5
	void set_hl_ref_4(); // 0xCB E6

	void set_a_5(); // 0xCB EF
	void set_b_5(); // 0xCB E8
	void set_c_5(); // 0xCB E9
	void set_d_5(); // 0xCB EA
	void set_e_5(); // 0xCB EB
	void set_h_5(); // 0xCB EC
	void set_l_5(); // 0xCB ED
	void set_hl_ref_5(); // 0xCB EE

	void set_a_6(); // 0xCB F7
	void set_b_6(); // 0xCB F0
	void set_c_6(); // 0xCB F1
	void set_d_6(); // 0xCB F2
	void set_e_6(); // 0xCB F3
	void set_h_6(); // 0xCB F4
	void set_l_6(); // 0xCB F5
	void set_hl_ref_6(); // 0xCB F6

	void set_a_7(); // 0xCB FF
	void set_b_7(); // 0xCB F8
	void set_c_7(); // 0xCB F9
	void set_d_7(); // 0xCB FA
	void set_e_7(); // 0xCB FB
	void set_h_7(); // 0xCB FC
	void set_l_7(); // 0xCB FD
	void set_hl_ref_7(); // 0xCB FE


	/********************************
	 * Section 3.3.8, p. 108: Jumps *
	 ********************************/

	void jp_nn(); // 0xC3

	// JP cc,nn
	void jp_nz_nn(); // 0xC2
	void jp_z_nn();   // 0xCA
	void jp_nc_nn(); // 0xD2
	void jp_c_nn();   // 0xDA

	void jp_hl(); // 0xE9

	void jr_n(); // 0x18 // TODO: PC gets incremented here because of reading. Maybe decrement is needed

	// JR CC,n
	void jr_nz_n(); // 0x20
	void jr_z_n();   // 0x28
	void jr_nc_n(); // 0x30
	void jr_c_n();   // 0x38

	/********************************
	 * Section 3.3.9, p. 108: Calls *
	 ********************************/

	void call_nn(); // 0xCD

	// CALL cc,nn
	void call_nz_nn(); // 0xC4
	void call_z_nn();   // 0xCC
	void call_nc_nn(); // 0xD4
	void call_c_nn();   // 0xDC

	/************************************
	 * Section 3.3.10, p. 108: Restarts *
	 ************************************/

	// RST, n
	void rst_00(); // 0xC7
	void rst_08(); // 0xCF
	void rst_10(); // 0xD7
	void rst_18(); // 0xDF
	void rst_20(); // 0xE7
	void rst_28(); // 0xEF
	void rst_30(); // 0xF7
	void rst_38(); // 0xFF

	/************************************
	 * Section 3.3.11, p. 108: Returns  *
	 ************************************/

	void ret(); // 0xC9

	void ret_nz(); // 0xC0
	void ret_z();   // 0xC8
	void ret_nc(); // 0xD0
	void ret_c();   // 0xD8

	void reti(); // 0xD9

private:
	bool stop_ = false;
	bool halt_ = false;
	bool interruptable_ = false;

	/************************************************
	 * Helper Functions for Read/Write Instructions *
	 ************************************************/

	// Read Program Counter, 8-bit
	[[nodiscard]] inline u8 read_pc() {
		u8 val =  mem->read(PC);
	//	printf(" %x", val);
		PC++;
		return val;
	}

	// Read Program Ccounter, 16-bit
	[[nodiscard]] inline u16 read16_pc() {
		u16 val = read_pc();
		val |= read_pc() << 8;
		return val;
	}

	// Read Stack, 8-bit
	[[nodiscard]] inline u8 pop() {
		SP++;
		return mem->read(SP);
	}

	// Read Stack, 16-bit
	[[nodiscard]] inline u16 pop16() {
		u16 val = pop() << 8;
		val |= pop();
		return val;
	}

	// Write Stack, 8-bit
	inline void push(u8 val) {
		mem->write(SP, val);
		SP--;
	}

	// Write Stack, 16-bit
	inline void push16(u16 val) {
		push(val & 0xFF);
		push(val >> 8);
	}

	// Read from arbitrary address, 8-bit
	inline u8 read(u16 addr) {
		return mem->read(addr);
	}

	// Write to arbitrary address, 8-bit
	inline void write(u16 addr, u8 val) {
		mem->write(addr, val);
	}

	// Write to arbitrary address, 16-bit
	inline void write16(u16 addr, u16 val) {
		mem->write(addr, val & 0xFF);
		mem->write(addr + 1, val >> 8);

	}


	/************************************************
	 * Helper Functions for Arithmetic Instructions *
	 ************************************************/

	[[nodiscard]] inline u8 add8bit(u8 op1, u8 op2) {
		u16 result = op1 + op2;
		flags.c = 0x100 == ((op1 ^ op2 ^ result) & 0x100);
		flags.h = 0x10 == ((op1 ^ op2 ^ result) & 0x10);
		flags.z = result == 0;
		flags.n = false;
		return (u8)(result & 0xFF);

	}

	[[nodiscard]] inline u8 adc8bit(u8 op1, u8 op2) {
		u16 result = op1 + op2 + flags.c;
		flags.c = 0x100 == ((op1 ^ op2 ^ result) & 0x100);
		flags.h = 0x10 == ((op1 ^ op2 ^ result) & 0x10);
		flags.z = result == 0;
		flags.n = false;
		return (u8)(result & 0xFF);
	}

	[[nodiscard]] inline u16 add16bit(u16 op1, u16 op2) {
		u32 result = op1 + op2;
		flags.n = false;
		flags.c = 0x1'00'00 == ((op1 ^ op2 ^ result) & 0x1'00'00);
		flags.h = 0x10'00 == ((op1 ^ op2 ^ result) & 0x10'00);
		return (u16)result;

	}

	[[nodiscard]] inline u8 sub8bit(u8 op1, u8 op2) {
		flags.c = op1 < op2;
		flags.h = (op1 & 0x0F) < (op2 & 0x0F);
		flags.z = op1 == op2;
		flags.n = true;
		return (u8)(op1 - op2);
	}

	[[nodiscard]] inline u8 sbc8bit(u8 op1, u8 op2) {
		op2 = op2 + flags.c;
		flags.c = op1 < op2;
		flags.h = (op1 & 0x0F) < (op2 & 0x0F);
		flags.z = op1 == op2;
		flags.n = true;
		return (u8)(op1 - op2);
	}

	[[nodiscard]] inline u8 and8bit(u8 op1, u8 op2) {
		u8 result = op1 & op2;
		flags.z = result == 0;
		flags.n = false;
		flags.h = true;
		flags.c = false;
		return result;
	};

	[[nodiscard]] inline u8 or8bit(u8 op1, u8 op2) {
		u8 result = op1 | op2;
		flags.z = result == 0;
		flags.n = false;
		flags.h = false;
		flags.c = false;
		return result;
	}

	[[nodiscard]] inline u8 xor8bit(u8 op1, u8 op2) {
		u8 result = op1 ^ op2;
		flags.z = op1 == 0;
		flags.n = false;
		flags.h = false;
		flags.c = false;
		return result;
	}

	inline void inc(u8* addr) {
		flags.n = false;
		flags.h = 0x0F == (*addr & 0x0F);
		(*addr)++;
		flags.z = 0 == *addr;
	}

	inline void dec(u8* addr) {
		(*addr)--;
		flags.z = 0 == *addr;
		flags.n = true;
		flags.h = (*addr & 0x0F) == 0x0F;
	}

	/***************************************************
	 * Helper Functions for Miscellaneous Instructions *
	 ***************************************************/

	[[nodiscard]] inline u8 swap(u8 val) {
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;
		flags.c = false;

		u8 tmp = val & 0x0F;
		val = val >> 4;
		val = val | (tmp << 4);

		return val;
	}

	/****************************************************
	 * Helper Functions for Rotate & Shift Instructions *
	 ****************************************************/

	[[nodiscard]] inline u8 rlc(u8 val) {
		flags.c = (val & 0x80);
		val <<= 1;
		val |= flags.c;

		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] inline u8 rl(u8 val) {
		bool carry = (val & 0x80);
		val <<= 1;
		val |= flags.c;

		flags.c = carry;
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] inline u8 rrc(u8 val) {
		flags.c = val & 0x01;
		val >>= 1;
		val |= (flags.c << 7);

		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] u8 rr(u8 val) { // 0x1F
		bool carry = (val & 0x01);
		val >>= 1;
		val |= (flags.c << 7);

		flags.c = carry;
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] u8 sla(u8 val) {
		flags.c = (val & 0x80);
		val <<= 1;
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] u8 sra(u8 val) {
		u8 msb = val & 0x80;
		flags.c = (val & 0x01);
		val >>= 1;
		val |= msb;
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	[[nodiscard]] u8 srl(u8 val) {
		flags.c = val & 0x01;
		val >>= 1;
		flags.z = val == 0;
		flags.n = false;
		flags.h = false;

		return val;
	}

	/*****************************************
	 * Helper Functions for Bit Instructions *
	 *****************************************/

	void bit(u8 bit, u8 reg) {
		flags.z = !(reg & (0x01 << bit));
		flags.n = false;
		flags.h = true;
	}

	[[nodiscard]] u8 set(u8 bit, u8 reg) {
		return reg | (0x01 << bit);
	}

	[[nodiscard]] u8 res(u8 bit, u8 reg) {
		return reg & ~(0x01 << bit);
	}
};

} /* namespace */
