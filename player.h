#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <string>
using namespace std;

class player
{
public:
    player(){};
    player(string account, string password, int budget, int now_processing, int story_level)
    {
        account = account;
        password = password;
        budget = budget;
        now_processing = now_processing;
        story_level = story_level;
    }
    string account;
    string password;
    int budget;
    int now_processing; //story mode
    int story_level;
    bool is_main_user = false;
};

#endif //PLAYER_H_INCLUDED
