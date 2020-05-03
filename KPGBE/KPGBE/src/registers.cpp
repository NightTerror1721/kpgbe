#include "registers.h"


Registers::Registers() :
	A{ 0x01 },
	F{ 0xb0 },
	B{ 0x00 },
	C{ 0x13 },
	D{ 0x00 },
	E{ 0xd8 },
	H{ 0x01 },
	L{ 0x4d },
	SP{ 0xfffe },
	PC{ 0x0100 }
{}

void Registers::reset()
{
	A = 0x01;
	F = 0xb0;
	B = 0x00;
	C = 0x13;
	D = 0x00;
	E = 0xd8;
	H = 0x01;
	L = 0x4d;
	SP = 0xfffe;
	PC = 0x0100;
}


#define STR_R8(_X) ByteToHexString(_X)
#define STR_R16(_X) WordToHexString(_X)
std::ostream& operator<< (std::ostream& os, const Registers& regs)
{
	os << "AF[" << STR_R16(regs.AF) << "] A[" << STR_R8(regs.A) << "] F[" << STR_R8(regs.F) << "]" << std::endl;
	os << "BC[" << STR_R16(regs.BC) << "] B[" << STR_R8(regs.B) << "] C[" << STR_R8(regs.C) << "]" << std::endl;
	os << "DE[" << STR_R16(regs.DE) << "] D[" << STR_R8(regs.D) << "] E[" << STR_R8(regs.E) << "]" << std::endl;
	os << "HL[" << STR_R16(regs.HL) << "] H[" << STR_R8(regs.H) << "] L[" << STR_R8(regs.L) << "]" << std::endl;
	os << "SP[" << STR_R16(regs.SP) << "]" << std::endl;
	os << "PC[" << STR_R16(regs.PC) << "]" << std::endl;
	return os;
}
#undef STR_R8
#undef STR_R16
