#include "EventScheduler.h"
#include "PlayerEvents.h"

#include <boost/statechart/asynchronous_state_machine.hpp>
#define ASIO_STANDALONE
#include <boost/asio.hpp>

#include <thread>
#include <vector>

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
    Handle createProcessor(Arg1 arg1) {
        Handle handle = std::distance(processors_.begin(), processors_.end()); // just the index

        auto iter = processors_.emplace_back(scheduler_.create_processor<Processor>(arg1));
        scheduler_.initiate_processor(*iter);

        return handle;
    }

    template<class T>
    void queueEvent(Handle handle, T event) {
        auto processor = processors_[handle];

        scheduler_.queue_event(processor,
                               boost::intrusive_ptr<T>(new T()));
    }

    void startTimer(Handle handle, uint32_t msecs) {
        auto processor = processors_[handle];
        auto timer = std::make_shared<ba::deadline_timer>(io_, msecs);

        timer->async_wait([this, timer, processor](const boost::system::error_code&) {
            this->scheduler_.queue_event(processor, boost::intrusive_ptr<EvTimeout>(new EvTimeout()));
        });
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

    FifoScheduler       scheduler_;
    std::thread         schedulerThread_;
    ba::io_context      io_;
    std::thread         ioCtxThread_;

    std::vector<FifoScheduler::processor_handle>                            processors_;
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
EventScheduler::Handle EventScheduler::createProcessor(Arg1 arg1)
{
    return pimpl_->createProcessor<Processor>(arg1);
}

template <typename T>
void EventScheduler::queueEvent(Handle handle, T event)
{
    pimpl_->queueEvent(handle, event);
}

void EventScheduler::startTimer(Handle handle, uint32_t msecs)
{
    pimpl_->startTimer(handle, msecs);
}

void EventScheduler::run(void)
{
    pimpl_->run();
}


} // gameplay
