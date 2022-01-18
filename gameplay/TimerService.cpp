#include "TimerService.h"

TimerService::TimerService()
{

    // Create a thread to call asio 'run'
}

int TimerService::startTimer(const boost::posix_time::milliseconds ms,
                             const std::function<void(void)> action)
{


    return 0; // TODO
}

void TimerService::cancelTimer(const int handle)
{

}

void TimerService::dispatcher(void)
{

}

