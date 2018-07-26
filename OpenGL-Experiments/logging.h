#pragma once

#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2

// Settings
#define LOG_TYPE LOG_TYPE_FILE
#define LOG_STREAM_OVERLOAD
////

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
// TODO(Derek): Define LOG_S using a null output stream
int logOpen(const char *alogPath="debug.log", const char *amode="w", LogLevel alogLevel=WARN) {return 0;}
void logClose() {}

#else // LOG_TYPE != LOG_TYPE_NO


#include <string>

#include "fmt/core.h"

// TODO(Derek): possibly change to LOG
// TODO(Derek): add conditional log
#define LOG_F(errLevel, format, ...) log(errLevel, __FILE__, __LINE__, format, __VA_ARGS__)


// std::string curDateTime();

int logOpen(const char *alogPath="debug.log", const char *amode="w", LogLevel alogLevel=WARN);
void logClose();


void logb(LogLevel errLevel,  const char *file, unsigned int line, std::string message);

template <typename... Args>
void log(LogLevel errLevel, const char *file, unsigned int line, const char* format, const Args & ... args) {
    std::string message = fmt::format(format, args...);

    logb(errLevel, file, line, message);
}


#ifdef LOG_STREAM_OVERLOAD
// Allows user defined types with operator<< overloads to be used in LOG_F
#include "fmt/ostream.h"
#endif

#ifdef LOG_STREAM

#include "fmt/format.h"

#define LOG_S(errLevel) StreamLogger(errLevel, __FILE__, __LINE__)

class StreamLogger {
public:
    StreamLogger(LogLevel errLevel, const char *file, unsigned int line) : _errLevel(errLevel), _file(file), _line(line) {}
    StreamLogger::~StreamLogger() {
        logb(_errLevel, _file, _line, fmt::to_string(_message));
    }

    template<typename T>
    StreamLogger& operator<<(const T& t) {
        format_to(_message, "{}", t);
        return *this;
    }

private:
    LogLevel           _errLevel;
    const char*        _file;
    unsigned int       _line;
    fmt::memory_buffer _message;
};

#endif // ifdef LOG_STREAM

#endif // LOG_TYPE != LOG_TYPE_NO
