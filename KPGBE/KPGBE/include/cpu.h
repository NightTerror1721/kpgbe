#pragma once

#include "common.h"

class VirtualMachine;

class CPU
{
private:
	bool _stop;
	Ticks _ticks;

public:
	CPU();
	~CPU();

	void step(VirtualMachine& vm);

	void reset();

	void stop();
	bool isStopped() const;

	inline void increaseTicks(unsigned int ticks) { _ticks += static_cast<Ticks>(ticks); }
	inline void decreaseTicks(unsigned int ticks) { _ticks -= static_cast<Ticks>(ticks); }
	inline Ticks ticks() const { return _ticks; }
};
