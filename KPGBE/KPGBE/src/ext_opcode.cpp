#include "opcodes.h"

#include "vm.h"

#ifndef BYTE
#define BYTE Byte
#endif

#ifndef WORD
#define WORD Word
#endif

#define __VM vm
#define BASE_ARGS VirtualMachine& __VM

#define OPCODE opcode
#define OPCODE_ARG const Byte OPCODE

#define __HL __VM.regs.HL

#define READ_BYTE(offset) __VM.mmu.read((offset))
#define READ_WORD(offset) __VM.mmu.readWord((offset))

#define WRITE_BYTE(offset, value) __VM.mmu.write((offset), (value))
#define WRITE_WORD(offset, value) __VM.mmu.writeWord((offset), (value))

#define ZERO_FLAG __VM.regs.zeroFlag
#define SUBTRACT_FLAG __VM.regs.subtractFlag
#define HALFCARRY_FLAG __VM.regs.halfCarryFlag
#define CARRY_FLAG __VM.regs.carryFlag

#define SET_FLAG(flag) (flag) = ENABLED_FLAG
#define CLEAR_FLAG(flag) (flag) = DISABLED_FLAG

#define SET_IF_ELSE_CLEAR(flag, if_statement) if(if_statement) SET_FLAG(flag); else CLEAR_FLAG(flag)
#define CLEAR_IF_ELSE_SET(flag, if_statement) if(if_statement) CLEAR_FLAG(flag); else SET_FLAG(flag)


enum class RegisterId { B, C, D, E, H, L, HL, A };

#define REG_ID reg_id
#define REG_ID_ARG const RegisterId REG_ID


enum class Bit : Byte
{
	b0 = (1 << 0),
	b1 = (1 << 1),
	b2 = (1 << 2),
	b3 = (1 << 3),
	b4 = (1 << 4),
	b5 = (1 << 5),
	b6 = (1 << 6),
	b7 = (1 << 7)
};

#define BIT __bit
#define BIT_ARG const Bit BIT
#define BIT_VALUE static_cast<Byte>(BIT)


static constexpr bool is_hlp(REG_ID_ARG) { return REG_ID == RegisterId::HL; }

static inline Reg8& select(BASE_ARGS, REG_ID_ARG)
{
	switch (REG_ID)
	{
		case RegisterId::B: return __VM.regs.B;
		case RegisterId::C: return __VM.regs.C;
		case RegisterId::D: return __VM.regs.D;
		case RegisterId::E: return __VM.regs.E;
		case RegisterId::H: return __VM.regs.H;
		case RegisterId::L: return __VM.regs.L;
		case RegisterId::A: return __VM.regs.A;
		default: return __VM.regs.A;
	}
}

static constexpr RegisterId get_register_id(OPCODE_ARG) { return static_cast<RegisterId>(OPCODE % 8U); }

static constexpr Bit get_bit(OPCODE_ARG) { return static_cast<Bit>((OPCODE % 64U) / 8U); }


#define GET_REG_ID() get_register_id(OPCODE)
#define DECL_REG_ID REG_ID_ARG = GET_REG_ID()

#define GET_BIT() get_bit(OPCODE)

#define HLP is_hlp(REG_ID)
#define REG select(__VM, REG_ID)
#define TICKS (HLP ? 16 : 8)

#define def_extopcode(_Name) void _Name (BASE_ARGS, REG_ID_ARG)
#define def_bit_extopcode(_Name) void _Name (BASE_ARGS, REG_ID_ARG, BIT_ARG)




def_extopcode(rlc)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	int carry = (value & 0x80) >> 7;
	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x80);

	value <<= 1;
	value += carry;

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(rrc)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	int carry = value & 0x01;

	value >>= 1;
	if (carry)
	{
		SET_FLAG(CARRY_FLAG);
		value |= 0x80;
	}
	else CLEAR_FLAG(CARRY_FLAG);

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(rl)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	int carry = CARRY_FLAG ? 1 : 0;
	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x80);

	value <<= 1;
	value += carry;

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(rr)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	value >>= 1;
	if (CARRY_FLAG)
		value |= 0x80;

	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x01);

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(sla)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x01);
	value <<= 1;

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(sra)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x01);
	value = (value & 0x80) | (value >> 1);

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(swap)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	value = ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);
	CLEAR_FLAG(CARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_extopcode(srl)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	SET_IF_ELSE_CLEAR(CARRY_FLAG, value & 0x01);
	value >>= 1;

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value);
	CLEAR_FLAG(SUBTRACT_FLAG);
	CLEAR_FLAG(HALFCARRY_FLAG);

	if (HLP)
		WRITE_BYTE(__HL, value);
	else REG = value;
}


def_bit_extopcode(bit)
{
	BYTE value = HLP ? READ_BYTE(__HL) : REG;

	CLEAR_IF_ELSE_SET(ZERO_FLAG, value & BIT_VALUE);
	CLEAR_FLAG(SUBTRACT_FLAG);
	SET_FLAG(HALFCARRY_FLAG);
}


def_bit_extopcode(res)
{
	if (HLP)
		WRITE_BYTE(__HL, READ_BYTE(__HL) & ~BIT_VALUE);
	else REG = REG & ~BIT_VALUE;
}


def_bit_extopcode(set)
{
	if (HLP)
		WRITE_BYTE(__HL, READ_BYTE(__HL) | BIT_VALUE);
	else REG = REG | BIT_VALUE;
}






namespace ExtendedOpcode
{
	void execute(VirtualMachine& __VM, Byte OPCODE)
	{
		DECL_REG_ID;
		switch (OPCODE & 0xC0)
		{
			case 0x00:
				switch (OPCODE / 8)
				{
					case 0:
						rlc(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 1:
						rrc(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 2:
						rl(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 3:
						rr(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 4:
						sla(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 5:
						sra(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 6:
						swap(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;

					case 7:
						srl(__VM, REG_ID);
						__VM.cpu.increaseTicks(TICKS);
						break;
				}
				break;

			case 0x40:
				bit(__VM, REG_ID, GET_BIT());
				__VM.cpu.increaseTicks(TICKS);
				break;

			case 0x80:
				res(__VM, REG_ID, GET_BIT());
				__VM.cpu.increaseTicks(TICKS);
				break;

			case 0xC0:
				set(__VM, REG_ID, GET_BIT());
				__VM.cpu.increaseTicks(TICKS);
				break;
		}
	}
}
