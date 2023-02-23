#include "bwLog/bwLog.hpp"
#include "string_builder.hpp"
#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>


// TODO this is a mess. Gotta do some spring cleaning.

#define PRIMITIVE_OVERLOAD(type) Log& Log::operator<<(type value) {log_entry.message.write(value);return *this;}

namespace bwtk
{
    namespace log
    {
        constexpr auto MAX_BUFFER_SIZE = 4096;

        struct LogEntry
        {
            LogSeverity severity = LogSeverity::Debug;
            TimePoint tp;
            string_builder<MAX_BUFFER_SIZE> message;
        };

        LogSeverity log_filter = LogSeverity::Debug;

        bool write_to_file = false;
        std::ofstream log_file;

        thread_local LogEntry log_entry;

        struct LogCallbackEntry
        {
            LogSeverity min_level, max_level;
            LogCallback callback;
        };

        std::mutex callbacks_mutex;
        std::vector<LogCallbackEntry> callbacks;

        void default_callback(LogSeverity, const TimePoint&, const std::string&);

        bool has_callback(LogCallback cb)
        {
            // Checking if given callback is not already registered
            bool result = std::any_of(callbacks.begin(), callbacks.end(),
                        [&](const LogCallbackEntry& e) {return cb == e.callback;});

            return result;
        }

        void safe_add_callback(LogSeverity min, LogSeverity max, LogCallback cb)
        {
            std::lock_guard guard(callbacks_mutex);

            if(has_callback(cb))
            {
                return;
            }
            callbacks.push_back({min, max, cb});
        }

        void setLogFilter(LogSeverity filter)
        {
            log_filter = filter;
        }

        void setLogFile(const std::string& filename)
        {
            closeLogFile();
            log_file.open(filename.c_str());

            if(!log_file.is_open())
            {
                write_to_file = false;
                log_file.clear();
                // TODO report error
            }
            else
            {
                write_to_file = true;
            }
        }

        void closeLogFile()
        {
            log_file.clear();
            log_file.close();
            write_to_file = false;
        }

        void enableDefaultCallback()
        {
            addCallback(default_callback);
        }

        void addCallback(LogCallback cb)
        {
            safe_add_callback(LogSeverity::Debug, LogSeverity::Error, cb);
        }

        void addLevelCallback(LogSeverity level, LogCallback cb)
        {
            safe_add_callback(level, level, cb);
        }

        void addCallbackMin(LogSeverity min, LogCallback cb)
        {
            safe_add_callback(min, LogSeverity::Error, cb);
        }

        void addCallbackMax(LogSeverity max, LogCallback cb)
        {
            safe_add_callback(LogSeverity::Debug, max, cb);
        }

        void addCallbackRanged(LogSeverity min, LogSeverity max, LogCallback cb)
        {
            safe_add_callback(min, max, cb);
        }


        void publish()
        {
            std::lock_guard lock(callbacks_mutex);

            std::string msg = {log_entry.message.get()};
            log_entry.tp = TimePoint();

            for(auto& l : callbacks)
            {
                if(l.min_level <= log_entry.severity && l.max_level >= log_entry.severity)
                {
                    l.callback(log_entry.severity, log_entry.tp, msg);
                }
            }

            // Cleanup
            log_entry.message.clear();
        }

        Log::Log(LogSeverity severity)
        {
            log_entry.severity = severity;
        }

        Log::~Log()
        {
            publish();
        }

        PRIMITIVE_OVERLOAD(unsigned char)

        PRIMITIVE_OVERLOAD(unsigned short)

        PRIMITIVE_OVERLOAD(unsigned int)

        PRIMITIVE_OVERLOAD(unsigned long long int)

        PRIMITIVE_OVERLOAD(char)

        PRIMITIVE_OVERLOAD(short)

        PRIMITIVE_OVERLOAD(int)

        PRIMITIVE_OVERLOAD(long long int)

        PRIMITIVE_OVERLOAD(float)

        PRIMITIVE_OVERLOAD(double)

        PRIMITIVE_OVERLOAD(const std::string&)

        PRIMITIVE_OVERLOAD(void*)

        void default_callback(LogSeverity severity, const TimePoint& t, const std::string& msg)
        {
            std::stringstream message;

            switch(severity)
            {
                case LogSeverity::Debug:
                    message << "[DEBUG]";
                    break;
                case LogSeverity::Info:
                    message << "[INFO]";
                    break;
                case LogSeverity::Warning:
                    message << "[WARNING]";
                    break;
                case LogSeverity::Error:
                    message << "[ERROR]";
                    break;
            }

            message << t.str() << " : ";
            message << msg;

            if(log_filter <= severity)
            {
                auto output = message.str();
                std::cout << output << '\n';

                if(write_to_file)
                {
                    log_file << output << '\n';
                }
            }

            // Error is always active!
            if(severity == LogSeverity::Error)
            {
                std::abort();
            }
        }
    }
}