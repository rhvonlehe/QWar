#pragma once

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>


// The design of this is such that it would be shared among many users.  This way
// only one thread needs to be started to handle the io_context::run and it can
// scale up to handle many timers.
// TODO: does this even make sense?  For a small system this is fine, but it won't truly
// scale up because every startTimer and timeout and cancel would require a lock on the vector.
//
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
