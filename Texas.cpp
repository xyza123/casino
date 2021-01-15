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
map<pair<int, int>, ALLEGRO_BITMAP *> poker_img; // poker_img[{point, color}] = bitmap
ALLEGRO_BITMAP *info = NULL;
ALLEGRO_BITMAP *selection_window = NULL;
ALLEGRO_BITMAP *bg_call = NULL;
ALLEGRO_BITMAP *bg_check = NULL;
ALLEGRO_BITMAP *bg_all_in = NULL;
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

void return_money(int winner, int amount)
{
    all_player[winner].connected_player->budget += amount;
    return;
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
            now_round++;
        }
        flag = check_termination();
        if (flag)
            break;
    }
    endgame(now_round);
    return;
}

void endgame(int round)
{
    int total_return_money = 0;
    for(int i=0;i<all_player.size();i++)
        total_return_money += all_player[i].bet_amount;
    if(round != 4){
        int winner;
        for(int i=0;i<all_player.size();i++){
            if(all_player[i].status)
                winner = i;
        }
        return_money(winner, total_return_money);
        return;
    }
    sort(all_player.begin(), all_player.end(), card_set_compare);
    int now_player = 0;
    while(1){
        if(all_player[now_player].status){
            int will_get = all_player[now_player].bet_amount * all_player.size();
            if(will_get >= total_return_money){
                return_money(now_player, total_return_money);
                break;
            }
            else{
                total_return_money -= will_get;
                return_money(now_player, will_get);
                now_player++;
            }
        }
    }
    return;
}

int card_set_compare(Texas_player &a, Texas_player &b){
    card_set A = a.own;
    card_set B = b.own;
    for(int i=0;i<on_board.card_set.size();i++){
        A.card_set.push_back(on_board.card_set[i]);
        B.card_set.push_back(on_board.card_set[i]);
    }
    pair<int, int> compare_A = {0,0};
    pair<int, int> compare_B = {0,0};
    for(int i=0;i<6;i++){
        for(int j=i+1;j<7;j++){
            card_set copy_set_A = A;
            card_set copy_set_B = B;
            card_set real_set_A;
            card_set real_set_B;
            copy_set_A.card_set[i].point = -1;
            copy_set_A.card_set[j].point = -1;
            copy_set_B.card_set[i].point = -1;
            copy_set_B.card_set[j].point = -1;
            pair<int, int> now_compare_A = {0,0};
            pair<int, int> now_compare_B = {0,0};
            for(int k=0;k<7;k++){
                if(copy_set_A.card_set[k].point != -1)
                    real_set_A.card_set.push_back(copy_set_A.card_set[k]);
                if(copy_set_B.card_set[k].point != -1)
                    real_set_B.card_set.push_back(copy_set_B.card_set[k]);
            }
            while(1){
                if(straight_flush(real_set_A).first){
                    now_compare_A.first = 8;
                    now_compare_A.second = straight_flush(real_set_A).second;
                    break;
                }
                if(four_of_a_kind(real_set_A).first){
                    now_compare_A.first = 7;
                    now_compare_A.second = four_of_a_kind(real_set_A).second;
                    break;
                }
                if(full_house(real_set_A).first){
                    now_compare_A.first = 6;
                    now_compare_A.second = full_house(real_set_A).second;
                    break;
                }
                if(flush(real_set_A).first){
                    now_compare_A.first = 5;
                    now_compare_A.second = flush(real_set_A).second;
                    break;
                }
                if(straight(real_set_A).first){
                    now_compare_A.first = 4;
                    now_compare_A.second = straight(real_set_A).second;
                    break;
                }
                if(three_of_a_kind(real_set_A).first){
                    now_compare_A.first = 3;
                    now_compare_A.second = three_of_a_kind(real_set_A).second;
                    break;
                }
                if(two_pair(real_set_A).first){
                    now_compare_A.first = 2;
                    now_compare_A.second = two_pair(real_set_A).second;
                    break;
                }
                if(one_pair(real_set_A).first){
                    now_compare_A.first = 1;
                    now_compare_A.second = one_pair(real_set_A).second;
                    break;
                }
                if(high_card(real_set_A).first){
                    now_compare_A.first = 0;
                    now_compare_A.second = high_card(real_set_A).second;
                    break;
                }
            }
            while(1){
                if(straight_flush(real_set_B).first){
                    now_compare_B.first = 8;
                    now_compare_B.second = straight_flush(real_set_B).second;
                    break;
                }
                if(four_of_a_kind(real_set_B).first){
                    now_compare_B.first = 7;
                    now_compare_B.second = four_of_a_kind(real_set_B).second;
                    break;
                }
                if(full_house(real_set_B).first){
                    now_compare_B.first = 6;
                    now_compare_B.second = full_house(real_set_B).second;
                    break;
                }
                if(flush(real_set_B).first){
                    now_compare_B.first = 5;
                    now_compare_B.second = flush(real_set_B).second;
                    break;
                }
                if(straight(real_set_B).first){
                    now_compare_B.first = 4;
                    now_compare_B.second = straight(real_set_B).second;
                    break;
                }
                if(three_of_a_kind(real_set_B).first){
                    now_compare_B.first = 3;
                    now_compare_B.second = three_of_a_kind(real_set_B).second;
                    break;
                }
                if(two_pair(real_set_B).first){
                    now_compare_B.first = 2;
                    now_compare_B.second = two_pair(real_set_B).second;
                    break;
                }
                if(one_pair(real_set_B).first){
                    now_compare_B.first = 1;
                    now_compare_B.second = one_pair(real_set_B).second;
                    break;
                }
                if(high_card(real_set_B).first){
                    now_compare_B.first = 0;
                    now_compare_B.second = high_card(real_set_B).second;
                    break;
                }
            }
            if(now_compare_A.first > compare_A.first || (now_compare_A.first == compare_A.first && compare_A.second < now_compare_A.second)){
                compare_A.first = now_compare_A.first;
                compare_A.second = now_compare_A.second;
            }
            if(now_compare_B.first > compare_B.first || (now_compare_B.first == compare_B.first && compare_B.second < now_compare_B.second)){
                compare_B.first = now_compare_B.first;
                compare_B.second = now_compare_B.second;
            }
        }
    }
    if(compare_A.first > compare_B.first || compare_A.first == compare_B.first && compare_A.second > compare_B.second)
        return 1;
    if(compare_A.first == compare_B.first && compare_A.second == compare_B.second){
        return double_check(A, B, compare_A.first, compare_A.second);
    }
    return 0;
}

