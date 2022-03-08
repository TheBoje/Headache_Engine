#ifndef LOGGER_H
#define LOGGER_H

#include <string.h>

namespace ift3100 {
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
    
    #define IFT_LOG_NOTICE(...)  ofLog(OF_LOG_NOTICE,      "<%s::%s::%d> %s", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define IFT_LOG_WARNING(...) ofLog(OF_LOG_WARNING,     "<%s::%s::%d> %s", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define IFT_LOG_ERROR(...)   ofLog(OF_LOG_ERROR,       "<%s::%s::%d> %s", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define IFT_LOG_FATAL(...)   ofLog(OF_LOG_FATAL_ERROR, "<%s::%s::%d> %s", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
}

#endif