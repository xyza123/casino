#include "roulette.h"
#include <iostream>
#include <map>
using namespace std;
int gambling_odd[9] = {35, 17, 11, 8, 1, 1, 2, 2, 1}; //賠率
// vector<player> all_player;
roulettle now_game;
vector<roulettle_bet> all_bet;
vector<pair<int, int>> bet_postion;
vector<string> chip_set;                                                             // store the variable names of different chip img
vector<pair<string, pair<int, int>>> drawing_set;                                    // store the current on board (variable, (x, y))
vector<pair<pair<pair<int, int>, pair<int, int>>, pair<int, int>>> board_bet_number; // referencing board_bet_number
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
    RETURN,
    EXIT
};
enum
{
    RCHIP,
    BCHIP,
    BKCHIP,
    GCHIP,
    RFCHIP,
    BFCHIP,
    BKFCHIP,
    GFCHIP
};
roulettle::roulettle()
{
    pair<int, int> now;

    now = {0, GREEN};
    number.push_back(now);

    now = {1, RED};
    number.push_back(now);

    now = {2, BLACK};
    number.push_back(now);

    now = {3, RED};
    number.push_back(now);

    now = {4, BLACK};
    number.push_back(now);

    now = {5, RED};
    number.push_back(now);

    now = {6, BLACK};
    number.push_back(now);

    now = {7, RED};
    number.push_back(now);

    now = {8, BLACK};
    number.push_back(now);

    now = {9, RED};
    number.push_back(now);

    now = {10, BLACK};
    number.push_back(now);

    now = {11, BLACK};
    number.push_back(now);

    now = {12, RED};
    number.push_back(now);

    now = {13, BLACK};
    number.push_back(now);

    now = {14, RED};
    number.push_back(now);

    now = {15, BLACK};
    number.push_back(now);

    now = {16, RED};
    number.push_back(now);

    now = {17, BLACK};
    number.push_back(now);

    now = {18, RED};
    number.push_back(now);

    now = {19, RED};
    number.push_back(now);

    now = {20, BLACK};
    number.push_back(now);

    now = {21, RED};
    number.push_back(now);

    now = {22, BLACK};
    number.push_back(now);

    now = {23, RED};
    number.push_back(now);

    now = {24, BLACK};
    number.push_back(now);

    now = {25, RED};
    number.push_back(now);

    now = {26, BLACK};
    number.push_back(now);

    now = {27, RED};
    number.push_back(now);

    now = {28, BLACK};
    number.push_back(now);

    now = {29, BLACK};
    number.push_back(now);

    now = {30, RED};
    number.push_back(now);

    now = {31, BLACK};
    number.push_back(now);

    now = {32, RED};
    number.push_back(now);

    now = {33, BLACK};
    number.push_back(now);

    now = {34, RED};
    number.push_back(now);

    now = {35, BLACK};
    number.push_back(now);

    now = {36, RED};
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
    init_chip_drawing_set();
    init_roulette_resources();
    for (int i = 0; i < all_player.size(); i++)
    {
        player now_player = all_player[i];
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
    string floating_cursor;
    double deg = 0;
    int count_spinning = 0;
    int bet_type;
    vector<pair<int, int>> covered_number;
    while (!done)
    {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
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
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (start_spinning)
            {
                count_spinning++;
                if (count_spinning < 180)
                    deg++;
                else if (count_spinning < 240)
                    deg += 0.5;
                else if (count_spinning < 300)
                    deg += 0.25;
                else if (count_spinning < 360)
                    deg += 0.125;
                if (count_spinning >= 360)
                    start_spinning = false;
                if (deg >= 360)
                    deg = 0;
            }
            else
                count_spinning = 0;
        }
        else if (event.type == AELLGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            mouse_position = check_mouse_range(mouse_x, mouse_y);
            if (event.mouse.button == MOUSE_RIGHT)
                mouse_position = nothing;
            if (mouse_position == nothing)
                status = false;
            else if (mouse_position == IN_BOARD)
            {
                drawing_set.push_back({floating_cursor, {mouse_x, mouse_y}});
                roulettle_bet now_bet(bet_amount(floating_cursor), bet_type, &now_player, covered_number);
                all_set.push_back(now_bet);
            }
            else if (mouse_position == IN_NEW_BET_RANGE)
            {
                floating_cursor = check_potential_chip_selection(mouse_x, mouse_y);
            }
            else if (mouse_position == RETURN)
                drawing_set.pop_back();
            else if (mouse_position == EXIT)
                done = true;
        }
        draw_roulette_table();
        if (status)
            al_draw_bitmap(floating_cursor, mouse_x, mouse_y, 0);
        al_draw_rotated_bitmap(roulette, 600, 598, -50, window_height + 100, deg * 3.14159 / 180, 0);
        al_flip_display();
        if (status)
        {
            if (mouse_position == IN_BOARD)
            {
                covered_number = check_hovered(mouse_x, mouse_y);
                bet_type = check_bet_type(covered_number);
            }
            if (mouse_position == IN_NEW_BET_RANGE)
            {
                floating_cursor = check_potential_chip_selection(mouse_x, mouse_y);
            }
        }
        else
        {
            if (mouse_position == IN_BOARD)
            {
                covered_number = check_hovered(mouse_x, mouse_y);
                bet_type = check_bet_type(covered_number);
            }
            if (mouse_position == IN_NEW_BET_RANGE)
            {
                floating_cursor = check_potential_chip_selection(mouse_x, mouse_y);
            }
        }
    }
}