int double_check(card_set A, card_set B, int type, int represent_number){
    int own_A[14] = {0};
    int own_B[14] = {0};
    if(type != 5){
        for(int i=0;i<A.card_set.size();i++){
            own_A[A.card_set[i].point] ++;
            own_B[B.card_set[i].point] ++;
        }
    }
    if(type == 8){
        return 1;
    }
    if(type == 7){
        for(int i=13;i>0;i--){
            if(i != represent_number){
                if(own_A[i] != 0 && own_B[i] == 0)
                    return 1;
                else if(own_A[i] == 0 && own_B[i] != 0)
                    return 0;
            }
        }
        return 1;
    }
    if(type == 6){
        for(int i=13;i>0;i--){
            if(i != represent_number){
                if(own_A[i] == 2 && own_B[i] < 2)
                    return 1;
                else if(own_A[i] < 2 && own_B[i] == 2)
                    return 0;
            }
        }
        return 1;
    }
    if(type == 5){
        int color = 0;
        for(int j=1;j<=4;j++){
            int count = 0;
            for(int i=0;i<A.card_set.size();i++){
                if(A.card_set[i].color == j){
                    count++;
                }
                if(count == 5)
                color = j;
                break;
            }
            if(color != 0)
                break;
        }
        for(int i=0;i<A.card_set.size();i++){
            if(A.card_set[i].color == color)
                own_A[A.card_set[i].point] ++;
            if(B.card_set[i].color == color)
                own_B[B.card_set[i].point] ++;
        }
        for(int i=13;i>0;i--){
            if(own_A[i] != 0 && own_B[i] == 0)
                return 1;
            if(own_A[i] == 0 && own_B[i] != 0)
                return 0;
        }
        return 1;
    }
    if(type == 4){
        return 1;
    }
    if(type <= 3){
        for(int i=13;i>0;i--){
            if(i != represent_number){
                if(own_A[i] != 0 && own_B[i] == 0)
                    return 1;
                else if(own_A[i] == 0 && own_B[i] != 0)
                    return 0;
            }
        }
        return 1;
    }
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
                AI_win_rate = check_win_rate(current_round, all_player[i].own);
                select = AI_select(antes, AI_win_rate, all_player[i].AI_budget);
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
        // draw_selection_window(raise_to);
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
                al_draw_bitmap(poker_img[{0, 0}], p.pos_x - player_card_width, p.pos_y - player_card_height, 0);
                al_draw_bitmap(poker_img[{0, 0}], p.pos_x, p.pos_y - player_card_height, 0);
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
            ALLEGRO_BITMAP *card = al_load_bitmap(s.c_str());
            poker_img[{i, j}] = card;
        }
    }
    ALLEGRO_BITMAP *card = al_load_bitmap("./card_set/red_back.png");
    poker_img[{0, 0}] = card;
    info = al_load_bitmap("./msg_pic/player_info.png");
    selection_window = al_load_bitmap("./raise_amount_window.png");
    bg_call = al_load_bitmap("./texas_bg_call.png");
    bg_check = al_load_bitmap("./texas_bg_check.png");
    bg_all_in = al_load_bitmap("./texas_bg_all_in.png");
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

