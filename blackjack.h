#ifndef BLACKJACK_H_INCLUDED
#define BLACKJACK_H_INCLUDED
#include "poker.h"
#include "player.h"
#include "global.h"
#include <string>
using namespace std;

class blackjack_player
{
public:
    blackjack_player(player &now_player)
    {
        connected_player = &now_player;
        status = false;
        is_main_user = true;
        return;
    }
    player *connected_player;
    card_set own;
    int bet_amount;
    vector<int> choose;
    void add_bet(int amount);
    int pos_x, pos_y;
    bool is_main_user = false;
};
#endif //PLAYER_H_INCLUDED
