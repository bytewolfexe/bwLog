#include "bwLog/bwLog.hpp"
#include "string_builder.hpp"
#include <vector>
#include <mutex>

#define PRIMITIVE_OVERLOAD(type) Log& Log::operator<<(type value) {log_entry.message.write(value);return *this;}

namespace bwtk
{
    namespace log
    {
        constexpr auto MAX_BUFFER_SIZE = 4096;

        struct LogEntry
        {
            LogSeverity severity;
            TimePoint tp;
            string_builder<MAX_BUFFER_SIZE> message;
        };

        thread_local LogEntry log_entry;

        struct LogCallbackEntry
        {
            LogSeverity min_level, max_level;
            LogCallback callback;
        };

        std::mutex callbacks_mutex;
        std::vector<LogCallbackEntry> callbacks;

        bool has_callback(LogCallback cb)
        {
            // Checking if given callback is not already registered
            for(auto& l : callbacks)
            {
                if(l.callback == cb)
                {
                    return true;
                }
            }
            return false;
        }

        void addCallback(LogCallback cb)
        {
            if(has_callback(cb))
            {
                return;
            }

            callbacks.push_back({LogSeverity::Debug, LogSeverity::Error, cb});
        }

        void addLevelCallback(LogSeverity level, LogCallback cb)
        {
            if(has_callback(cb))
            {
                return;
            }

            callbacks.push_back({level, level, cb});
        }

        void addCallbackMin(LogSeverity min, LogCallback cb)
        {
            if(has_callback(cb))
            {
                return;
            }
            callbacks.push_back({min, LogSeverity::Error, cb});
        }

        void addCallbackMax(LogSeverity max, LogCallback cb)
        {
            if(has_callback(cb))
            {
                return;
            }
            callbacks.push_back({LogSeverity::Debug, max, cb});
        }

        void addCallbackRanged(LogSeverity min, LogSeverity max, LogCallback cb)
        {
            if(has_callback(cb))
            {
                return;
            }
            callbacks.push_back({min, max, cb});
        }


        void publish()
        {
            std::lock_guard lock(callbacks_mutex);
            std::string msg = {log_entry.message.get()};

            for(auto& l : callbacks)
            {
                if(l.min_level <= log_entry.severity && l.max_level >= log_entry.severity)
                {
                    l.callback(log_entry.severity, log_entry.tp, std::string());
                }
            }
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
    }
}