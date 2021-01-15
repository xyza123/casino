#include "Texas.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#define board_left_x 288
#define board_right_x 1310
#define player_card_width 75
#define player_card_height 115
#define board_card_width 183
#define board_card_height 280
using namespace std;
vector<Texas_player> all_player;
card_set on_board;
card_set aboundant;
deck pool;
int on_board_money;
bool all_rounds_over = false;
map<pair<int, int>, ALLEGRO_BITMAP *> poker_img;       // poker_img[{point, color}] = bitmap
map<pair<int, int>, ALLEGRO_BITMAP *> small_poker_img; // poker_img[{point, color}] = bitmap
ALLEGRO_BITMAP *info = NULL;
ALLEGRO_BITMAP *selection_window = NULL;
ALLEGRO_BITMAP *bg_call = NULL;
ALLEGRO_BITMAP *bg_check = NULL;
ALLEGRO_BITMAP *bg_all_in = NULL;
ALLEGRO_BITMAP *msg_bubble = NULL;
void Texas_player::add_bet(int amount)
{
    if (!AI)
    {
        if (connected_player->budget >= amount)
        {
            bet_amount += amount;
            on_board_money += amount;
            connected_player->budget -= amount;
            return;
        }
    }
    AI_budget -= amount;
    bet_amount += amount;
    on_board_money += amount;
    draw_texas_bg(CALL);
    update_player_status(all_player);
    update_board(on_board);
    return;
}

void return_money(Texas_player *all_player)
{
}
void create_game(vector<player> &all, int antes, int AI_number, int player_number, int AI_budget)
{
    init_poker_imgs();
    on_board_money = antes * (player_number + AI_number);
    pool.refresh_deck();
    for (int i = 0; i < player_number; i++)
    {
        Texas_player now(all[i], antes);
        all_player.push_back(now);
    }
    for (int i = 0; i < AI_number; i++)
    {
        Texas_player now(AI_budget, antes);
        all_player.push_back(now);
    }
    // ----------setting player's position-------------
    all_player[0].pos_x = 800;
    all_player[0].pos_y = 790;
    all_player[1].pos_x = 213;
    all_player[1].pos_y = 600;
    all_player[all_player.size() - 1].pos_x = 1385;
    all_player[all_player.size() - 1].pos_y = 600;
    int player_size = all_player.size();
    int t_length = board_right_x - board_left_x;
    double player_dist = (t_length - player_card_width * 2 * (player_size - 3)) / (player_size - 3 + 1);
    int px = board_left_x + player_dist;
    cout << player_dist;
    for (int i = 2; i < all_player.size() - 1; i++)
    {
        all_player[i].pos_x = px;
        all_player[i].pos_y = 360;
        px += player_dist + player_card_width * 2;
    }
    // -------------------------------------------------
    // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    // shuffle(all_player.begin(), all_player.end(), default_random_engine(seed));
    int now_round = 0;
    bool flag;
    while (now_round <= 3)
    {
        if (now_round == 0)
        {
            for (int j = 0; j < 2; j++) // two cards
            {
                for (int i = 0; i < all_player.size(); i++)
                {
                    pool.licensing_card(all_player[i].own);
                    cout << all_player.size() << endl;
                }
            }
            update_player_status(all_player);
            int now_player = 0;
            int count = 0;
            while (count != all_player.size())
            {
                pair<int, int> antes_and_count = bet_round(antes, now_round, now_player, count);
                antes = antes_and_count.first;
                count = antes_and_count.second;
                now_player++;
                if (now_player == all_player.size())
                    now_player = 0;
            }
            draw_texas_bg(CHECK);
            update_player_status(all_player);
            update_board(on_board);
            now_round++;
        }
        else
        {
            if (now_round == 1)
            {
                pool.licensing_card(aboundant);
                for (int i = 0; i < 3; i++)
                {
                    pool.licensing_card(on_board);
                }
                update_board(on_board);
            }
            else
            {
                pool.licensing_card(aboundant);
                pool.licensing_card(on_board);
                update_board(on_board);
            }
            int now_player = 0;
            int count = 0;
            while (count != all_player.size())
            {
                pair<int, int> antes_and_count = bet_round(antes, now_round, now_player, count);
                antes = antes_and_count.first;
                count = antes_and_count.second;
                now_player++;
                if (now_player == all_player.size())
                    now_player = 0;
            }
            draw_texas_bg(CHECK);
            update_player_status(all_player);
            update_board(on_board);
            now_round++;
        }
        flag = check_termination();
        if (flag)
            break;
    }
    all_rounds_over = true;
    update_player_status(all_player);
    al_rest(5);
    endgame();
    return;
}
void endgame()
{
    // clear all player and the card set on board
}

bool check_termination()
{
    int remaining_player = 0;
    for (int i = 0; i < all_player.size(); i++)
    {
        if (all_player[i].status)
            remaining_player++;
        if (remaining_player >= 2)
            return false;
    }
    return true;
}

