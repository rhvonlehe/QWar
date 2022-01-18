#pragma once

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>

class TimerService
{
    struct TimerContext
    {
        boost::asio::deadline_timer     timer;
        std::function<void(void)>       action;
    };

public:
    TimerService();

    int startTimer(const boost::posix_time::milliseconds ms,
                   const std::function<void(void)> action);
    void cancelTimer(const int handle);

private:
    void dispatcher(void);

private:
    boost::asio::io_context     _io;
    std::vector<TimerContext>   _timers;
};
