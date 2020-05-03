#pragma once

#include "common.h"

class VirtualMachine;

typedef void (*VoidOpcodeFunction) (VirtualMachine&);
typedef void (*ByteOpcodeFunction) (VirtualMachine&, Byte);
typedef void (*WordOpcodeFunction) (VirtualMachine&, Word);


class Opcode
{
public:
	enum class Type { Invalid, NoArgs, ByteArg, WordArg };

private:
	std::string _name;
	Type _type;
	void* _func;

public:
	Opcode();
	Opcode(const std::string& name, VoidOpcodeFunction opcodeFunction);
	Opcode(const std::string& name, ByteOpcodeFunction opcodeFunction);
	Opcode(const std::string& name, WordOpcodeFunction opcodeFunction);
	Opcode(const Opcode&) = default;
	~Opcode() = default;

	Opcode& operator= (const Opcode&) = default;

	const std::string& name() const;
	size_t length() const;

	void operator() (VirtualMachine& vm) const;
	void operator() (VirtualMachine& vm, Byte arg) const;
	void operator() (VirtualMachine& vm, Word arg) const;

public:
	static const Opcode& of(Byte code);
	static Ticks ticksOf(Byte code);

	static void executeNext(VirtualMachine& vm);

private:
	static const Opcode OPCODES[256];
	static const unsigned int TICKS[256];
};

namespace ExtendedOpcode
{
	void execute(VirtualMachine& vm, Byte opcode);
}

