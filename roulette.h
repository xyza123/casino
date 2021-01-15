#ifndef ROULETTE_H_INCLUDED
#define ROULETTE_H_INCLUDED
#include <vector>
#include <utility>
#include "player.h"
#include "global.h"
using namespace std;
enum
{
    GREEN,
    BLACK,
    RED
};
enum
{
    SINGLE, // 1
    SPLIT,  // 2
    STREET, // 3
    CORNER, // 4
    COLOR,
    ODD_AND_EVEN,
    TWELVE_NUMBER,
    MOD,
    COMPARE
};
// color green:
//     0: 0
//     1: odd
//     2: even
//     3: 1st twelve
//     4: 2nd twelve
//     5: 3rd twelve

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
    vector<pair<int, int>> bet_number; // winning set (number, color)
};
void check_win();
void player_bet(player &now_player);
void create_roulette_game(vector<player>);
void init_roulette_resources();
void draw_roulette_table();
void init_chip_drawing_set();
string check_potential_chip_selection(int, int);
vector<pair<int, int>> check_hovered(int, int);
int bet_amount(int);
#endif