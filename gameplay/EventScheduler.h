#pragma once

#include "EventSchedulerDefs.h"
#include "PlayerEvents.h"

#include <boost/statechart/asynchronous_state_machine.hpp>
#define ASIO_STANDALONE
#include <boost/asio.hpp>

#include <thread>
#include <map>
#include <memory>

namespace sc = boost::statechart;
namespace ba = boost::asio;

using FifoScheduler = boost::statechart::fifo_scheduler<>;

namespace gameplay
{

// Normal use is to instantiate the scheduler, then create one or more processors,
// then call run. Users of the EventScheduler can then queue events or start timers
// using the handle to the Processor that was created in 'createProcessor'.

class EventScheduler
{
public:
    EventScheduler() = default;
    ~EventScheduler() = default;

    template <class Processor, typename Arg1>
    const ProcessorHandle createProcessor(Arg1 arg1)
    {
        auto procHandle = nextProcHandle_++;
        auto processor = scheduler_.create_processor<Processor>(arg1);

        processors_[procHandle] = processor;
        scheduler_.initiate_processor(processor);

        return procHandle;
    }

    template <typename T>
    void queueEvent(ProcessorHandle handle, T* event)
    {
        auto processor = processors_[handle];

        scheduler_.queue_event(processor, boost::intrusive_ptr<T>(event));
    }

    const TimerHandle startTimer(ProcessorHandle handle, uint32_t msecs)
    {
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
    void stopTimer(TimerHandle handle)
    {
        if (timers_.find(handle) != timers_.end())
        {
            auto& item = timers_.at(handle);
            item.cancel();
        }
    }

    void run(void)
    {
        schedulerThread_ = std::thread( [&]() {
            scheduler_();
        });
        ioCtxThread_ = std::thread( [&]() {
            work_ = std::make_unique<ba::executor_work_guard<ba::io_context::executor_type>>(ba::make_work_guard(io_));
            io_.run();
        });
    }

private:
    std::atomic<uint64_t>   nextProcHandle_ = 0;
    std::atomic<uint64_t>   nextTimerHandle_ = 0;
    FifoScheduler           scheduler_;
    std::thread             schedulerThread_;
    ba::io_context          io_;
    std::thread             ioCtxThread_;

    std::map<uint64_t, FifoScheduler::processor_handle>                     processors_;
    std::map<uint64_t, ba::deadline_timer>                                  timers_;
    std::unique_ptr<ba::executor_work_guard<ba::io_context::executor_type>> work_;
};


} // gameplay
