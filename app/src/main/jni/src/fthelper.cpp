//
// Created by eltonjiang on 2018/6/12.
//
#include "fthelper.h"
#include "common.h"

void checkFT_Error(FT_Error error, const char *msg)
{
    if (error) {
        LOGE("ERROR %02x, %s", error, msg);
    }
}