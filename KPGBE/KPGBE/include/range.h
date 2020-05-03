#pragma once

#include "common.h"

template<typename _Ty, _Ty _Min, _Ty _Max>
class Range
{
public:
	static constexpr _Ty min() { return _Min; }
	static constexpr _Ty max() { return _Max; }

	static constexpr bool contains(const _Ty value) { return (_Min <= value) && (value <= _Max); }

	static constexpr _Ty clamp(const _Ty value) { return ::clamp<_Ty>(value, _Min, _Max); }

	static constexpr _Ty index(const _Ty value) { return ::clamp<_Ty>(value, _Min, _Max) - _Min; }
};

#define DECL_RANGE(_Type, _Min, _Max) Range<_Type, _Min, _Max>
