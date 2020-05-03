#pragma once

#include "common.h"

#define ENABLED_FLAG 1
#define DISABLED_FLAG 0


typedef u8 Reg8;
typedef u16 Reg16;

struct Registers
{
	union
	{
		struct
		{
			union
			{
				struct
				{
					Reg8 : 4;
					Reg8 carryFlag : 1;
					Reg8 halfCarryFlag : 1;
					Reg8 subtractFlag : 1;
					Reg8 zeroFlag : 1;
				};
				Reg8 F;
			};
			Reg8 A;
		};
		Reg16 AF;
	};

	union
	{
		struct { Reg8 C, B; };
		Reg16 BC;
	};

	union
	{
		struct { Reg8 E, D; };
		Reg16 DE;
	};

	union
	{
		struct { Reg8 L, H; };
		Reg16 HL;
	};

	Reg16 SP;

	Reg16 PC;


public:
	enum class Flag : Reg8
	{
		Carry = 0x1 << 4,
		HalfCarry = 0x1 << 5,
		Subtract = 0x1 << 6,
		Zero = 0x1 << 7
	};

public:
	Registers();
	Registers(const Registers&) = default;

	Registers& operator= (const Registers&) = default;

	void reset();

	inline void setCarryFlag() { carryFlag = ENABLED_FLAG; }
	inline void setHalfCarryFlag() { halfCarryFlag = ENABLED_FLAG; }
	inline void setSubtractFlag() { subtractFlag = ENABLED_FLAG; }
	inline void setZeroFlag() { zeroFlag = ENABLED_FLAG; }
	inline void setFlag(const Flag flag) { F |= static_cast<Reg8>(flag); }
	inline void setFlags(const Flag flag0, const Flag flag1)
	{
		F |= static_cast<Reg8>(flag0) |
			 static_cast<Reg8>(flag1);
	}
	inline void setFlags(const Flag flag0, const Flag flag1, const Flag flag2)
	{
		F |= static_cast<Reg8>(flag0) |
			 static_cast<Reg8>(flag1) |
			 static_cast<Reg8>(flag2);
	}
	inline void setAllFlags()
	{
		F |= static_cast<Reg8>(Flag::Carry) |
			 static_cast<Reg8>(Flag::HalfCarry) |    
			 static_cast<Reg8>(Flag::Subtract) |
			 static_cast<Reg8>(Flag::Zero);
	}

	inline void clearCarryFlag() { carryFlag = DISABLED_FLAG; }
	inline void clearHalfCarryFlag() { halfCarryFlag = DISABLED_FLAG; }
	inline void clearSubtractFlag() { subtractFlag = DISABLED_FLAG; }
	inline void clearZeroFlag() { zeroFlag = DISABLED_FLAG; }
	inline void clearFlag(const Flag flag) { F &= ~static_cast<Reg8>(flag); }
	inline void clearFlags(const Flag flag0, const Flag flag1)
	{
		F &= ~(static_cast<Reg8>(flag0) |
			   static_cast<Reg8>(flag1)
		);
	}
	inline void clearFlags(const Flag flag0, const Flag flag1, const Flag flag2)
	{
		F &= ~(static_cast<Reg8>(flag0) |
			   static_cast<Reg8>(flag1) |
			   static_cast<Reg8>(flag2)
		);
	}
	inline void clearAllFlags()
	{
		F &= ~(static_cast<Reg8>(Flag::Carry) |
			   static_cast<Reg8>(Flag::HalfCarry) |
			   static_cast<Reg8>(Flag::Subtract) |
			   static_cast<Reg8>(Flag::Zero)
		);
	}

	friend std::ostream& operator<< (std::ostream& os, const Registers& regs);
};



