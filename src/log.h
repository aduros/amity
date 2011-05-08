#pragma once

#ifdef __ANDROID__
#include <android/log.h>

#define LOG_TAG "amity"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else
#include <syslog.h>

#define LOGI(...) syslog(LOG_INFO, __VA_ARGS__);
#define LOGW(...) syslog(LOG_WARNING, __VA_ARGS__);
#define LOGE(...) syslog(LOG_ERR, __VA_ARGS__);

#endif
