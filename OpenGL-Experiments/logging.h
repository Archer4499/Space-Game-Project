#pragma once
#include <string>


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


// std::string curTime();

int logOpen(const char *alogPath="debug.log", LogLevel alogLevel=WARN);

void log(std::string err, LogLevel errLevel=WARN);

void logClose();
