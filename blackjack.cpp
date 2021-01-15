#include "blackjack.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

enum{
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    INSURANCE
};
deck pool;

vector <player> all;
vector <blackjack_player> on_board_player;
card_set bookmaker;
void blackjacl_player::add_bet(int amount)
{
    if (connected_player->budget >= amount)
    {
        bet_amount += amount;
        on_board_money += amount;
        connected_player->budget -= amount;
        return;
    }
    return;
}


// void create_game(vector<player> all){
//     for(int i=0;i<all.size();i++){
//         blackjack_player now_player(all[i]);
//         on_board_player.push_back(now_player);
//     }
//     // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
//     // shuffle(all_player.begin(), all_player.end(), default_random_engine(seed));
//     for(int i=0;i<on_board_player.size();i++){
//         pool.licensing_card(on_board_player[i].own);//發暗牌
//         int bet_amount = player_add_bet();//玩家選擇金額
//         if(bet_amount == 0){
            
//         }
//         now_player.add_bet(bet_amount);
//         pool.licensing_card(on_board_player[i].own);//發明牌
//     }
//     for(int i=0;i<on_board_player.size();i++){
//         while(1){
//             //
//         }
//     }
// }