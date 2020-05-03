#pragma once

#include "common.h"

class Bios
{
private:
	static const u8 GB_BIOS_BYTES[256];
	static const u8 GBC_BIOS_BYTES[2048];

public:
	enum class Type { GameBoy, GameBoyColor };

private:
	Type _type;

public:
	Bios(const Type type);
	Bios(const Bios&) = default;

	Bios& operator= (const Bios&) = default;

	bool isGB() const;
	bool isGBC() const;

	u8 operator[] (const Address addr) const;

public:
	static Bios gameBoyBios();
	static Bios gameBoyColorBios();
};
