#ifndef LIGHTNVR_LOGGER_H
#define LIGHTNVR_LOGGER_H

#include <stdarg.h>
#include <stddef.h>

// Log levels
// Change your logger.h enum to avoid conflicting with syslog.h
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_DEBUG = 3
} log_level_t;

/**
 * Initialize the logging system
 * 
 * @return 0 on success, non-zero on failure
 */
int init_logger(void);

/**
 * Shutdown the logging system
 */
void shutdown_logger(void);

/**
 * Set the log level
 * 
 * @param level The log level to set
 */
void set_log_level(log_level_t level);

/**
 * Set the log file
 * 
 * @param filename Path to the log file
 * @return 0 on success, non-zero on failure
 */
int set_log_file(const char *filename);

/**
 * Enable or disable console logging
 * 
 * Note: With tee behavior enabled, console logging is always active
 * This function is kept for API compatibility but has no effect on output
 * 
 * @param enable True to enable console logging, false to disable (no effect with tee behavior)
 */
void set_console_logging(int enable);

/**
 * Log a message at ERROR level
 * 
 * @param format Printf-style format string
 * @param ... Format arguments
 */
void log_error(const char *format, ...);

/**
 * Log a message at WARN level
 * 
 * @param format Printf-style format string
 * @param ... Format arguments
 */
void log_warn(const char *format, ...);

/**
 * Log a message at INFO level
 * 
 * @param format Printf-style format string
 * @param ... Format arguments
 */
void log_info(const char *format, ...);

/**
 * Log a message at DEBUG level
 * 
 * @param format Printf-style format string
 * @param ... Format arguments
 */
void log_debug(const char *format, ...);

/**
 * Log a message at the specified level
 * 
 * @param level Log level
 * @param format Printf-style format string
 * @param ... Format arguments
 */
void log_message(log_level_t level, const char *format, ...);

/**
 * Log a message at the specified level with va_list
 * 
 * @param level Log level
 * @param format Printf-style format string
 * @param args Format arguments as va_list
 */
void log_message_v(log_level_t level, const char *format, va_list args);

/**
 * Rotate log files if they exceed a certain size
 * 
 * @param max_size Maximum size in bytes before rotation
 * @param max_files Maximum number of rotated files to keep
 * @return 0 on success, non-zero on failure
 */
int log_rotate(size_t max_size, int max_files);

/**
 * Get the string representation of a log level
 *
 * @param level The log level
 * @return String representation of the log level, or "UNKNOWN" if invalid
 */
const char *get_log_level_string(log_level_t level);

/**
 * Enable syslog logging
 *
 * @param ident Syslog identifier (application name)
 * @param facility Syslog facility (e.g., LOG_USER, LOG_DAEMON, LOG_LOCAL0-7)
 * @return 0 on success, non-zero on failure
 */
int enable_syslog(const char *ident, int facility);

/**
 * Disable syslog logging
 */
void disable_syslog(void);

/**
 * Check if syslog is enabled
 *
 * @return 1 if syslog is enabled, 0 otherwise
 */
int is_syslog_enabled(void);

/**
 * Check if logger is available for use
 *
 * @return 1 if logger is available, 0 if shutting down or not initialized
 */
int is_logger_available(void);

/* -----------------------------------------------------------------------
 * Per-thread logging context
 *
 * Each long-running thread may call log_set_thread_context() once at
 * startup so that every subsequent log_* call from that thread
 * automatically includes a [component] and, when applicable, a
 * [stream_name] field in the log line:
 *
 *   [timestamp] [LEVEL] [component] [stream] message
 *   [timestamp] [LEVEL] [component] message        <- no stream set
 *   [timestamp] [LEVEL] message                    <- no context set
 *
 * The implementation uses __thread storage; no mutex is required.
 * ----------------------------------------------------------------------- */

/**
 * Set the logging context for the current thread.
 *
 * @param component   Short label for the subsystem, e.g. "MP4Writer"
 *                    (max 63 chars, copied into thread-local storage).
 *                    Pass NULL or "" to clear.
 * @param stream_name Name of the stream this thread is handling, e.g.
 *                    "front_door" (max 127 chars, copied).
 *                    Pass NULL or "" when the thread is not stream-specific.
 */
void log_set_thread_context(const char *component, const char *stream_name);

/**
 * Clear the logging context for the current thread.
 * After this call log_* calls from the thread omit the context prefix.
 */
void log_clear_thread_context(void);

/**
 * Return the component label stored for the current thread.
 * Returns "" when no context has been set.
 */
const char *log_get_thread_component(void);

/**
 * Return the stream name stored for the current thread.
 * Returns "" when no stream context has been set.
 */
const char *log_get_thread_stream(void);

#endif // LIGHTNVR_LOGGER_H
