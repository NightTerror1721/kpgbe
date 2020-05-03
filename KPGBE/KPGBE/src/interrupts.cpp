#include "interrupts.h"

#include "vm.h"

#define CHECK_INT(interrupt) if(enabled_##interrupt) { int_##interrupt = DISABLED_FLAG; interrupt(vm); }


Interrupts::Interrupts() :
	master{ false },
	enabled{ 0 },
	flags{ 0 }
{}

void Interrupts::step(VirtualMachine& vm)
{
	if (master && enabled && flags)
	{
		CHECK_INT(vblank)
		CHECK_INT(lcdstat)
		CHECK_INT(timer)
		CHECK_INT(serial)
		CHECK_INT(joypad)
	}
}

void Interrupts::reset()
{
	master = false;
	enabled = 0;
	flags = 0;
}

void Interrupts::vblank(VirtualMachine& vm)
{
	// TODO: drawFrameBuffer();

	master = false;
	vm.stack.pushWord(vm.regs.PC);
	vm.regs.PC = 0x40;
	vm.cpu.increaseTicks(12);
}

void Interrupts::lcdstat(VirtualMachine& vm)
{
	master = false;
	vm.stack.pushWord(vm.regs.PC);
	vm.regs.PC = 0x48;
	vm.cpu.increaseTicks(12);
}

void Interrupts::timer(VirtualMachine& vm)
{
	master = false;
	vm.stack.pushWord(vm.regs.PC);
	vm.regs.PC = 0x50;
	vm.cpu.increaseTicks(12);
}

void Interrupts::serial(VirtualMachine& vm)
{
	master = false;
	vm.stack.pushWord(vm.regs.PC);
	vm.regs.PC = 0x58;
	vm.cpu.increaseTicks(12);
}

void Interrupts::joypad(VirtualMachine& vm)
{
	master = false;
	vm.stack.pushWord(vm.regs.PC);
	vm.regs.PC = 0x60;
	vm.cpu.increaseTicks(12);
}

void Interrupts::returnFromInterrupt(VirtualMachine& vm)
{
	master = true;
	vm.regs.PC = vm.stack.popWord();
}