// int AI_select_raise_amount(int budget_amount, double win_rate, int antes){
//     if(win_rate <= 0.7){
//         if(antes >= budget_amount)
//             return antes * 0.5;
//         return budget_amount * 0.3;
//     }
//     if(win_rate <= 0.8){
//         if(antes >= budget_amount)
//             return antes * 0.7;
//         return budget_amount * 0.6;
//     }
//     if(win_rate <= 0.9){
//         if(antes >= budget_amount)
//             return antes * 1;
//         return budget_amount * 0.8;
//     }
// }

// int AI_select(int now_antes,double win_rate, int goal_antes)
// {
//     if(win_rate <= 0.2)
//         return FOLD;
//     if(win_rate <= 0.6 && goal_antes == now_antes)
//         return CALL;
//     if(win_rate <= 0.9)
//         return RAISE;
//     return ALL_IN;
// }

double check_win_rate(int current_round, card_set now_card){
    double win_rate;
    int score = 0;
    if(current_round == 0){
        now_card.sort_out();
        if(now_card.card_set[0].color == now_card.card_set[1].color)
            score += 200;
        if(now_card.card_set[0].point == now_card.card_set[1].color)
            score += 500;
        score += now_card.card_set[0].point * 10;
        score += now_card.card_set[1].point * 20;
    }
    else if(current_round == 1){
        now_card.sort_out();
        int represent_number = 0;
        //重複牌加權
        if(now_card.card_set[0].point == now_card.card_set[1].point){
            score += 300;
            represent_number = now_card.card_set[0].point;
        }
        for(int i=0;i<on_board.card_set.size();i++){
            for(int j=0;j<now_card.card_set.size();j++){
                if(on_board.card_set[i].point == now_card.card_set[j].point){
                    if(represent_number == now_card.card_set[j].point)
                        score += 500;
                    else{
                        score += 300;
                        represent_number = now_card.card_set[j].point;
                    }
                }
            }
        }
        //順子加權
        int own[14];
        for(int i=0;i<on_board.card_set.size();i++)
            own[on_board.card_set[i].point]++;
        for(int i=0;i<now_card.card_set.size();i++)
            own[now_card.card_set[i].point]++;
        int conti = 0;
        for(int i=0;i<13;i++){
            if(own[i] != 0){
                conti++;
            }
            else{
                if(conti == 5)
                    return 1;
                if(conti == 4)
                    score += 100;
                conti = 0;
            }
        }
        //同花加權
        if(now_card.card_set[0].color == now_card.card_set[1].color){
            int count = 0;
            for(int i=0;i<on_board.card_set.size();i++){
                if(on_board.card_set[i].color == now_card.card_set[0].color)
                    count++;
            }
            if(count == 1)
                score += 50;
            if(count == 2)
                score += 500;
            if(count == 3)
                score += 100;
        }
        bool same_color = true;
        int color = 0;
        for(int i=1;i<on_board.card_set.size();i++){
            if(on_board.card_set[i].color != on_board.card_set[i-1].color)
                same_color = false;
            else
                color = on_board.card_set[i-1].color;
        }
        if(same_color){
            for(int i=0;i<now_card.card_set.size();i++){
                if(now_card.card_set[i].color == color){
                    score += 50;
                    break;
                }
                else if(i = now_card.card_set.size()-1)
                    score -= 300;
            }
        }
        //單張加權
        if(represent_number == 0){
            score += now_card.card_set[0].point * 8;
            score += now_card.card_set[1].point * 15;
        }
    }
    else if(current_round == 2){
        now_card.sort_out();
        int represent_number = 0;
        //重複牌加權
        if(now_card.card_set[0].point == now_card.card_set[1].point){
            score += 200;
            represent_number = now_card.card_set[0].point;
        }

        for(int i=0;i<on_board.card_set.size();i++){
            for(int j=0;j<now_card.card_set.size();j++){
                if(on_board.card_set[i].point == now_card.card_set[j].point){
                    if(represent_number == now_card.card_set[j].point)
                        score += 600;
                    else{
                        score += 200;
                        represent_number = now_card.card_set[j].point;
                    }
                }
            }
        }
        //順子加權
        int own[14];
        for(int i=0;i<on_board.card_set.size();i++)
            own[on_board.card_set[i].point]++;
        for(int i=0;i<now_card.card_set.size();i++)
            own[now_card.card_set[i].point]++;
        int conti = 0;
        for(int i=0;i<13;i++){
            if(own[i] != 0){
                conti++;
            }
            else{
                if(conti == 5)
                    return 1;
                if(conti == 4)
                    score += 50;
                conti = 0;
            }
        }
        //同花加權
        if(now_card.card_set[0].color == now_card.card_set[1].color){
            int count = 0;
            for(int i=0;i<on_board.card_set.size();i++){
                if(on_board.card_set[i].color == now_card.card_set[0].color)
                    count++;
            }
            if(count == 2)
                score += 200;
            if(count == 3)
                score += 500;
            if(count == 4)
                represent_number = 0;
        }
        bool same_color = true;
        int color = 0;
        for(int i=1;i<on_board.card_set.size();i++){
            if(on_board.card_set[i].color != on_board.card_set[i-1].color)
                same_color = false;
            else
                color = on_board.card_set[i-1].color;
        }
        if(same_color){
            for(int i=0;i<now_card.card_set.size();i++){
                if(now_card.card_set[i].color == color){
                    score += 300;
                    break;
                }
                else if(i = now_card.card_set.size()-1)
                    score -= 400;
            }
        }
        //單張加權
        if(represent_number == 0){
            score += now_card.card_set[0].point * 8;
            score += now_card.card_set[1].point * 15;
        }
    }
    else if(current_round == 3){
        int represent_number = 0;
        //重複牌加權
        if(now_card.card_set[0].point == now_card.card_set[1].point){
            score += 100;
            represent_number = now_card.card_set[0].point;
        }

        for(int i=0;i<on_board.card_set.size();i++){
            for(int j=0;j<now_card.card_set.size();j++){
                if(on_board.card_set[i].point == now_card.card_set[j].point){
                    if(represent_number == now_card.card_set[j].point)
                        score += 700;
                    else{
                        score += 100;
                        represent_number = now_card.card_set[j].point;
                    }
                }
            }
        }
        //順子加權
        int own[14];
        bool flag = false;
        for(int i=0;i<on_board.card_set.size();i++)
            own[on_board.card_set[i].point]++;
        int conti = 0;
        for(int i=0;i<13;i++){
            if(own[i] != 0){
                conti++;
            }
            else{
                if(conti == 4)
                    flag = 1;
                conti = 0;
            }
        }
        for(int i=0;i<now_card.card_set.size();i++)
            own[now_card.card_set[i].point]++;
        conti = 0;
        for(int i=0;i<13;i++){
            if(own[i] != 0){
                conti++;
            }
            else{
                if(conti == 5)
                    return 1;
                conti = 0;
            }
        }
        if(flag)
            score -= 200;
        //同花加權
        if(now_card.card_set[0].color == now_card.card_set[1].color){
            int count = 0;
            for(int i=0;i<on_board.card_set.size();i++){
                if(on_board.card_set[i].color == now_card.card_set[0].color)
                    count++;
            }
            if(count == 2)
                score += 200;
            if(count == 3)
                score += 500;
            if(count == 4)
                represent_number = 0;
        }
        bool same_color = true;
        int color = 0;
        for(int i=1;i<on_board.card_set.size();i++){
            if(on_board.card_set[i].color != on_board.card_set[i-1].color)
                same_color = false;
            else
                color = on_board.card_set[i-1].color;
        }
        if(same_color){
            for(int i=0;i<now_card.card_set.size();i++){
                if(i = now_card.card_set.size()-1)
                    score -= 1000;
            }
        }
        //單張加權
        if(represent_number == 0 || (now_card.card_set[1].point == now_card.card_set[0].point)){
            score += now_card.card_set[0].point * 8;
            score += now_card.card_set[1].point * 15;
        }
    }
    win_rate = score / 100;
    if(win_rate > 1)
        win_rate = 1;
    return win_rate;
}



