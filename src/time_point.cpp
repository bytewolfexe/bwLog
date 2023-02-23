#include "bwLog/bwLog.hpp"
#include <ctime>
#include <sstream>


namespace bwtk::log
{
    TimePoint::TimePoint()
    {
        auto tt = time(nullptr);

        struct tm* now = localtime(&tt);

        hh = (char)now->tm_hour;
        mm = (char)now->tm_min;
        ss = (char)now->tm_sec;
    }

    std::string TimePoint::str() const
    {
        std::stringstream st;
        st << (short)hh << ':' << (short)mm << ':' << (short)ss;
        return st.str();
    }
}