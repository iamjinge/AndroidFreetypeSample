//
// Created by 姜进 on 2018/6/12.
//

#ifndef FONTDECODE_COMMON_H
#define FONTDECODE_COMMON_H

#include <android/log.h>

#define LOG_TAG "FontDecode"
#ifdef ANDROID
#  define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#  define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#  define QUOTEME_(x) #x
#  define QUOTEME(x) QUOTEME_(x)
#  define LOGI(...) printf("I/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__)
#  define LOGE(...) printf("E/" LOG_TAG "(" ")" __VA_ARGS__)
#endif

#endif //FONTDECODE_COMMON_H
