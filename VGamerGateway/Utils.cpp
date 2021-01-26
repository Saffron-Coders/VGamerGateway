#include <cstdio>
#include "Utils.h"

uint16_t Utils::bytes2short(const uint8_t buff[2])
{
    return bytes2short(buff[0], buff[1]);
}

uint16_t Utils::bytes2short(const uint8_t msb, const uint8_t lsb)
{
    uint16_t ret;
    ret = ((uint16_t)msb << 8) | lsb;
    return ret;
}

void Utils::printHex(const uint8_t* buff, size_t len)
{
    for (int i = 0; i < len; ++i) {
        printf("%02x ", buff[i]);
    }
    puts("");
}
