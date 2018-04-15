#pragma once


#ifndef LOG_TYPE_NO
#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2
#endif

#ifndef LOG_TYPE
#define LOG_TYPE LOG_TYPE_FILE
#endif

// TODO(Derek): Possibly add FATAL
enum LogLevel {
    NO_LOG,
    ERR,
    WARN,
    INFO,
    DEBUG
};

#if LOG_TYPE == LOG_TYPE_NO

int logOpen(const char *alogPath="debug.log", LogLevel alogLevel=WARN) {}
void logClose() {}

#else // LOG_TYPE != LOG_TYPE_NO

#include <string>

// std::string curTime();

int logOpen(const char *alogPath="debug.log", LogLevel alogLevel=WARN);

void log(std::string err, LogLevel errLevel=WARN);

void logClose();

#endif // LOG_TYPE != LOG_TYPE_NO
