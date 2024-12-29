#pragma once

#include <memory>

// NOTE: using Pimpl here is most likely overkill. Since EventScheduler is not part
// of the gameplay library's API, it's *all* considered an implementation detail whether
// it's confined to a cpp file or a private header. I think I mostly did it as a mental
// exercise, especially since I didn't try to abstract away Boost::StateChart away
// in most other areas.

namespace gameplay
{

// Normal use is to instantiate the scheduler, then create one or more processors,
// then call run. Users of the EventScheduler can then queue events or start timers
// using the handle to the Processor that was created in 'createProcessor'.

class EventScheduler
{
public:
    using ProcessorHandle = uint64_t;
    using TimerHandle = uint64_t;

    EventScheduler();
    ~EventScheduler();

    template <class Processor, typename Arg1>
    const ProcessorHandle createProcessor(Arg1 arg1);

    template <typename T>
    void queueEvent(ProcessorHandle);

    const TimerHandle startTimer(ProcessorHandle handle, uint32_t msecs);
    void stopTimer(TimerHandle handle);

    void run(void);

private:
    struct Pimpl;
    std::unique_ptr<Pimpl>  pimpl_;
};


} // gameplay
