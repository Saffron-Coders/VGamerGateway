#include "Utils.h"

uint16_t Utils::bytes2short(const char buff[2])
{
    uint16_t ret;
    ret = ((uint16_t)buff[0] << 8) | buff[1];
    return ret;
}
