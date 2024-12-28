#pragma once

#include <memory>

// NOTE: using Pimpl here is most likely overkill. Since EventScheduler is not part
// of the gameplay library's API, it's *all* considered an implementation detail whether
// it's confined to a cpp file or a private header. I think I mostly did it as a mental
// exercise, especially since I didn't try to abstract away Boost::StateChart away
// in most other areas.

namespace gameplay
{

class EventScheduler
{
public:
    using Handle = uint32_t;

    EventScheduler() = default;

    template <class Processor, typename Arg1>
    Handle createProcessor(Arg1 arg1);

    template <typename T>
    void queueEvent(Handle, T event);

    void startTimer(Handle handle, uint32_t msecs);

    void run(void);

private:
    struct Pimpl;
    std::unique_ptr<Pimpl>  pimpl_;
};


} // gameplay
