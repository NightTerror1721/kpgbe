#pragma once

#include "common.h"


class VirtualMachine;

struct Interrupts
{
	bool master;
	union
	{
		struct
		{
			u8 enabled_vblank : 1;
			u8 enabled_lcdstat : 1;
			u8 enabled_timer : 1;
			u8 enabled_serial : 1;
			u8 enabled_joypad : 1;
			u8 : 3;
		};
		u8 enabled;
	};
	union
	{
		struct
		{
			u8 int_vblank : 1;
			u8 int_lcdstat : 1;
			u8 int_timer : 1;
			u8 int_serial : 1;
			u8 int_joypad : 1;
			u8 : 3;
		};
		u8 flags;
	};

public:
	Interrupts();
	Interrupts(const Interrupts&) = default;

	Interrupts& operator= (const Interrupts&) = default;

	void step(VirtualMachine& vm);

	void reset();

	void vblank(VirtualMachine& vm);
	void lcdstat(VirtualMachine& vm);
	void timer(VirtualMachine& vm);
	void serial(VirtualMachine& vm);
	void joypad(VirtualMachine& vm);

	void returnFromInterrupt(VirtualMachine& vm);
};
