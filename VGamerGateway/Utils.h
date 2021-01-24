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
	static uint16_t bytes2short(const uint8_t buff[2]);
	static uint16_t bytes2short(const uint8_t msb, const uint8_t lsb);

	static void printHex(const uint8_t* buff, size_t len);
};
