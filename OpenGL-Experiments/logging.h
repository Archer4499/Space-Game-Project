#pragma once


#ifndef LOG_TYPE_NO
#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2
#endif

#ifndef LOG_TYPE
#define LOG_TYPE LOG_TYPE_FILE
#endif

// #define LOG_STREAM

// TODO(Derek): Possibly add FATAL
enum LogLevel {
    NO_LOG,
    FATAL,
    ERR,
    WARN,
    INFO,
    DEBUG
};

#if LOG_TYPE == LOG_TYPE_NO

#define LOG_F(errLevel, ...)
#define LOG_S(errLevel)
int logOpen(const char *alogPath="debug.log", const char *amode="w", LogLevel alogLevel=WARN) {}
void logClose() {}

#else // LOG_TYPE != LOG_TYPE_NO


#include <string>

#include "fmt/format.h"

#define LOG_F(errLevel, format, ...) log(errLevel, __FILE__, __LINE__, format, __VA_ARGS__)
#define LOG_S(errLevel) StreamLogger(errLevel, __FILE__, __LINE__)

// TODO(Derek): replace sstream with fmt

// std::string curDateTime();

int logOpen(const char *alogPath="debug.log", const char *amode="w", LogLevel alogLevel=WARN);
void logClose();


void logb(LogLevel errLevel,  const char *file, unsigned int line, std::string message);

template <typename... Args>
void log(LogLevel errLevel, const char *file, unsigned int line, const char* format, const Args & ... args) {
    std::string message = fmt::format(format, args...);

    logb(errLevel, file, line, message);
}

#ifdef LOG_STREAM
#include <sstream>
#include "fmt/ostream.h"

class StreamLogger {
public:
    StreamLogger(LogLevel errLevel, const char *file, unsigned line) : _errLevel(errLevel), _file(file), _line(line) {}
    StreamLogger::~StreamLogger() {
        std::string message = _ss.str();
        log(_errLevel, _file, _line, message.c_str());
    }

    template<typename T>
    StreamLogger& operator<<(const T& t) {
        _ss << t;
        return *this;
    }

    // std::endl and other iomanip:s.
    StreamLogger& operator<<(std::ostream&(*f)(std::ostream&)) {
        f(_ss);
        return *this;
    }

private:
    LogLevel    _errLevel;
    const char* _file;
    unsigned    _line;
    std::ostringstream _ss;
};

#endif // ifdef LOG_STREAM

#endif // LOG_TYPE != LOG_TYPE_NO
