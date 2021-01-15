#include "roulette.h"
#include <iostream>
using namespace std;
int gambling_odd[9] = {35, 17, 11, 8, 1, 1, 2, 2, 1}; //賠率

// vector<player> all_player;
roulettle now_game;
vector<roulettle_bet> all_bet;
vector<pair<int, int>> bet_postion;
ALLEGRO_BITMAP *roulette = NULL;
ALLEGRO_BITMAP *roulette_board = NULL;
ALLEGRO_BITMAP *rchip = NULL;
ALLEGRO_BITMAP *gchip = NULL;
ALLEGRO_BITMAP *bchip = NULL;
ALLEGRO_BITMAP *bkchip = NULL;
ALLEGRO_BITMAP *rfchip = NULL;
ALLEGRO_BITMAP *gfchip = NULL;
ALLEGRO_BITMAP *bfchip = NULL;
ALLEGRO_BITMAP *bkfchip = NULL;
int bet_length;
enum
{
    NOTHING,
    IN_BOARD,
    IN_NEW_BET_RANGE,
    EXIT
};
roulettle::roulettle()
{
    pair<int, int> now;

    now = {0, green};
    number.push_back(now);

    now = {1, red};
    number.push_back(now);

    now = {2, black};
    number.push_back(now);

    now = {3, red};
    number.push_back(now);

    now = {4, black};
    number.push_back(now);

    now = {5, red};
    number.push_back(now);

    now = {6, black};
    number.push_back(now);

    now = {7, red};
    number.push_back(now);

    now = {8, black};
    number.push_back(now);

    now = {9, red};
    number.push_back(now);

    now = {10, black};
    number.push_back(now);

    now = {11, black};
    number.push_back(now);

    now = {12, red};
    number.push_back(now);

    now = {13, black};
    number.push_back(now);

    now = {14, red};
    number.push_back(now);

    now = {15, black};
    number.push_back(now);

    now = {16, red};
    number.push_back(now);

    now = {17, black};
    number.push_back(now);

    now = {18, red};
    number.push_back(now);

    now = {19, red};
    number.push_back(now);

    now = {20, black};
    number.push_back(now);

    now = {21, red};
    number.push_back(now);

    now = {22, black};
    number.push_back(now);

    now = {23, red};
    number.push_back(now);

    now = {24, black};
    number.push_back(now);

    now = {25, red};
    number.push_back(now);

    now = {26, black};
    number.push_back(now);

    now = {27, red};
    number.push_back(now);

    now = {28, black};
    number.push_back(now);

    now = {29, black};
    number.push_back(now);

    now = {30, red};
    number.push_back(now);

    now = {31, black};
    number.push_back(now);

    now = {32, red};
    number.push_back(now);

    now = {33, black};
    number.push_back(now);

    now = {34, red};
    number.push_back(now);

    now = {35, black};
    number.push_back(now);

    now = {36, red};
    number.push_back(now);
}

roulettle_bet::roulettle_bet(int amount, int type, vector<pair<int, int>> bet_number, player &now_player)
{
    now_player.budget -= amount;
    connected_player = &now_player;
    amount = amount;
    type = type;
    bet_number = bet_number;
}

void create_roulette_game(vector<player> all_player)
{
    cout << "check point 1" << endl;
    init_roulette_resources();
    for (int i = 0; i < all_player.size(); i++)
    {
        cout << "hi";
        player now_player = all_player[i];
        cout << "passed" << endl;
        player_bet(now_player);
    }
    //check_win();
}

void player_bet(player &now_player)
{
    bool done = false;
    bool start_spinning = false;
    int mouse_x, mouse_y;
    bool status = false; //0:not select bet, 1:select bet
    int select;          //select_bet_number
    int mouse_position;
    // roulettle_bet now_bet;
    double deg = 0;
    int count_spinning = 0;
    while (!done)
    {
        cout << "I'm in!" << endl;
        /*do
        {
            al_wait_event(event_queue, &event);
            if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            }
            mouse_position = check_mouse_range(mouse_x, mouse_y);
            if (mouse_position == nothing && event.mouse.bottom == MOUSE_LEFT)
                status = false;
        } while (event.mouse.bottom != MOUSE_LEFT || mouse_position == nothing) if (mouse_position == exit) break;
        */
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (start_spinning)
            {
                count_spinning++;
                if (count_spinning < 120)
                    deg++;
                else if (count_spinning < 180)
                    deg += 0.5;
                else if (count_spinning < 240)
                    deg += 0.25;
                else if (count_spinning < 300)
                    deg += 0.125;
                else if (count_spinning >= 300)
                    start_spinning = false;
                if (deg >= 360)
                    deg = 0;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // if (event.keyboard.keycode == ALLEGRO_KEY_S)
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_S:
                start_spinning = true;
                break;
            case ALLEGRO_KEY_E:
                done = true;
            default:
                break;
            }
        }
        draw_roulette_table();
        al_draw_rotated_bitmap(roulette, 600, 598, -50, window_height + 100, deg * 3.14159 / 180, 0);
        al_flip_display();
        // if (status)
        // {
        //     if (mouse_position == in_board)
        //     {
        //     }
        //     if (mouse_position == in_new_bet_range)
        //     {
        //     }
        // }
        // else
        // {
        //     if (mouse_position == in_board)
        //     {
        //     }
        //     if (mouse_position == in_new_bet_range)
        //     {
        //     }
        // }
    }
}

bool is_in_exit_range()
{
}

bool is_in_new_bet_range()
{
}

bool is_in_exist_bet_range()
{
}

bool is_in_board_range()
{
}

int check_mouse_range(int mouse_x, int mouse_y)
{
    // if (is_in_exit_range())
    //     return exit;
    // if (is_in_new_bet_range())
    //     return in_new_bet_range;
    // if (is_in_board_range())
    //     return in_board;
    // return nothing;
}
void draw_roulette_table()
{
    al_draw_bitmap(roulette_board, 0, 0, 0);
}
void init_roulette_resources()
{
    roulette = al_load_bitmap("./roulette.png");
    roulette_board = al_load_bitmap("./roulette_board.png");
    rchip = al_load_bitmap("./chip_set/rchip.png");
    bchip = al_load_bitmap("./chip_set/bchip.png");
    bkchip = al_load_bitmap("./chip_set/bkchip.png");
    gchip = al_load_bitmap("./chip_set/gchip.png");
    rfchip = al_load_bitmap("./chip_set/rfChip.png");
    gfchip = al_load_bitmap("./chip_set/gfChip.png");
    bfchip = al_load_bitmap("./chip_set/bfChip.png");
    bkfchip = al_load_bitmap("./chip_set/kfChip.png");
    cout << "load complete" << endl;
}
