#include "roulette.h"
#include "global.h"
vector<player> all_player;
roulettle now_game;
vector <roulettle_bet> all_bet;
vector <pair<int, int> >bet_postion;
int bet_length;
enum{
    nothing,
    in_board,
    in_new_bet_range,
    exit
};
roulettle::roulettle(){
    pair<int,int> now;

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

roulettle_bet::roulettle_bet(int amount, int type, vector<pair<int, int> > bet_number, player& now_player){
    now_player.budget -= amount;
    connected_player = &now_player;
    amount = amount;
    type = type;
    bet_number = bet_number;
}

void create_roulettle_game(vector<player> all_player){
    for(int i=0;i<all_player.size();i++){
        player now_player = all_player[i];
        player_round(now_player);
    }
    //check_win();
}
/*
void player_round(player& now_player){
    int mouse_x, mouse_y;
    bool status = false;//0:not select bet, 1:select bet
    int select; //select_bet_number
    int mouse_position;
    roulettle_bet now_bet;
    while(1){
        do{
            al_wait_event(event_queue, &event);
            if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            }
            mouse_position = check_mouse_range(mouse_x, mouse_y);
            if(mouse_position == nothing && event.mouse.bottom == MOUSE_LEFT)
                status = false;
        } while(event.mouse.bottom != MOUSE_LEFT || mouse_position == nothing)
        if(mouse_position == exit)
            break;
        if(status){
            if(mouse_position == in_board){
                
            }
            if(mouse_position == in_new_bet_range){

            }
        }
        else{
            if(mouse_position == in_board){
                
            }
            if(mouse_position == in_new_bet_range){

            }
        }
    }
}

bool is_in_exit_range(){

}

bool is_in_new_bet_range(){

}

bool is_in_exist_bet_range(){

}

bool is_in_board_range(){

}

bool is_in_new_bet_range(){

}

int check_mouse_range(int mouse_x, int mouse_y){
    if(is_in_exit_range())
        return exit;
    if(is_in_new_bet_range())
        return in_new_bet_range;
    if(is_in_board_range())
        return in_board_range;
    return nothing;
}
*/