pair<bool,int> straight_flush(card_set now_card_set){
    now_card_set.sort_out();
    poker_card last_card = now_card_set.card_set[0];
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(last_card.color != now_card.color || now_card.point != last_card.point - 1)
            return {false, 0};
        last_card = now_card;
    }
    return {true,last_card.point};
}

pair<bool,int> four_of_a_kind(card_set now_card_set){
    now_card_set.sort_out();
    int count = 1;
    poker_card last_card = now_card_set.card_set[0];
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(last_card.point == now_card.point)
            count++;
        else
            count = 1;
        last_card = now_card;
        if(count == 4)
            return {true, last_card.point};
    }
    return {false, 0}; 
}

pair<bool,int> full_house(card_set now_card_set){
    now_card_set.sort_out();
    int status = 1;
    int count = 1;
    poker_card last_card = now_card_set.card_set[0];
    int save_pt;
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(now_card.point == last_card.point && status == 1)
            count++;
        else if(status == 1){
            if(count == 2)
                save_pt = now_card.point;
            if(count == 3)
                save_pt = last_card.point;
            else
                return {false, 0};
            status = 2;
        }
        if(now_card.point != last_card.point && status == 2)
            return {false, 0};
        last_card = now_card;
    }
    return {true,save_pt};   
}

pair<bool,int> flush(card_set now_card_set){
    now_card_set.sort_out();
    poker_card last_card = now_card_set.card_set[0];
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(last_card.color == now_card.color){
            last_card = now_card;
            continue;
        }
        return {false, 0};
    }
    return {true, last_card.point};
}