pair<int, int> bet_round(int antes, int current_round, int now_player, int count)
{
    int i = now_player;
    if (all_player[i].status)
    {
        int select;
        double AI_win_rate;
        if (!all_player[i].AI)
        {
            if (all_player[i].connected_player->budget > 0)
            {
                judge_status(all_player[i], antes);
                select = player_select(all_player[i]); //check...
            }
        }
        else
        {
            if (all_player[i].AI_budget > 0)
            {
                AI_win_rate = check_win_rate(all_player[i].own);
                select = AI_select(antes, AI_win_rate, all_player[i].AI_budget);
                draw_AI_selection(all_player[i], select);
                al_rest(1);
            }
        }
        if (select == FOLD)
        {
            all_player[i].status = false;
            update_player_status(all_player);
        }
        else if (select == RAISE)
        {
            count = 0;
            int raise_amount;
            if (!all_player[i].AI)
                raise_amount = select_raise_amount(all_player[i]); //
            else
                int raise_amount = AI_select_raise_amount(all_player[i].AI_budget, AI_win_rate, antes); //
            all_player[i].add_bet(raise_amount - all_player[i].bet_amount);
            antes = raise_amount;
            draw_texas_bg(CALL);
            update_player_status(all_player);
            update_board(on_board);
        }
        else if (select == CALL || select == CHECK)
        {
            all_player[i].add_bet(antes - all_player[i].bet_amount);
            update_player_status(all_player);
        }
        else if (select == ALL_IN)
        {
            int amount;
            if (!all_player[i].AI)
                all_player[i].add_bet(all_player[i].connected_player->budget);
            else
                all_player[i].add_bet(all_player[i].AI_budget);
            amount = all_player[i].bet_amount;
            if (amount > antes)
                antes = amount;
        }
        return {antes, count + 1};
    }
    return {antes, count + 1};
}
void judge_status(Texas_player &now, int now_antes)
{
    now.choose.clear();
    if (now.connected_player->budget <= now_antes - now.bet_amount)
    {
        now.choose.push_back(ALL_IN);
    }
    else if (now.bet_amount == now_antes)
    {
        now.choose.push_back(RAISE);
        now.choose.push_back(CHECK);
        now.choose.push_back(ALL_IN);
    }
    else
    {
        now.choose.push_back(CALL);
        now.choose.push_back(RAISE);
        now.choose.push_back(ALL_IN);
    }
    now.choose.push_back(FOLD);
}

int player_select(Texas_player &now_player) // remember to merge
{
    // ----------update selection--------------
    vector<int> choices = now_player.choose;
    int status = 0;
    if (find(choices.begin(), choices.end(), CALL) != choices.end())
        status = CALL;
    else if (find(choices.begin(), choices.end(), CHECK) != choices.end())
        status = CHECK;
    else if (choices.size() == 2)
        status = ALL_IN;
    draw_texas_bg(status);
    update_player_status(all_player);
    update_board(on_board);
    // ----------update selection--------------
    int mouse_x, mouse_y;
    do
    {
        // cout << "selection mode" << endl;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (mouse_x >= 455 && mouse_x <= 795 && mouse_y >= 1074 && mouse_y <= 1176 && status != ALL_IN)
                return RAISE;
            else if (mouse_x >= 795 && mouse_x <= 1135 && mouse_y >= 1074 && mouse_y <= 1176 && status != ALL_IN)
                return CALL;
            else if (mouse_x >= 1135 && mouse_x <= 1475 && mouse_y >= 1074 && mouse_y <= 1176)
                return FOLD;
            else if (mouse_x >= 115 && mouse_x <= 455 && mouse_y >= 1074 && mouse_y <= 1176)
                return ALL_IN;
        }
    } while (1);
}

int select_raise_amount(Texas_player p)
{
    // show the raise amount selection scene
    // the lowest raise must be greater than the double of the antes
    int raise_to = p.bet_amount;
    int mouse_x, mouse_y;
    draw_selection_window(raise_to, CALL);
    do
    {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (mouse_x >= 930 && mouse_x <= 1010 && mouse_y >= 530 && mouse_y <= 580)
                raise_to += 1000;
            else if (mouse_x >= 930 && mouse_x <= 1010 && mouse_y >= 610 && mouse_y <= 690)
                raise_to -= 1000;
            draw_selection_window(raise_to, CALL);
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            return raise_to;
    } while (1);
    return p.bet_amount;
}

int AI_select_raise_amount(int budget_amount, double win_rate, int antes)
{
    return 1;
}

int AI_select(int now_antes, double win_rate, int goal_antes)
{
    return 0;
}

