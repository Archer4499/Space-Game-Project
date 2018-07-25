#include "logging.h"

#if LOG_TYPE != LOG_TYPE_NO

#include <chrono>

#include "fmt/time.h"


LogLevel setLogLevel = NO_LOG;
FILE *logFILE = NULL;


std::string curDateTime() {
    auto now = std::chrono::system_clock::now();

    time_t now_t = std::chrono::system_clock::to_time_t(now);

    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    std::string fullDateTime = fmt::format("{:%Y-%m-%d %H:%M:%S}.{:03}", fmt::localtime(now_t), milli);

    return fullDateTime;
}


int logOpen(const char *alogPath, const char *amode, LogLevel alogLevel) {
    setLogLevel = alogLevel;

#if LOG_TYPE == LOG_TYPE_FILE
    logFILE = _fsopen(alogPath, amode, _SH_DENYWR);

    if (logFILE == NULL) {
        perror("Error opening file");
        return 1;
    }
#else
    logFILE = stdout;
#endif
    return 0;
}


void logb(LogLevel errLevel, const char *file, unsigned int line, std::string message) {
    if (errLevel > setLogLevel) return;

    if (logFILE == NULL) {
        fmt::print(stderr, "Log file/stream has either been closed or not yet been opened with logOpen(..) at: {}:{}\n", file, line);
        return;
    }

    std::string errText;
    switch(errLevel) {
        case FATAL:
            errText = "  FATAL";
            break;
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

    fmt::print(logFILE, "{}\n", fullMessage);
    fflush(logFILE); // Ensures log is up to date if we crash (Hopefully...)

    if (ferror(logFILE)) {
        fmt::print(stderr, "There is a problem with the log file at: {}:{}\n", file, line);
        return;
    }
}


void logClose() {
#if LOG_TYPE == LOG_TYPE_FILE
    fclose(logFILE);
#else
    logFILE = NULL;
#endif
}

#endif // LOG_TYPE != LOG_TYPE_NO
