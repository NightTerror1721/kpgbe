#include "mmu.h"

#include "range.h"


#define INTERNAL_RAM_SIZE 8_KB


#define ADDRESS_RANGE(_From, _ToExclusive) typedef DECL_RANGE(Address, (_From), (_ToExclusive) - 1) 
ADDRESS_RANGE(0, 0x100) GameBoyBiosRange;
ADDRESS_RANGE(0, 0x800) GameBoyColorBiosRange;
ADDRESS_RANGE(0xC000, 0xE000) InternalRamRange;
ADDRESS_RANGE(0xE000, 0xFE00) EchoInternalRamRange;


MMU::MMU(const Bios::Type bios) :
	_bios{ bios },
	_biosMode{ true },
	_internalRAM{ INTERNAL_RAM_SIZE }
{}
MMU::~MMU()
{

}

Byte MMU::read(const Address addr) const
{
	switch (addr & 0xF000)
	{
		/* ROM bank #0 */
		case 0x0000:
			if (_biosMode)
			{
				if (GameBoyBiosRange::contains(addr))
					return _bios[addr];
				else if (GameBoyColorBiosRange::contains(addr) && _bios.isGBC())
					return _bios[addr];
			}
		case 0x1000:
		case 0x2000:
		case 0x3000:
			break;

		/* switchable ROM bank */
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			break;

		/* Video RAM */
		case 0x8000:
		case 0x9000:
			break;

		/* switchable RAM bank */
		case 0xA000:
		case 0xB000:
			break;

		/* Internal RAM */
		case 0xC000:
		case 0xD000:
			return _internalRAM.read(InternalRamRange::index(addr));

		/* Echo of Internal RAM (0xE000 to 0xFDFF) */
		case 0xE000:
		case 0xF000:

			/* Shadow ram */
			if (EchoInternalRamRange::contains(addr))
				return _internalRAM.read(EchoInternalRamRange::index(addr));

			/* OAM */
			else if (addr < 0xFEA0)
				break;

			/* ??? */
			else if (addr < 0xFF80);

			/* Interrupts */
			else break;
			break;
	}

	return 0;
}

void MMU::write(const Address addr, const Byte value)
{
	switch (addr & 0xF000)
	{
		/* ROM bank #0 */
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
			break;

		/* switchable ROM bank */
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			break;

		/* Video RAM */
		case 0x8000:
		case 0x9000:
			break;

		/* switchable RAM bank */
		case 0xA000:
		case 0xB000:
			break;

		/* Internal RAM */
		case 0xC000:
		case 0xD000:
			_internalRAM.write(InternalRamRange::index(addr), value);
			break;

		/* Echo of Internal RAM (0xE000 to 0xFDFF) */
		case 0xE000:
		case 0xF000:

			/* Shadow ram */
			if (EchoInternalRamRange::contains(addr))
				_internalRAM.write(EchoInternalRamRange::index(addr), value);

			/* OAM */
			else if (addr < 0xFEA0)
				break;

			/* ??? */
			else if (addr < 0xFF80);

			/* Interrupts */
			else break;
			break;
	}
}

Word MMU::readWord(const Address addr) const
{
	return static_cast<Word>((read(addr) & 0xffU) << 8) | static_cast<Word>(read(addr + 1) & 0xffU);
}
void MMU::writeWord(const Address addr, const Word value)
{
	write(addr, static_cast<Byte>((value >> 8) & 0xffU));
	write(addr + 1, static_cast<Byte>(value & 0xffU));
}
