#pragma warning (disable : 4996)
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#define NO_LOG 0
#define ERR 1
#define WARN 2
#define INFO 3
#define DEBUG 4

#ifndef LOG_FILE
#define LOG_FILE "err.log"
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

// Log file
#if LOG_LEVEL
std::ofstream log_file(LOG_FILE);
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

void log(std::string err, int errLevel=2) {
#if LOG_LEVEL
    if (!log_file.is_open()) {
        std::cout << "Log file is not open!" << std::endl;
    }

    if (errLevel <= LOG_LEVEL) {
        log_file << curTime();

        switch(errLevel) {
            case ERR:
                log_file << " - ERROR - ";
                break;
            case WARN:
                log_file << " - WARNING - ";
                break;
            case INFO:
                log_file << " - INFO - ";
                break;
            case DEBUG:
                log_file << " - DEBUG - ";
                break;
        }

        log_file << err << std::endl;
    }
#else
    std::cout << err << std::endl;
#endif
}

void logClose() {
#if LOG_LEVEL
    log_file.close();
#endif
}
