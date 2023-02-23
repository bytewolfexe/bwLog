#ifndef BWTK_LOG_HPP
#define BWTK_LOG_HPP

#include <string>

namespace bwtk
{
    namespace log
    {
        enum class LogSeverity
        {
            Debug = 0,
            Info,
            Warning,
            Error
        };

        struct TimePoint;

        using LogCallback = void(*)(LogSeverity, const TimePoint&, const std::string&);

        /// @brief Enables default callback function, which outputs into standard output (console)
        /// and log file.
        void enableDefaultCallback();

        /// @brief Adds new callback for any type of log.
        void addCallback(LogCallback callback);

        /// @brief Adds new callback function for given log type.
        void addLevelCallback(LogSeverity level, LogCallback callback);

        /// @brief Adds new callback function which applies to all log levels above set minimum level.
        void addCallbackMin(LogSeverity min, LogCallback callback);

        /// @brief Add new callback function which applies to all log levels below set maximum level.
        void addCallbackMax(LogSeverity max, LogCallback callback);

        /// @brief Add new callback function which applies to given range of log levels.
        void addCallbackRanged(LogSeverity min, LogSeverity max, LogCallback callback);

        /// @brief Sets new log filter level.
        /// Each log that is lower level than the filter set will be ignored.
        /// @param severity New minimum severity level
        void setLogFilter(LogSeverity severity);

        /// @brief Sets new log file output for default log handler.
        /// If there is a file opened already, it will be closed and replaced with
        /// the new file (if file opening was successful)
        void setLogFile(const std::string& filename);

        /// @brief Closes log file for default log handler.
        void closeLogFile();

        class Log
        {
        public:
            Log(LogSeverity severity);
            ~Log();

            Log& operator << (unsigned char);
            Log& operator << (unsigned short);
            Log& operator << (unsigned int);
            Log& operator << (unsigned long long int);

            Log& operator << (char);
            Log& operator << (short);
            Log& operator << (int);
            Log& operator << (long long int);

            Log& operator << (float);
            Log& operator << (double);

            Log& operator << (const std::string&);
            Log& operator << (void*);
        };


        struct TimePoint
        {
            TimePoint();

            char hh;
            char mm;
            char ss;

            [[nodiscard]] std::string str() const;
        };
    }
}

#endif //BWTK_LOG_HPP