pair<bool,int> straight(card_set now_card_set){
    now_card_set.sort_out();
    poker_card last_card = now_card_set.card_set[0];
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(now_card.point != last_card.point - 1)
            return {false, 0};
        last_card = now_card;
    }
    return {true,last_card.point};
}

pair<bool,int> three_of_a_kind(card_set now_card_set){
    now_card_set.sort_out();
    int count = 1;
    poker_card last_card = now_card_set.card_set[0];
    for(int i=1;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        if(last_card.point == now_card.point)
            count++;
        else
            count = 1;
        last_card = now_card;
        if(count == 3)
            return {true, last_card.point};
    }
    return {false, 0}; 
}
////
pair<bool,int> two_pair(card_set now_card_set){
    now_card_set.sort_out();
    int own_pt[14] = {0};
    int count = 0;
    int save_pt;
    for(int i=0;i<5;i++){
        poker_card now_card = now_card_set.card_set[i];
        own_pt[now_card.point]++;
    }
    for(int i=0;i<13;i++){
        if(own_pt[i] == 2){
            count++;
            save_pt = i;
        }
    }
    if(count == 2){
        return {true, save_pt};
    }
    return {false, 0};
}
////
pair<bool,int> one_pair(card_set now_card_set){
    now_card_set.sort_out();
    int count = 1;
    poker_card last_card = now_card_set.card_set[4];
    for(int i=3;i>=0;i--){
        poker_card now_card = now_card_set.card_set[i];
        if(last_card.point == now_card.point)
            return {true, last_card.point};
        else
        last_card = now_card;
    }
    return {false, 0}; 
}

pair<bool,int> high_card(card_set now_card_set){
    now_card_set.sort_out();
    return {true, now_card_set.card_set[4].point};
}