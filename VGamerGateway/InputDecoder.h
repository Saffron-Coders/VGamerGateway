#pragma once

#include <stdint.h>

class InputDecoder
{
public:
	int decode(const char* msg, size_t len);
};

