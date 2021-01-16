#pragma once

#include <cstdlib>
#include <cstring>
#include <cstdint>

class Utils
{
public:

	/// Converts a sequence of 2 bytes into a short(uint16_t).
	/// NOTE:
	/// 1st byte of @buff is MSB.
	static uint16_t bytes2short(const char buff[2]);
};
