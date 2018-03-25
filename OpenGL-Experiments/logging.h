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

enum LogLevel {
    NO_LOG,
    ERR,
    WARN,
    INFO,
    DEBUG
};


// std::string curTime();

int logOpen(std::string logPath="debug.log", int logLevel=2);

void log(std::string err, int errLevel=2);

void logClose();
