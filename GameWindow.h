#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED
#include <vector>
#include <list>
#include <string>
#include <time.h>
#include "Texas.h"
#include "global.h"
#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7

// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();

    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw_running_map();
    void draw_log_in_scene();
    void draw_game_mode_scene();
    void draw_lobby_scene();
    void draw_msg_box(int);
    void draw_wheel_msg_box();
    void draw_texas_table(int);
    // process of updated event
    int process_event();
    int log_in_event();
    int game_mode_event();
    int register_event();
    int lobby_event();
    int Texas_event();
    int Roulette_event();
    int Blackjack_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);
    // detect if a tower will be constructed on road
    bool isOnRoad();

public:
    bool initial = true;

private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *logInBg = NULL;
    ALLEGRO_BITMAP *mouse_pic = NULL;
    ALLEGRO_BITMAP *vegas = NULL;
    ALLEGRO_BITMAP *logInMsg = NULL;
    ALLEGRO_BITMAP *lobbyBg = NULL;
    ALLEGRO_BITMAP *gameMode = NULL;
    ALLEGRO_BITMAP *msgBox = NULL;
    ALLEGRO_BITMAP *texas_table_call = NULL;
    ALLEGRO_BITMAP *texas_table_check = NULL;
    ALLEGRO_BITMAP *texas_table_all_in = NULL;

    ALLEGRO_DISPLAY *display = NULL;
    // ALLEGRO_FONT *font = NULL;
    // ALLEGRO_FONT *Medium_font = NULL;
    // ALLEGRO_FONT *Large_font = NULL;

    // ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    // ALLEGRO_EVENT event;
    ALLEGRO_TIMER *texas_timer = NULL;
    ALLEGRO_TIMER *roulette_timer = NULL;
    ALLEGRO_TIMER *blackjack_timer = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;
    int mouse_x, mouse_y;
    int tmp_x, tmp_y;
    int game_mode;
    bool judge_next_window[20] = {false};
    bool redraw = false;
    bool mute = false;
    bool soundOn = true;
    int window = 0;
    bool input = false, complete = false;
    string userName = " ", password = " ";
    int starting_bet[3] = {10000, 500000, 1500000};
    player main_user;
};

#endif // MAINWINDOW_H_INCLUDED
