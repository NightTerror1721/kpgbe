#pragma once

#include "common.h"
#include "bios.h"
#include "ram.h"


class MMU
{
private:
	Bios _bios;
	bool _biosMode;

	RAM _internalRAM;

public:
	MMU(const Bios::Type bios);
	~MMU();

	Byte read(const Address addr) const;
	void write(const Address addr, const Byte value);

	Word readWord(const Address addr) const;
	void writeWord(const Address addr, const Word value);
};
