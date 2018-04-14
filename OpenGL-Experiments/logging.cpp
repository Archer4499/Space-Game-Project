#pragma warning (disable : 4996) // Visuall C++ declares localtime 'unsafe'

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "logging.h"


// TODO(Derek): Allow either trunc or append file modes
// TODO(Derek): Either switch to full printf or use << operator instead of log()


LogLevel setLogLevel = NO_LOG;
bool hasBeenOpened = false;

#if LOG_TYPE == LOG_TYPE_FILE
std::ofstream logOS;
#elif LOG_TYPE == LOG_TYPE_COUT
auto logOS = std::cout;
#endif


std::string curTime() {
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_t);

    auto now_milli = std::chrono::system_clock::now().time_since_epoch();
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now_milli).count() % 1000;


    std::ostringstream ss;
    ss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S.")
       << std::setfill('0') << std::setw(3) << milli;

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

void log(std::string err, LogLevel errLevel) {
#if LOG_TYPE != LOG_TYPE_NO
    #if LOG_TYPE == LOG_TYPE_FILE
        if (!hasBeenOpened) {
            std::cerr << "Log file has not yet been opened with logOpen(..)" << std::endl;
            return;
        }

        if (!logOS.is_open()) {
            std::cerr << "Log file has either already been closed or there is a problem with the file. Error that attempted to log:" << std::endl;
            std::cerr << err << std::endl;
            return;
        }
    #endif

    if (errLevel <= setLogLevel) {
        logOS << curTime();

        switch(errLevel) {
            case ERR:
                logOS << " - ERROR - ";
                break;
            case WARN:
                logOS << " - WARNING - ";
                break;
            case INFO:
                logOS << " - INFO - ";
                break;
            case DEBUG:
                logOS << " - DEBUG - ";
                break;
        }

        logOS << err << std::endl;
    }
#endif
}

void logClose() {
#if LOG_TYPE == LOG_TYPE_FILE
    logOS.close();
#endif
}
