#pragma warning (disable : 4996) // Visual C++ declares localtime 'unsafe'

#include "logging.h"

#if LOG_TYPE != LOG_TYPE_NO

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>


// TODO(Derek): Allow either trunc or append file modes
// TODO(Derek): Either switch to full printf or use << operator instead of log()


LogLevel setLogLevel = NO_LOG;
bool hasBeenOpened = false;

#if LOG_TYPE == LOG_TYPE_FILE
std::ofstream logOS;
#elif LOG_TYPE == LOG_TYPE_COUT
auto logOS = std::cout;
#endif


std::string curDateTime() {
    auto now = std::chrono::system_clock::now();

    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_t);

    auto milli = now.time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::milliseconds>(milli).count() % 1000;

    // TODO(Derek): use fmt
    std::ostringstream ss;
    ss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S.")
       << std::setfill('0') << std::setw(3) << secs;

    return ss.str();
}


int logOpen(const char *alogPath, LogLevel alogLevel) {
#if LOG_TYPE == LOG_TYPE_FILE
    setLogLevel = alogLevel;

    logOS.open(alogPath);

    hasBeenOpened = logOS.is_open();

    return !hasBeenOpened;
#else
    setLogLevel = alogLevel;
    return 0;
#endif
}


void logb(LogLevel errLevel, const char *file, unsigned int line, std::string message) {
    if (errLevel > setLogLevel) return;

#if LOG_TYPE == LOG_TYPE_FILE
    if (!hasBeenOpened) {
        std::cerr << "Log file has either been closed or not yet been opened with logOpen(..) at: " << file << ":" << line << std::endl;
        return;
    }

    if (!logOS.is_open()) {
        std::cerr << "There is a problem with the log file at: " << file << ":" << line << std::endl;
        return;
    }
#endif

    std::string errText;
    switch(errLevel) {
        case ERR:
            errText = "  ERROR";
            break;
        case WARN:
            errText = "WARNING";
            break;
        case INFO:
            errText = "   INFO";
            break;
        case DEBUG:
            errText = "  DEBUG";
            break;
    }

    // Only file name not full path
    for (const char *ptr = file; *ptr; ++ptr) {
        if (*ptr == '/' || *ptr == '\\') {
            file = ptr + 1;
        }
    }

    std::string prefix = fmt::format("{} [{:>20}:{:<5}] {}| ", curDateTime(), file, line, errText);

    std::string fullMessage = prefix + message;

    // TODO(Derek): Use fmt::print(file, )
    logOS << fullMessage << std::endl; // endl flushes
}


void logClose() {
#if LOG_TYPE == LOG_TYPE_FILE
    logOS.close();
    hasBeenOpened = false;
#endif
}

#endif // LOG_TYPE != LOG_TYPE_NO
