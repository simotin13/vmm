#ifndef _LOG_H_
#define _LOG_H_

typedef enum _LOG_LEVEL {
    LOG_LEVEL_INFO,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_ERROR,
} LOG_LEVEL;

#define INFO_LOG(...)  write_log(__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_INFO, __VA_ARGS__);
#define TRACE_LOG(...)  write_log(__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_TRACE, __VA_ARGS__);
#define DEBUG_LOG(...)  write_log(__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_DEBUG, __VA_ARGS__);
#define ERROR_LOG(...)  write_log(__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__);

extern void write_log(const char *file, const char *func, int line, LOG_LEVEL level, const char *format, ...);

#endif    // _LOG_H_