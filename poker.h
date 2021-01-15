#ifndef POKER_H_INCLUDED
#define POKER_H_INCLUDED
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

int Rand(int n);

class poker_card
{
public:
    int point; // 1~13
    int color; // 1:梅花 2:方塊 3:紅桃 4:黑桃
               //   C     D     H     S
};

class card_set
{
public:
    void sort_out();
    vector<poker_card> card_set;
};

class deck
{
public:
    deck();
    void licensing_card(card_set &now_player);
    void refresh_deck();
    void card_shuffle();
    vector<poker_card> remaining_card;
};

bool card_cmp(poker_card &a, poker_card &b);
#endif //POKER_H_INCLUDED