#include "blackjack.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

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
//         blackjack_player now_player(now_player);
//         int bet_amount = player_add_bet();//玩家選擇金額
//         now_player.add_bet(bet_amount);
//         on_board_player.push_back(now_player);
//     }
//     // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
//     // shuffle(all_player.begin(), all_player.end(), default_random_engine(seed));
//     for(int i=0;i<on_board_player.size();i++){
//         for(int i=0;i<2;i++){
//             pool.licensing_card(on_board_player[i].own);//一明一暗
//         }
//     }
//     for(int i=0;i<on_board_player.size();i++){
//         int 
//         if(on_board_player[i].own[0].point == on_board_player[i].own[1].point)
//             split_card();
//         while(1){
            
//         }
//     }
// }