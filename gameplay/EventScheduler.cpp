#include "EventScheduler.h"
#include "PlayerEvents.h"
#include <iostream>

using namespace std;

namespace gameplay {

std::atomic<uint64_t>    EventScheduler::nextProcHandle_ = 1;
std::atomic<uint64_t>    EventScheduler::nextTimerHandle_ = 1;

EventScheduler::EventScheduler(void) :
    scheduler_(true)
{}

EventScheduler::~EventScheduler(void)
{
    for(auto& pair : timers_)
    {
        pair.second.cancel();
    }

    scheduler_.terminate();
    schedulerThread_.join();
    work_.reset();
    ioCtxThread_.join();
}

const TimerHandle EventScheduler::startTimer(ProcessorHandle handle, uint32_t msecs)
{
    std::lock_guard<std::mutex> lock(timersMutex_);
    boost::posix_time::milliseconds boost_ms(msecs);
    auto processor = processors_[handle];
    TimerHandle timerHandle = nextTimerHandle_++;

    const auto [iter, success] = timers_.insert({timerHandle, ba::deadline_timer(io_, boost_ms)});

    assert(success);
    iter->second.async_wait([this, timerHandle, processor](const boost::system::error_code&) {
        std::lock_guard<std::mutex> lock(timersMutex_);
        timers_.erase(timerHandle);
        this->scheduler_.queue_event(processor, boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
    });

    std::cout << "current timer handles in startTimer: " << endl;
    for (auto& pair : timers_)
    {
        std::cout << pair.first << std::endl;
    }

    return timerHandle;

}

void EventScheduler::stopTimer(TimerHandle handle)
{
    std::lock_guard<std::mutex> lock(timersMutex_);

    std::cout << "current timer handles in stopTimer: " << endl;
    for (auto& pair : timers_)
    {
        std::cout << pair.first << std::endl;
    }

    if (timers_.find(handle) != timers_.end())
    {
        auto& item = timers_.at(handle);
        item.cancel();
        timers_.erase(handle);
    }
}

void EventScheduler::run(void)
{
    schedulerThread_ = std::thread( [&]() {
        scheduler_();
        cout << "Finished the endless scheduler() loop" << endl;
    });
    ioCtxThread_ = std::thread( [&]() {
        work_ = std::make_unique<ba::executor_work_guard<ba::io_context::executor_type>>(ba::make_work_guard(io_));
        io_.run();
    });
}

} // gameplay
