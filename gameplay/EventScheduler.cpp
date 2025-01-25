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

const TimerHandle EventScheduler::startTimer(ProcessorHandle handle, uint32_t msecs)
{
    std::lock_guard<std::mutex> lock(mutex_);
    boost::posix_time::milliseconds boost_ms(msecs);
    auto processor = processors_[handle];
    TimerHandle timerHandle = nextTimerHandle_++;

    const auto [iter, success] = timers_.insert({timerHandle, ba::deadline_timer(io_, boost_ms)});

    assert(success);
    iter->second.async_wait([this, timerHandle, processor](const boost::system::error_code&) {
        timers_.erase(timerHandle);
        this->scheduler_.queue_event(processor, boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
    });

    return timerHandle;

}

void EventScheduler::stopTimer(TimerHandle handle)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (timers_.find(handle) != timers_.end())
    {
        auto& item = timers_.at(handle);
        item.cancel();
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
