#pragma warning (disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "logging.h"


int _logLevel = 0;
bool _hasBeenOpened = false;

#if LOG_TYPE == LOG_TYPE_FILE
std::ofstream _logOutput;
#elif LOG_TYPE == LOG_TYPE_COUT
auto _logOutput = std::cout;
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


int logOpen(std::string logPath, int logLevel ) {
#if LOG_TYPE == LOG_TYPE_FILE
    _logLevel = logLevel;
    _logOutput.open(logPath);

    _hasBeenOpened = _logOutput.is_open();

    return !_hasBeenOpened;
#else
    _logLevel = logLevel;
    return 0;
#endif
}

void log(std::string err, int errLevel) {
#if LOG_TYPE != LOG_TYPE_NO
    #if LOG_TYPE == LOG_TYPE_FILE
        if (!_hasBeenOpened) {
            std::cerr << "Log file has not yet been opened with logOpen(..)" << std::endl;
            return;
        }

        if (!_logOutput.is_open()) {
            std::cerr << "Log file has either already been closed or there is a problem with the file. Error that attempted to log:" << std::endl;
            std::cerr << err << std::endl;
            return;
        }
    #endif

    if (errLevel <= _logLevel) {
        _logOutput << curTime();

        switch(errLevel) {
            case ERR:
                _logOutput << " - ERROR - ";
                break;
            case WARN:
                _logOutput << " - WARNING - ";
                break;
            case INFO:
                _logOutput << " - INFO - ";
                break;
            case DEBUG:
                _logOutput << " - DEBUG - ";
                break;
        }

        _logOutput << err << std::endl;
    }
#endif
}

void logClose() {
#if LOG_TYPE == LOG_TYPE_FILE
    _logOutput.close();
#endif
}
