#ifndef _VL_LOGGER_H_
#define _VL_LOGGER_H_

#pragma once

#include <cstdio>
#include <ctime>        // va_list, va_start, va_end
#include <cstdarg>
#include <string>


/**
 * @file    logger.hpp
 * @brief   
 * 
*/
namespace vl {
namespace utils {
namespace logger {

#ifndef COLOR
    #define COLOFF  "\033[0m"      
    #define RED      "\033[0;31m"
    #define GREEN    "\033[0;32m"
    #define BLUE     "\033[0;34m"
#endif


enum LogLevel {
    LOG_LEVEL_SILENT = 0,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
};

inline LogLevel& getGlobalLogLevel()
{
    static LogLevel glovalLogLevel = LOG_LEVEL_INFO;
    return glovalLogLevel;
}

inline void setLogLevel(LogLevel level)
{
    getGlobalLogLevel() = level;
}

inline LogLevel getLogLevel()
{
    return getGlobalLogLevel();
}

inline std::string getCurrentTimeString()
{
    std::time_t t = std::time(nullptr);
    struct tm *lt = std::localtime(&t);

    char buff[32];
    std::strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", lt);
    return std::string(buff);
}


inline void logMessage(LogLevel level,
                        const char* file,
                        int line,
                        const char* tag,
                        const char* fmt, ...)
{
    if(level < getLogLevel()) {
        return;
    }

    const char* color = COLOFF;
    const char* levelName = "UNKNOWN";

    switch (level)
    {
        case LOG_LEVEL_FATAL:       color = RED;        levelName = "FATAL";
        case LOG_LEVEL_ERROR:       color = RED;        levelName = "ERROR";
        case LOG_LEVEL_WARNING:     color = BLUE;       levelName = "WARN";
        case LOG_LEVEL_INFO:        color = GREEN;      levelName = "INFO";
        case LOG_LEVEL_DEBUG:       color = COLOFF;     levelName = "DEBUG";
        case LOG_LEVEL_VERBOSE:     color = COLOFF;     levelName = "VERB";
        default: break;
    }
    
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    std::fprintf(   stderr,    "%s[%s] %s [%s:%d] [%s] %s%s\n",
                    color,
                    getCurrentTimeString().c_str(),
                    levelName,
                    file,
                    line,
                    (tag ? tag : " "),
                    buffer,
                    COLOFF
    );

    if(level == LOG_LEVEL_FATAL) {

    }
    
}


#define VL_LOG_FATAL(tag, fmt, ...)     vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_FATAL,   __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );
#define VL_LOG_ERROR(tag, fmt, ...)     vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_ERROR,   __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );
#define VL_LOG_WARNING(tag, fmt, ...)   vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_WARNING, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );
#define VL_LOG_INFO(tag, fmt, ...)      vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_INFO,    __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );
#define VL_LOG_DEBUG(tag, fmt, ...)     vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_DEBUG,   __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );
#define VL_LOG_VERBOSE(tag, fmt, ...)   vl::utils::logger::logMessage(vl::utils::logger::LOG_LEVEL_VERBOSE, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__  );


} // namespace logger
} // namespace utils
} // namespace vl

#endif

