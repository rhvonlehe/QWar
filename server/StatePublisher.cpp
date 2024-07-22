#include "StatePublisher.h"

StatePublisher::StatePublisher(boost::asio::io_context &ctx)
    : pubSocket_(ctx)
{
    pubSocket_.bind("tcp://*:11998");  // TODO do not hard-code

}
