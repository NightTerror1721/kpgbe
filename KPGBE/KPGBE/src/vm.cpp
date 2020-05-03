#include "vm.h"

VirtualMachine::VirtualMachine(const Bios::Type bios) :
	mmu{ bios },
	cpu{},
	regs{},
	ints{},
	stack{ *this }
{}
VirtualMachine::~VirtualMachine() {}

void VirtualMachine::reset()
{
	cpu.reset();
	regs.reset();
	ints.reset();
}



VirtualMachine::Stack::Stack(VirtualMachine& vm) :
	_vm{ vm }
{}

void VirtualMachine::Stack::pushByte(Byte value)
{
	_vm.mmu.write(--_vm.regs.SP, value);
}
void VirtualMachine::Stack::pushWord(Word value)
{
	_vm.mmu.writeWord(_vm.regs.SP -= 2, value);
}

Byte VirtualMachine::Stack::popByte()
{
	return _vm.mmu.read(_vm.regs.SP++);
}
Word VirtualMachine::Stack::popWord()
{
	Word value = _vm.mmu.readWord(_vm.regs.SP);
	_vm.regs.SP += 2;
	return value;
}
