#include "cpu.h"

#include "vm.h"

CPU::CPU() :
	_stop{ false },
	_ticks{ 0 }
{}
CPU::~CPU() {}

void CPU::step(VirtualMachine& vm)
{
	//TODO:
}

void CPU::reset()
{
	_stop = false;
	_ticks = 0;
}

void CPU::stop() { _stop = true; }
bool CPU::isStopped() const { return _stop; }