bool is_in_exit_range(int x, int y)
{
    if (x >= 1 && x <= 5 && y >= 3 && y <= 10)
        return true;
    else
        return false;
}

bool is_in_new_bet_range(int x, int y)
{
    if (x >= 1 && x <= 5 && y >= 3 && y <= 10)
        return true;
    else
        return false;
}

bool is_in_exist_bet_range(int x, int y)
{
    if (x >= 1 && x <= 5 && y >= 3 && y <= 10)
        return true;
    else
        return false;
}

bool is_in_board_range(int x, int y)
{
    if (x >= 1 && x <= 5 && y >= 3 && y <= 10)
        return true;
    else
        return false;
}
string check_potential_chip_selection(int x, int y)
{
    int sc = 0;
    if (x >= 1 && x <= 5 && y >= 4 && y <= 10)
        sc = RCHIP;
    else if (x >= 1 && x <= 5 && y >= 4 && y <= 10)
        sc = BCHIP;
    else if (x >= 1 && x <= 5 && y >= 4 && y <= 10)
        sc = BKCHIP;
    else if (x >= 1 && x <= 5 && y >= 4 && y <= 10)
        sc = GCHIP;
    return chip_set[sc];
}
int check_mouse_range(int mouse_x, int mouse_y)
{
    if (is_in_exit_range(mouse_x, mouse_y))
        return EXIT;
    if (is_in_new_bet_range(mouse_x, mouse_y))
        return IN_NEW_BET_RANGE;
    if (is_in_board_range(mouse_x, mouse_y))
        return IN_BOARD;
    if (is_in_return_range(mouse_x, mouse_y))
        return RETURN;
    return NOTHING;
}
void draw_roulette_table()
{
    al_draw_bitmap(roulette_board, 0, 0, 0);
    for (auto v : drawing_set)
        al_draw_bitmap(v.first, v.second.first - 50, v.second.second - 50, 0);
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
    bfchip = al_load_bitmap("./chip_set/bfChip.png");
    bkfchip = al_load_bitmap("./chip_set/kfChip.png");
    gfchip = al_load_bitmap("./chip_set/gfChip.png");
}

void init_chip_drawing_set()
{
    chip_set.clear();
    chip_set.push_back("rchip");
    chip_set.push_back("bchip");
    chip_set.push_back("bkchip");
    chip_set.push_back("gchip");
    chip_set.push_back("rfchip");
    chip_set.push_back("bfchip");
    chip_set.push_back("bkfchip");
    chip_set.push_back("gfchip");
}

int bet_amount(int chip_type)
{
    switch (chip_type)
    {
    case RCHIP:
        return 1;
        break;
    case BCHIP:
        return 2;
        break;
    case BKCHIP:
        return 3;
        break;
    case GCHIP:
        return 4;
        break;
    default:
        break;
    }
    return 0;
}

int check_bet_type(vector<pair<int, int>> bet)
{
    if (bet.size() == 1)
    {
        if (bet[0].first == 0)
            return COLOR;
        else if (bet[0].second == GREEN)
        {
            switch (bet[0].first)
            {
            case 0:
                return SINGLE;
            case 1:
                return ODD_AND_EVEN;
            case 2:
                return ODD_AND_EVEN;
            case 3:
                return TWELVE_NUMBER;
            case 4:
                return TWELVE_NUMBER;
            case 5:
                return TWELVE_NUMBER;
            default:
                break;
            }
        }
        return SINGLE;
    }
    else if (bet.size() == 2)
        return SPLIT;
    else if (bet.size() == 3)
        return STREET;
    else if (bet.size() == 4)
        return corner;
}

