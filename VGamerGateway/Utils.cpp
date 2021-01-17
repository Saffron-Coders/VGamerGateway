#include <cstdio>
#include "Utils.h"

uint16_t Utils::bytes2short(const uint8_t buff[2])
{
    uint16_t ret;
    ret = ((uint16_t)buff[0] << 8) | buff[1];
    return ret;
}

void Utils::printHex(const uint8_t* buff, size_t len)
{
    for (int i = 0; i < len; ++i) {
        printf("%02x ", buff[i]);
    }
    puts("");
}
