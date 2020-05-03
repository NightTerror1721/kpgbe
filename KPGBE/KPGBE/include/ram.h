#pragma once

#include "common.h"


class RAM
{
private:
	Byte* _mem;
	size_t _size;

public:
	RAM(const size_t size);
	~RAM();

	size_t size() const;

	void dump(std::ostream& os, const size_t bytesPerRow = 0x10);

	void write(const Address addr, const Byte value);
	Byte read(const Address addr) const;

	Byte& operator[] (const Address& addr);
	const Byte& operator[] (const Address& addr) const;

	friend std::ostream& operator<< (std::ostream& os, const RAM& ram);
};