void init_board_bet_number()
{
    board_bet_number.push_back({{{290, 165}, {377, 575}}, {0, GREEN}});
    board_bet_number.push_back({{{1080, 670}, {1257, 760}}, {1, GREEN}});
    board_bet_number.push_back({{{553, 670}, {731, 760}}, {2, GREEN}});
    board_bet_number.push_back({{{377, 573}, {731, 670}}, {3, GREEN}});
    board_bet_number.push_back({{{731, 573}, {1080, 670}}, {4, GREEN}});
    board_bet_number.push_back({{{1080, 573}, {1433, 670}}, {5, GREEN}});
    board_bet_number.push_back({{{377, 670}, {553, 760}}, {6, GREEN}});
    board_bet_number.push_back({{{1257, 670}, {1433, 760}}, {7, GREEN}});
    board_bet_number.push_back({{{1433, 440}, {1522, 575}}, {8, GREEN}});
    board_bet_number.push_back({{{1433, 300}, {1522, 440}}, {9, GREEN}});
    board_bet_number.push_back({{{1433, 440}, {1522, 300}}, {10, GREEN}});
    board_bet_number.push_back({{{377, 440}, {467, 575}}, {1, RED}});
    board_bet_number.push_back({{{377, 300}, {467, 440}}, {2, BLACK}});
    board_bet_number.push_back({{{377, 165}, {467, 300}}, {3, RED}});
    board_bet_number.push_back({{{467, 440}, {553, 575}}, {4, BLACK}});
    board_bet_number.push_back({{{467, 300}, {553, 440}}, {5, RED}});
    board_bet_number.push_back({{{467, 165}, {553, 300}}, {6, BLACK}});
    board_bet_number.push_back({{{553, 440}, {640, 575}}, {7, RED}});
    board_bet_number.push_back({{{553, 300}, {640, 440}}, {8, BLACK}});
    board_bet_number.push_back({{{553, 165}, {640, 300}}, {9, RED}});
    board_bet_number.push_back({{{640, 440}, {731, 575}}, {10, BLACK}});
    board_bet_number.push_back({{{640, 300}, {731, 440}}, {11, BLACK}});
    board_bet_number.push_back({{{640, 165}, {731, 300}}, {12, RED}});
    board_bet_number.push_back({{{731, 440}, {818, 575}}, {13, BLACK}});
    board_bet_number.push_back({{{731, 300}, {818, 440}}, {14, RED}});
    board_bet_number.push_back({{{731, 165}, {818, 300}}, {15, BLACK}});
    board_bet_number.push_back({{{818, 440}, {905, 575}}, {16, RED}});
    board_bet_number.push_back({{{818, 300}, {905, 440}}, {17, BLACK}});
    board_bet_number.push_back({{{818, 165}, {905, 300}}, {18, RED}});
    board_bet_number.push_back({{{905, 440}, {974, 575}}, {19, RED}});
    board_bet_number.push_back({{{905, 300}, {974, 440}}, {20, BLACK}});
    board_bet_number.push_back({{{905, 165}, {974, 300}}, {21, RED}});
    board_bet_number.push_back({{{974, 440}, {1080, 575}}, {22, BLACK}});
    board_bet_number.push_back({{{974, 300}, {1080, 440}}, {23, RED}});
    board_bet_number.push_back({{{974, 165}, {1080, 300}}, {24, BLACK}});
    board_bet_number.push_back({{{1080, 440}, {1169, 575}}, {25, RED}});
    board_bet_number.push_back({{{1080, 300}, {1169, 440}}, {26, BLACK}});
    board_bet_number.push_back({{{1080, 165}, {1169, 300}}, {27, RED}});
    board_bet_number.push_back({{{1169, 440}, {1257, 575}}, {28, BLACK}});
    board_bet_number.push_back({{{1169, 300}, {1257, 440}}, {29, BLACK}});
    board_bet_number.push_back({{{1169, 165}, {1257, 300}}, {30, RED}});
    board_bet_number.push_back({{{1257, 440}, {1346, 575}}, {31, BLACK}});
    board_bet_number.push_back({{{1257, 300}, {1346, 440}}, {32, RED}});
    board_bet_number.push_back({{{1257, 165}, {1346, 300}}, {33, BLACK}});
    board_bet_number.push_back({{{1346, 440}, {1433, 575}}, {34, RED}});
    board_bet_number.push_back({{{1346, 300}, {1433, 440}}, {35, BLACK}});
    board_bet_number.push_back({{{1346, 165}, {1433, 300}}, {36, RED}});
}

// color green:
//     0: 0
//     1: odd
//     2: even
//     3: 1st twelve
//     4: 2nd twelve
//     5: 3rd twelve
//     6: 1-18
//     7: 19-36
//     8: MOD 1
//     9: MOD 2
//     10: MOD 3

vector<pair<int, int>> check_hovered(int x, int y)
{
    vector<pair<int, int>> H;
    int radius = 50;
    int s1[] = {1, 1, -1, -1};
    int s2[] = {1, -1, 1, -1};
    int hovered = false;
    for (auto b : board_bet_number)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 1; j <= radius; j++)
            {
                for (int k = 1; k <= radius; k++)
                {
                    if ((x + s1[i] * j) >= b.first.first.first && (x + s1[i] * j) <= b.first.second.first && (y + s2[i] * k) >= b.first.first.second && (y + s2[i] * k) <= b.first.second.second)
                    {
                        H.push_back(b.second);
                        hovered = true;
                        break;
                    }
                }
                if (hovered)
                    break;
            }
            if (hovered)
            {
                hovered = false;
                break;
            }
        }
    }
}