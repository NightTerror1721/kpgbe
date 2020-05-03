#include "ram.h"

RAM::RAM(const size_t size) :
	_mem{ new Byte[size] },
	_size{ size }
{}
RAM::~RAM()
{
	delete[] _mem;
}

size_t RAM::size() const { return _size; }

void RAM::dump(std::ostream& os, const size_t bytesPerRow) { DumpBytesToStream(os, _mem, _size, bytesPerRow); }

void RAM::write(const Address addr, const Byte value) { _mem[addr % _size] = value; }
Byte RAM::read(const Address addr) const { return _mem[addr % _size]; }

Byte& RAM::operator[] (const Address& addr) { return _mem[addr % _size]; }
const Byte& RAM::operator[] (const Address& addr) const { return _mem[addr % _size]; }

std::ostream& operator<< (std::ostream& os, const RAM& ram)
{
	return DumpBytesToStream(os, ram._mem, ram._size);
}
