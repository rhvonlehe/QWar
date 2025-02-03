#pragma once

#include "EventSchedulerDefs.h"

#include <boost/statechart/asynchronous_state_machine.hpp>
#define ASIO_STANDALONE
#include <boost/asio.hpp>

#include <thread>
#include <map>
#include <memory>
#include <mutex>

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
    EventScheduler();
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
    void queueEvent(ProcessorHandle handle, T& event)
    {
        auto processor = processors_[handle];

        scheduler_.queue_event(processor, boost::intrusive_ptr<T>(new T(event)));
    }

    const TimerHandle startTimer(ProcessorHandle handle, uint32_t msecs);

    void stopTimer(TimerHandle handle);

    void run(void);

private:
    static std::atomic<uint64_t>    nextProcHandle_;
    static std::atomic<uint64_t>    nextTimerHandle_;
    std::mutex                      timersMutex_;
    FifoScheduler                   scheduler_;
    std::thread                     schedulerThread_;
    ba::io_context                  io_;
    std::thread                     ioCtxThread_;

    std::map<uint64_t, FifoScheduler::processor_handle>                     processors_;
    std::map<uint64_t, ba::deadline_timer>                                  timers_;
    std::unique_ptr<ba::executor_work_guard<ba::io_context::executor_type>> work_;
};



} // gameplay
