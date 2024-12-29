#include "EventScheduler.h"
// #include "PlayerEvents.h"

#include <boost/statechart/asynchronous_state_machine.hpp>
#define ASIO_STANDALONE
#include <boost/asio.hpp>

#include <thread>
#include <map>

namespace sc = boost::statechart;
namespace ba = boost::asio;

using FifoScheduler = boost::statechart::fifo_scheduler<>;

namespace gameplay {

//-------------------------------- Pimpl ----------------------------------
//
struct EventScheduler::Pimpl
{
    Pimpl() : scheduler_(true) { }

    ~Pimpl() {
        scheduler_.terminate();
        schedulerThread_.join();
        work_.reset();  // <--- can this be removed since the unique_ptr goes out of scope anyway?
        ioCtxThread_.join();
    }

    template <class Processor, typename Arg1>
    const ProcessorHandle createProcessor(Arg1 arg1) {
        auto procHandle = nextProcHandle_++;
        auto processor = scheduler_.create_processor<Processor>(arg1);

        processors_[procHandle] = processor;
        scheduler_.initiate_processor(processor);

        return procHandle;
    }

    template<class T>
    void queueEvent(ProcessorHandle handle) {
        auto processor = processors_[handle];

        scheduler_.queue_event(processor,
                               boost::intrusive_ptr<T>(new T()));
    }

    const TimerHandle startTimer(ProcessorHandle procHandle, uint32_t msecs) {
        boost::posix_time::milliseconds boost_ms(msecs);
        auto processor = processors_[procHandle];
        TimerHandle timerHandle = nextTimerHandle_++;

        timers_.emplace(std::make_pair(timerHandle, ba::deadline_timer(io_, boost_ms)));

        timers_[timerHandle].async_wait([this, timerHandle, processor](const boost::system::error_code&) {
            timers_.erase(timerHandle);
            this->scheduler_.queue_event(processor, boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
        });

        return timerHandle;
    }

    void stopTimer(TimerHandle timerHandle)
    {
        if (timers_.find(timerHandle) != timers_.end())
        {
            timers_[timerHandle].cancel();
        }
    }

    void run(void) {
        schedulerThread_ = std::thread( [&]() {
            scheduler_();
        });
        ioCtxThread_ = std::thread( [&]() {
            work_ = std::make_unique<ba::executor_work_guard<ba::io_context::executor_type>>(ba::make_work_guard(io_));
            io_.run();
        });
    }

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


//---------------------------- Public API ----------------------------------
//

// Must specify compiler-defined constructor & destructor in cpp file to avoid
// compiler error about undefined type for pimpl.
//
EventScheduler::EventScheduler() = default;
EventScheduler::~EventScheduler() = default;

template <class Processor, typename Arg1>
const EventScheduler::ProcessorHandle EventScheduler::createProcessor(Arg1 arg1)
{
    return pimpl_->createProcessor<Processor>(arg1);
}

template <typename T>
void EventScheduler::queueEvent(ProcessorHandle processorHandle)
{
    pimpl_->queueEvent<T>(processorHandle);
}

const EventScheduler::TimerHandle EventScheduler::startTimer(ProcessorHandle processorHandle,
                                                             uint32_t msecs)
{
    return pimpl_->startTimer(processorHandle, msecs);
}

void EventScheduler::stopTimer(EventScheduler::TimerHandle timerHandle)
{
    pimpl_->stopTimer(timerHandle);
}

void EventScheduler::run(void)
{
    pimpl_->run();
}


} // gameplay
