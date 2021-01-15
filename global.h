#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro5/allegro_primitives.h> //Our primitive header file
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#define font_size 12
#define grid_width 40
#define grid_height 40

#define window_width 1600
#define window_height 1200
#define field_width 600
#define field_height 600

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2

#define Num_TowerType 5
#define Num_MonsterType 4
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_EVENT event;
extern ALLEGRO_FONT *XL_font;
extern ALLEGRO_FONT *Medium_font;
extern ALLEGRO_FONT *Large_font;
enum
{
    ARCANE = 0,
    ARCHER,
    CANON,
    POISON,
    STORM
};
enum
{
    WOLF = 0,
    WOLFKNIGHT,
    DEMONNIJIA,
    CAVEMAN
};
enum // window
{
    STARTING,
    LOGIN,
    REGISTER,
    SELECTION,
    LOBBY,
    TEXAS,
    BLACKJACK,
    ROULETTE
};

enum // game mode
{
    SINGLE,
    MULTI,
    STORY
};

enum // game choices for texas holdem
{
    CALL,
    FOLD,
    RAISE,
    ALL_IN,
    CHECK
};
#endif // GLOBAL_H_INCLUDED
