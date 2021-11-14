#pragma once

class Player;

class PlayerState
{
public:
    virtual void playFirst(Player&) {};
    virtual void winnerTie(Player&) {};
    virtual void layWarCards(Player&) {};
    virtual void roundEnded(Player&) {};
    virtual void outOfCards(Player&);
protected:
    void changeState(Player&, PlayerState*);
};

class Eliminated : public PlayerState
{
public:
    static PlayerState* instance();
};

class Idle : public PlayerState
{
public:
    static PlayerState* instance();

    virtual void playFirst(Player&);
};

class PlayedFirst : public PlayerState
{
public:
    static PlayerState* instance();

    virtual void winnerTie(Player&);
    virtual void roundEnded(Player&);
};

class LaidWarCards : public PlayerState
{
public:
    static PlayerState* instance();

    virtual void roundEnded(Player&);
};







#if 0 // todo remove
class PlayerSM
{
public:
    enum PlayerState {
        PLAYER_STATE_ELIMINATED,
        PLAYER_STATE_IDLE,
        PLAYER_STATE_PLAYED_FIRST,
        PLAYER_STATE_LAID_WAR_CARDS
    };

    enum PlayerEvent {
        PLAYER_EVENT_PLAY_FIRST,
        PLAYER_EVENT_WINNER_TIE,
        PLAYER_EVENT_LAY_WAR_CARDS,
        PLAYER_EVENT_ROUND_ENDED,
        PLAYER_EVENT_OUT_OF_CARDS
    };


    PlayerSM();

    void process(Player& context, PlayerEvent event);
};
#endif

