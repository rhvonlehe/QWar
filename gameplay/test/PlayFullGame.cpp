#include <Game.h>

#include <vector>
#include <string>

using namespace gameplay;

int main(void)
{
    std::vector<std::string> playerNames;

    playerNames.push_back("player1");
    playerNames.push_back("player2");
    playerNames.push_back("player3");
    playerNames.push_back("player4");

    Game game(playerNames);

}
