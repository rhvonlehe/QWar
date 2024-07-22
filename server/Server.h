#include "StatePublisher.h"
#include "StateReceiver.h"
// #include "GameplayCommunicator.h"
// #include "ControlCommunicator.h"

namespace ba = boost::asio;

class Server
{
public:
    Server(void);
    void operator()(void);




private:
    ba::io_context          ctx_;
    StatePublisher          publisher_;
    StateReceiver           stateRcvr_;
    // std::unique_ptr<UpdateReqRcvr>          updateReqRcvr_;



    // std::unique_ptr<GameplayCommunicator>   gameplayComm_;  // TODO remove
    // std::unique_ptr<ControlCommunicator>    controllComm_;  // TODO remove




    void GamePlayHandler(azmq::message& msg);


};

