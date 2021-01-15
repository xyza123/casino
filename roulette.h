#ifndef ROULETTE_H_INCLUDED
#define ROULETTE_H_INCLUDED
#include <vector>
#include <utility>
#include "player.h"
#include "global.h"
using namespace std;
enum
{
    green, //0
    red,
    black
};
enum
{
    single, //1
    split,  //2
    street, //3
    corner, //4
    color,
    odd_and_even,
    twelve_number,
    mod,
    compare
};

class roulettle
{
public:
    roulettle();
    vector<pair<int, int>> number; //first:number, second:color
};

class roulettle_bet
{
public:
    roulettle_bet(int amount, int type, vector<pair<int, int>> bet_number, player &now_player);
    int amount;
    int type;
    player *connected_player;
    vector<pair<int, int>> bet_number;
};
void check_win();
void player_bet(player &now_player);
void create_roulette_game(vector<player>);
void init_roulette_resources();
void draw_roulette_table();
#endif