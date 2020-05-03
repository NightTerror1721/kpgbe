#include "common.h"

#include <fstream>


RGBA::RGBA() :
	red{ 0 },
	green{ 0 },
	blue{ 0 },
	alpha{ 0 }
{}
RGBA::RGBA(const u32 rgba) :
	red{ static_cast<u8>(rgba & 0xffu) },
	green{ static_cast<u8>((rgba >> 8) & 0xffu) },
	blue{ static_cast<u8>((rgba >> 16) & 0xffu) },
	alpha{ static_cast<u8>((rgba >> 24) & 0xffu) }
{}
RGBA::RGBA(const u8 red, const u8 green, const u8 blue, const u8 alpha) :
	red{ red },
	green{ green },
	blue{ blue },
	alpha{ alpha }
{}

RGBA::operator u32() const
{
	return static_cast<u32>(red) |
		static_cast<u32>((green << 8) & 0xffu) |
		static_cast<u32>((blue << 16) & 0xffu) |
		static_cast<u32>((alpha << 24) & 0xffu);
}


static bool GetFileSize(std::fstream& f, size_t& out_size)
{
	f.seekg(0, std::fstream::end);
	std::streampos size;
	CHECK_MSG(!f.fail(), "seekg to end failed.");

	size = f.tellg();
	CHECK_MSG(size >= 0, "tellg failed.");
	
	f.seekg(0, std::fstream::beg);
	CHECK_MSG(!f.fail(), "seekg to beginning failed.");

	out_size = static_cast<size_t>(size);
	return true;

	ON_ERROR_RETURN;
}


FileData::FileData() :
	data{ nullptr },
	size{ 0 }
{}
FileData::~FileData() { clear(); }

bool FileData::read(const char* filename)
{
	u8* temp_data = nullptr;
	size_t temp_size;

	std::fstream f{ filename, std::fstream::in | std::fstream::binary };
	CHECK_MSG(f, "unable to open file \"%s\".\n", filename);

	CHECK(SUCCESS(GetFileSize(f, temp_size)));
	temp_data = new u8[temp_size];

	f.read(reinterpret_cast<char*>(temp_data), temp_size);
	CHECK_MSG(!f.fail(), "read file failed.");

	f.close();

	clear();
	data = temp_data;
	size = temp_size;

	return true;

	ON_ERROR_CLOSE_STREAM_AND_RETURN(f);
}
bool FileData::write(const char* filename) const
{
	std::fstream f{ filename, std::fstream::out | std::fstream::binary };
	CHECK_MSG(f, "unable to open file \"%s\".\n", filename);
	CHECK_MSG(data, "empty data to write.");

	f.write(reinterpret_cast<char*>(data), size);
	CHECK_MSG(!f.fail(), "write file failed.");

	f.close();
	return true;

	ON_ERROR_CLOSE_STREAM_AND_RETURN(f);
}
void FileData::clear()
{
	if (data)
	{
		delete data;
		data = nullptr;
		size = 0;
	}
}

bool operator!(const FileData& fd) { return !fd.data; }
FileData::operator bool() const { return data; }


std::string ByteToHexString(const Byte value)
{
	static const char symbols[] = "0123456789abcdef";
	char str[] = "00";
	if (value < 0x10)
		str[1] = symbols[value];
	else
	{
		str[1] = symbols[value & 0xf];
		str[0] = symbols[(value >> 4) & 0xf];
	}
	return str;
}

template<typename _Ty>
static std::string GenericIntegerToHexString(_Ty value)
{
	static const char symbols[] = "0123456789abcdef";
	const int units = (sizeof(_Ty) > 8 ? 8 : sizeof(_Ty)) * 2;
	int remaining = 0;
	std::string res(units, '0');
	char* str = &res[0];

	while (value >= 0x10 && remaining < units)
	{
		str[units - (remaining + 1)] = symbols[value & 0xf];
		remaining++;
		value >>= 4;
	}
	str[units - (remaining + 1)] = symbols[value & 0xf];
	return res;
}

std::string WordToHexString(const Word value) { return GenericIntegerToHexString(value); }
std::string SizeToHexString(const size_t value) { return GenericIntegerToHexString(value); }
std::string AddressToHexString(const Address value) { return GenericIntegerToHexString(value); }

std::ostream& DumpBytesToStream(std::ostream& os, const void* _mem, const size_t size, size_t bytesPerRow)
{
	size_t rowCount = bytesPerRow;
	const Byte* mem = reinterpret_cast<const Byte*>(_mem);
	for (size_t i = 0; i < size; i++)
	{
		if (rowCount == bytesPerRow)
			os << SizeToHexString(i) << ": ";
		os << ByteToHexString(mem[i]) << " ";
		if (--rowCount == 0)
		{
			rowCount = bytesPerRow;
			os << std::endl;
		}
	}

	return os;
}
