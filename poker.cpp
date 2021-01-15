#include "poker.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
deck::deck()
{
    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            poker_card now;
            now.point = i;
            now.color = j;
            remaining_card.push_back(now);
        }
    }
    card_shuffle();
    return;
}
void deck::licensing_card(card_set &now_player)
{
    poker_card now_licensing = *(remaining_card.end() - 1);
    remaining_card.pop_back();
    now_player.card_set.push_back(now_licensing);
    return;
}
void deck::refresh_deck()
{
    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            poker_card now;
            now.point = i;
            now.color = j;
            remaining_card.push_back(now);
        }
    }
    card_shuffle();
    return;
}
void deck::card_shuffle()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(remaining_card.begin(), remaining_card.end(), default_random_engine(seed));
    return;
}
void card_set::sort_out()
{
    sort(card_set.begin(), card_set.end(), card_cmp);
    return;
}
int Rand(int n)
{
    return rand() % n;
}
bool card_cmp(poker_card &a, poker_card &b)
{
    if (a.point == b.point)
        return a.color < b.color;
    return a.point < b.point;
}