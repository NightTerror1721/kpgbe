#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>


#define OK true
#define ERROR false
#define KILOBYTE (0x1U << 0xAU)
#define MEGABYTE (KILOBYTE << 0xAU)
#define GIGABYTE (MEGABYTE << 0xAU)
#define INVALID_TICKS (~0ULL)

#define SUCCESS(_X) ((_X) == OK)
#define PRINT_ERROR(...) std::fprintf(stderr, __VA_ARGS__)
#define CHECK_MSG(_X, ...)								\
	if(!(_X)) {											\
		PRINT_ERROR("%s:%d: ", __FILE__, __LINE__);		\
		PRINT_ERROR(__VA_ARGS__);						\
		goto __error;									\
	}
#define CHECK(_X) do if (!(_X)) { goto __error; } while(0)
#define ON_ERROR_RETURN \
	__error:			\
	return ERROR
#define ON_ERROR_CLOSE_STREAM_AND_RETURN(_F) \
	__error: \
	if(_F) { (_F).close(); } \
	return ERROR
#define UNREACHABLE(...) PRINT_ERROR(__VA_ARGS__), exit(1)

inline bool is_little_endian()
{
	unsigned int i = 1;
	char* c = reinterpret_cast<char*>(&i);
	return *c;
}
inline bool is_big_endian() { return !is_little_endian(); }

const bool IsLittleEndian = is_little_endian();
const bool IsBigEndian = !IsLittleEndian;


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef u16 Address;
typedef u16 MaskedAddress;
//typedef u32 RGBA;
typedef u64 Ticks;
typedef u8 Byte;
typedef u16 Word;


struct RGBA
{
	u8 red;
	u8 green;
	u8 blue;
	u8 alpha;

	RGBA();
	RGBA(const u32 rgba);
	RGBA(const u8 red, const u8 green, const u8 blue, const u8 alpha);
	RGBA(const RGBA&) = default;
	RGBA(RGBA&&) = default;

	RGBA& operator= (const RGBA&) = default;
	RGBA& operator= (RGBA&&) = default;

	operator u32() const;

	inline u32 code() const { return static_cast<u32>(*this); }
};


struct JoypadButtons
{
	bool down, up, left, right;
	bool start, select, B, A;
};

struct FileData
{
	u8* data;
	size_t size;

	FileData();
	~FileData();

	bool read(const char* filename);
	bool write(const char* filename) const;
	void clear();

	friend bool operator!(const FileData& fd);
	operator bool() const;
};


using std::min;
using std::max;

template<typename _Ty>
constexpr _Ty clamp(const _Ty value, const _Ty min, const _Ty max) { return std::min(max, std::max(min, value)); }

template<typename _Ty>
constexpr _Ty next_modulo(const _Ty value, const _Ty mod) { return mod - value % mod; }

constexpr size_t operator"" _KB(unsigned long long value) { return static_cast<size_t>(value * KILOBYTE); }
constexpr size_t operator"" _MB(unsigned long long value) { return static_cast<size_t>(value * MEGABYTE); }
constexpr size_t operator"" _GB(unsigned long long value) { return static_cast<size_t>(value * GIGABYTE); }

constexpr size_t kilobytes(const size_t value) { return static_cast<size_t>(value * KILOBYTE); }
constexpr size_t megabytes(const size_t value) { return static_cast<size_t>(value * MEGABYTE); }
constexpr size_t gigabytes(const size_t value) { return static_cast<size_t>(value * GIGABYTE); }

template<typename _Ty>
constexpr _Ty align_up(const _Ty x, const _Ty align) { return (x + align - 1) & ~(align - 1); }

template<typename _Ty>
constexpr _Ty align_down(const _Ty x, const _Ty align) { return (x & ~(align - 1)); }

template<typename _Ty>
constexpr _Ty is_aligned(const _Ty x, const _Ty align) { return (x & (align - 1)) == 0; }

std::string ByteToHexString(const Byte value);
std::string WordToHexString(const Word value);
std::string SizeToHexString(const size_t value);
std::string AddressToHexString(const Address value);

std::ostream& DumpBytesToStream(std::ostream& os, const void* mem, const size_t size, size_t bytesPerRow = 0x10);
