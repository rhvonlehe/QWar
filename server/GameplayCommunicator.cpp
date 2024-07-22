#include "GameplayCommunicator.h"


GameplayCommunicator::GameplayCommunicator(ba::io_context ctx)
    : gameplayRouter_(ctx)
{
    gameplayRouter_.bind("tcp://*:11998"); // TODO do not hard-code

    cout << "Waiting on server gameplay message" << endl;
    gameplayRouter_.async_receive();

}
