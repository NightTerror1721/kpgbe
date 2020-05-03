#pragma once

#include "cpu.h"
#include "mmu.h"
#include "registers.h"
#include "interrupts.h"


class VirtualMachine
{
public:
	MMU mmu;
	CPU cpu;
	Registers regs;
	Interrupts ints;

public:
	VirtualMachine(const Bios::Type bios);
	~VirtualMachine();

	void reset();


public:
	class Stack
	{
	private:
		VirtualMachine& _vm;

	public:
		Stack(VirtualMachine& vm);

		void pushByte(Byte value);
		void pushWord(Word value);

		Byte popByte();
		Word popWord();
	};
	Stack stack;
};