void update_player_status(vector<Texas_player> players)
{
    for (auto p : players)
    {
        if (!p.is_main_user)
        {
            al_draw_bitmap(info, p.pos_x - player_card_width, p.pos_y, 0);
            al_draw_textf(Medium_font, al_map_rgb(0, 0, 0), p.pos_x, p.pos_y + 15, ALLEGRO_ALIGN_CENTRE, "%d", p.bet_amount);
            al_draw_textf(Medium_font, al_map_rgb(255, 255, 255), p.pos_x, p.pos_y + 45, ALLEGRO_ALIGN_CENTRE, "AI");
        }
        else
        {
            al_draw_textf(XL_font, al_map_rgb(255, 255, 255), 1050, 940, 0, "Bet Amount:");
            al_draw_textf(XL_font, al_map_rgb(255, 255, 255), 1050, 1000, 0, "%d", p.bet_amount);
        }

        if (p.status)
        {
            if (p.is_main_user)
            {
                al_draw_bitmap(poker_img[{p.own.card_set[0].point, p.own.card_set[0].color}], p.pos_x - board_card_width, p.pos_y, 0);
                al_draw_bitmap(poker_img[{p.own.card_set[1].point, p.own.card_set[1].color}], p.pos_x, p.pos_y, 0);
            }
            else
            {
                if (all_rounds_over)
                {
                    al_draw_bitmap(small_poker_img[{p.own.card_set[0].point, p.own.card_set[0].color}], p.pos_x - player_card_width, p.pos_y - player_card_height, 0);
                    al_draw_bitmap(small_poker_img[{p.own.card_set[1].point, p.own.card_set[1].color}], p.pos_x, p.pos_y - player_card_height, 0);
                }
                else
                {
                    al_draw_bitmap(poker_img[{0, 0}], p.pos_x - player_card_width, p.pos_y - player_card_height, 0);
                    al_draw_bitmap(poker_img[{0, 0}], p.pos_x, p.pos_y - player_card_height, 0);
                }
            }
        }
    }
    al_flip_display();
}

void update_board(card_set on_board_cards)
{
    int card_nums = on_board_cards.card_set.size();
    int px = 545, py = 454;
    for (auto cd : on_board_cards.card_set)
    {
        al_draw_bitmap(poker_img[{cd.point, cd.color}], px, py, 0);
        px += 332 / (card_nums - 1);
    }
    al_flip_display();
}

void init_poker_imgs()
{
    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            string s = "./card_set/" + to_string(i) + "_" + to_string(j) + ".png";
            string s1 = "./ai_card_set/" + to_string(i) + "_" + to_string(j) + ".png";
            ALLEGRO_BITMAP *card = al_load_bitmap(s.c_str());
            ALLEGRO_BITMAP *card1 = al_load_bitmap(s1.c_str());
            poker_img[{i, j}] = card;
            small_poker_img[{i, j}] = card1;
        }
    }
    ALLEGRO_BITMAP *card = al_load_bitmap("./card_set/red_back.png");
    poker_img[{0, 0}] = card;
    info = al_load_bitmap("./msg_pic/player_info.png");
    selection_window = al_load_bitmap("./raise_amount_window.png");
    bg_call = al_load_bitmap("./texas_bg_call.png");
    bg_check = al_load_bitmap("./texas_bg_check.png");
    bg_all_in = al_load_bitmap("./texas_bg_all_in.png");
    msg_bubble = al_load_bitmap("./msg_bubble.png");
}

void draw_selection_window(int target, int status)
{
    draw_texas_bg(status);
    update_player_status(all_player);
    update_board(on_board);
    al_draw_bitmap(selection_window, (window_width - 600) / 2, (window_height - 200) / 2, 0);
    al_draw_textf(XL_font, al_map_rgb(255, 255, 255), window_width / 2 - 200, window_height / 2 - 30, 0, "%d", target);
    al_flip_display();
}

void draw_texas_bg(int status)
{
    if (status == CALL)
        al_draw_bitmap(bg_call, 0, 0, 0);
    else if (status == CHECK)
        al_draw_bitmap(bg_check, 0, 0, 0);
    else if (status == ALL_IN)
        al_draw_bitmap(bg_all_in, 0, 0, 0);
}

double check_win_rate(card_set c)
{
    return 1;
}

void draw_AI_selection(Texas_player p, int selection)
{
    string str = "error";
    if (selection == CALL)
        str = "CALL";
    else if (selection == FOLD)
        str = "FOLD";
    else if (selection == ALL_IN)
        str = "ALL_IN";
    else if (selection == CHECK)
        str = "CHECK";
    else if (selection == RAISE)
        str = "RAISE";
    al_draw_bitmap(msg_bubble, p.pos_x - 170, p.pos_y - 150, 0);
    al_draw_textf(Large_font, al_map_rgb(0, 0, 0), p.pos_x - 70, p.pos_y - 90, ALLEGRO_ALIGN_CENTRE, "%s", str.c_str());
    al_flip_display();
}