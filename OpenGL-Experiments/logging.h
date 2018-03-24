#pragma once
#include <string>


#define NO_LOG 0
#define ERR 1
#define WARN 2
#define INFO 3
#define DEBUG 4

#ifndef LOG_FILE
#define LOG_FILE "debug.log"
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif


std::string curTime();

void log(std::string err, int errLevel=2);

void logClose();
