#include <Game.h>
#include <TestPlayer.h>
#include <Player.h>

#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <condition_variable>
#include <iostream>

using namespace gameplay;
using namespace std;

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

TEST(GamePlay, PlayFullGame) {
    PlayFullGame();
}

