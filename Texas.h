#ifndef TEXAS_H_INCLUDED
#define TEXAS_H_INCLUDED
#include "poker.h"
#include "player.h"
#include "global.h"

using namespace std;
class Texas_player
{
public:
    Texas_player(int budget, int antes)
    {
        bet_amount = antes;
        status = 1;
        AI = true;
        AI_budget = budget - bet_amount;
        return;
    }
    Texas_player(player &now_player, int antes)
    {
        connected_player = &now_player;
        bet_amount = antes;
        status = 1;
        AI = false;
        AI_budget = 0;
        is_main_user = true;
        return;
    }
    player *connected_player;
    bool status; // 1: still on board, 0: out
    card_set own;
    int bet_amount;
    vector<int> choose;
    void add_bet(int amount);
    bool AI;
    int AI_budget;
    int pos_x, pos_y;
    bool is_main_user = false;
};
// functions declaration
void return_money(Texas_player *all_player);
int select_raise_amount(Texas_player);
int AI_select_raise_amount(int, double, int);
pair<int, int> bet_round(int, int, int, int);
bool check_termination(void);
void endgame(void);
void judge_status(Texas_player &, int);
int player_select(Texas_player &);
int AI_select(int, double, int);
void create_game(vector<player> &, int, int, int, int);
void update_player_status(vector<Texas_player>);
void update_board(card_set);
void init_poker_imgs(void);
void draw_selection_window(int, int);
void draw_texas_bg(int);
double check_win_rate(card_set);
void draw_AI_selection(Texas_player, int);
#endif //TEXAS_H_INCLUDED