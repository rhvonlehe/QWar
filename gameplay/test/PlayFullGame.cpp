#include <Game.h>
#include <TestPlayer.h>
#include <Player.h>

#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <condition_variable>
#include <iostream>
#include <future>

using namespace gameplay;
using namespace std;



// TODO find a better place for all of this, or FetchContent it in
// https://github.com/doleron/basic-googletest-example/blob/master/test/timeout_check.cpp

/**
 * so far google testing fw doesn' t provide a TIMEOUT utility.
 * As workaround it is widely adopted the use of std::async and std::future solutions
 * Here we apply the solution found here: http://antonlipov.blogspot.com.br/2015/08/how-to-timeout-tests-in-gtest.html
 * */
#define TEST_TIMEOUT_BEGIN                           \
  std::promise<bool> promisedFinished;               \
  auto futureResult = promisedFinished.get_future(); \
                              std::thread([](std::promise<bool>& finished) {

#define TEST_TIMEOUT_FAIL_END(X)                                            \
  finished.set_value(true);                                                 \
  }, std::ref(promisedFinished)).detach(); \
  EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(X)) != std::future_status::timeout);

#define TEST_TIMEOUT_SUCCESS_END(X)                                         \
  finished.set_value(true);                                                 \
  }, std::ref(promisedFinished)).detach(); \
  EXPECT_FALSE(futureResult.wait_for(std::chrono::milliseconds(X)) != std::future_status::timeout);



void PlayFullGame(void)
{
    std::vector<std::string> playerNames;

    playerNames.push_back("player1");
    playerNames.push_back("player2");
    playerNames.push_back("player3");
    playerNames.push_back("player4");

    Game game(playerNames);

    condition_variable  cv;
    mutex               m;

    game.addObserverCallback([&](Game::ObservableEvent event) { cv.notify_one(); });

    game.deal();

    vector<TestPlayer> testPlayers;

    for (auto& name : playerNames)
    {
        auto* player = game.getPlayer(name);

        testPlayers.emplace_back(player);
    }

    // initial 'action' to prime the pump
    for (auto& player : testPlayers)
    {
        player.doAction();
    }

    unique_lock lk(m);
    cv.wait(lk);

    cout << "Done" << endl;
}

TEST(GamePlay, PlayFullGame)
{
    TEST_TIMEOUT_BEGIN
    PlayFullGame();
    TEST_TIMEOUT_FAIL_END(20000);
}

