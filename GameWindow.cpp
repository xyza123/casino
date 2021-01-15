#include "GameWindow.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_FONT *XL_font = NULL;
ALLEGRO_FONT *Medium_font = NULL;
ALLEGRO_FONT *Large_font = NULL;
void GameWindow::game_init()
{
    char buffer[50];

    icon = al_load_bitmap("./icon.png");
    background = al_load_bitmap("./background_pic/log_in_bg.png");
    vegas = al_load_bitmap("./background_pic/vegas_bg.jpg");
    logInBg = al_load_bitmap("./background_pic/poker_bg.jpg");
    mouse_pic = al_load_bitmap("./mouse.png");
    logInMsg = al_load_bitmap("./msg_pic/log_in_msg.png");
    lobbyBg = al_load_bitmap("./background_pic/lobby_bg.psd");
    gameMode = al_load_bitmap("./background_pic/vegas_game_mode.psd");
    game_start = al_load_bitmap("./game_start.png");

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("growl.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());
    std::cout << "trying" << '\n';
    sample = al_load_sample("./main_bgm.wav");

    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());
    std::cout << "succeed\n";
}

bool GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if (mouse_x >= startx && mouse_x <= startx + width)
        if (mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

void GameWindow::game_play()
{
    int msg;

    srand(time(NULL));

    msg = -1;
    game_reset();
    game_begin();

    while (msg != GAME_EXIT)
    {
        msg = game_run();
    }

    show_err_msg(msg);
}

void GameWindow::show_err_msg(int msg)
{
    if (msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon();   // initialize the font addon
    al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    al_init_image_addon();  // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    Medium_font = al_load_font("Caviar_Dreams_Bold.ttf", 24, 0); //load medium font
    Large_font = al_load_font("Caviar_Dreams_Bold.ttf", 36, 0);  //load large font
    XL_font = al_load_font("Caviar_Dreams_Bold.ttf", 60, 0);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    game_init();
}

void GameWindow::game_begin()
{
    draw_game_mode_scene();
    // al_play_sample_instance(backgroundSound);
}

int GameWindow::game_run()
{
    int error = GAME_CONTINUE;
    if (window == STARTING)
    {
        draw_game_mode_scene();
        if (!al_is_event_queue_empty(event_queue))
        {
            error = game_mode_event();
            if (judge_next_window[3])
            {
                window = SELECTION;
            }
            else if (judge_next_window[2])
            {
                window = LOBBY;
                userName = "Visitor";
                password = "1111";
            }
        }
    }
    else if (window == SELECTION)
    {
        judge_next_window[3] = false;
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
            if (judge_next_window[0])
            {
                std::cout << "User Name: ";
                window = LOGIN;
            }
            else if (judge_next_window[1])
            {
                std::cout << "User Name: ";
                window = REGISTER;
            }
        }
    }
    else if (window == LOGIN)
    {
        judge_next_window[0] = false;
        draw_log_in_scene();
        if (!al_is_event_queue_empty(event_queue))
        {
            error = log_in_event();
            if (judge_next_window[2])
            {
                window = LOBBY;
            }
        }
    }
    else if (window == REGISTER)
    {
        judge_next_window[1] = false;
        draw_log_in_scene();
        if (!al_is_event_queue_empty(event_queue))
        {
            error = register_event();
            if (judge_next_window[2])
            {
                window = LOBBY;
            }
        }
    }
    else if (window == LOBBY)
    {
        judge_next_window[2] = false;
        judge_next_window[7] = false;
        draw_lobby_scene();
        main_user.account = userName;
        main_user.password = password;
        main_user.budget = 100000;
        main_user.now_processing = -1;
        main_user.story_level = 1;
        msgBox = al_load_bitmap("msg_box.png");
        if (!al_is_event_queue_empty(event_queue))
        {
            error = lobby_event();
            if (judge_next_window[4])
            {
                window = TEXAS;
                texas_table_call = al_load_bitmap("texas_bg_call.png");
                texas_table_check = al_load_bitmap("texas_bg_check.png");
                texas_table_all_in = al_load_bitmap("texas_bg_all_in.png");
                user_name_p = al_load_bitmap("./user_name.png");
                budget_p = al_load_bitmap("./budget.png");
                texas_timer = al_create_timer(1.0 / FPS);
                al_register_event_source(event_queue, al_get_timer_event_source(texas_timer));
            }
            else if (judge_next_window[5])
            {
                window = ROULETTE;
                roulette_timer = al_create_timer(1.0 / FPS);
                roulette_table = al_load_bitmap("./roulette_board.png");
                roulette = al_load_bitmap("./roulette.png");
                al_register_event_source(event_queue, al_get_timer_event_source(roulette_timer));
            }
            else if (judge_next_window[6])
            {
                window = BLACKJACK;
                blackjack_timer = al_create_timer(1.0 / FPS);
                al_register_event_source(event_queue, al_get_timer_event_source(blackjack_timer));
            }
        }
    }
    else if (window == TEXAS)
    {
        judge_next_window[4] = false;
        al_draw_bitmap(game_start, 0, 0, 0);
        al_flip_display();
        al_rest(3);
        draw_texas_table(CALL);
        al_start_timer(texas_timer);
        if (!al_is_event_queue_empty(event_queue))
        {
            error = Texas_event();
            if (judge_next_window[7])
            {
                window = LOBBY;
            }
        }
    }
    else if (window == ROULETTE)
    {
        // al_start_timer(roulette_timer);
        judge_next_window[5] = false;
        draw_roulette_table();
        al_flip_display();
        if (!al_is_event_queue_empty(event_queue))
        {
            error = Roulette_event();
            if (judge_next_window[7])
            {
                window = LOBBY;
            }
        }
    }

    return error;
}

int GameWindow::game_update()
{
    unsigned int i, j;
    return GAME_CONTINUE;
}

void GameWindow::game_reset()
{
    // reset game and begin
    mute = false;
    redraw = false;
    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);
}

void GameWindow::game_destroy()
{
    game_reset();
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(XL_font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);
}

int GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return GAME_EXIT;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_M:
            mute = !mute;
            if (mute)
                al_stop_sample_instance(backgroundSound);
            else
                al_play_sample_instance(backgroundSound);
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.mouse.button == MOUSE_LEFT && mouse_x >= 550 && mouse_x <= 950 && mouse_y >= 500 && mouse_y <= 700)
            judge_next_window[0] = true;
        else if (event.mouse.button == MOUSE_LEFT && mouse_x >= 530 && mouse_x <= 980 && mouse_y >= 730 && mouse_y <= 920)
            judge_next_window[1] = true;
    }
    // if (tmp_x != mouse_x && tmp_y != mouse_y)
    // {
    //     std::cout << "mouse move to (" << mouse_x << ", " << mouse_y << ")\n";
    //     tmp_x = mouse_x;
    //     tmp_y = mouse_y;
    // }
    al_flip_display();
    draw_running_map();
    return instruction;
}
int GameWindow::log_in_event()
{
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);
    if (complete)
    {
        judge_next_window[2] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_M:
            mute = !mute;
            if (mute)
                al_stop_sample_instance(backgroundSound);
            else
                al_play_sample_instance(backgroundSound);
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    if (tmp_x != mouse_x && tmp_y != mouse_y)
    {
        std::cout << "mouse move to (" << mouse_x << ", " << mouse_y << ")\n";
        tmp_x = mouse_x;
        tmp_y = mouse_y;
    }
    al_flip_display();
    if (!input)
    {
        std::cin >> userName;
        input = true;
    }
    else if (!complete)
    {
        std::cout << "Password: ";
        std::cin >> password;
        complete = true;
    }
    draw_log_in_scene();
    return instruction;
}
int GameWindow::register_event()
{
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);
    if (complete)
    {
        judge_next_window[2] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_M:
            mute = !mute;
            if (mute)
                al_stop_sample_instance(backgroundSound);
            else
                al_play_sample_instance(backgroundSound);
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    if (tmp_x != mouse_x && tmp_y != mouse_y)
    {
        std::cout << "mouse move to (" << mouse_x << ", " << mouse_y << ")\n";
        tmp_x = mouse_x;
        tmp_y = mouse_y;
    }
    al_flip_display();
    if (!input)
    {
        std::cin >> userName;
        input = true;
    }
    else if (!complete)
    {
        std::cout << "Password: ";
        std::cin >> password;
        while (1)
        {
            std::string c_password;
            std::cout << "Please enter your password again: ";
            std::cin >> c_password;
            if (c_password == password)
            {
                std::cout << "Registration Completed! Welcome!\n";
                break;
            }
            else
                std::cout << "Password doesn't match.\n";
        }
        complete = true;
    }
    draw_log_in_scene();
    return instruction;
}
int GameWindow::game_mode_event()
{
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return GAME_EXIT;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_M:
            mute = !mute;
            if (mute)
                al_stop_sample_instance(backgroundSound);
            else
                al_play_sample_instance(backgroundSound);
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.mouse.button == MOUSE_LEFT && mouse_x >= 620 && mouse_x <= 970 && mouse_y >= 535 && mouse_y <= 700)
        {
            judge_next_window[2] = true;
            game_mode = SINGLE;
        }
        else if (event.mouse.button == MOUSE_LEFT && mouse_x >= 635 && mouse_x <= 942 && mouse_y >= 742 && mouse_y <= 930)
        {
            judge_next_window[3] = true;
            game_mode = STORY;
        }
        else if (event.mouse.button == MOUSE_LEFT && mouse_x >= 600 && mouse_x <= 960 && mouse_y >= 970 && mouse_y <= 1100)
        {
            judge_next_window[3] = true;
            game_mode = MULTI;
        }
    }
    // if (tmp_x != mouse_x && tmp_y != mouse_y)
    // {
    //     std::cout << "mouse move to (" << mouse_x << ", " << mouse_y << ")\n";
    //     tmp_x = mouse_x;
    //     tmp_y = mouse_y;
    // }
    al_flip_display();
    draw_game_mode_scene();
    return GAME_CONTINUE;
}
int GameWindow::lobby_event()
{
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return GAME_EXIT;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_M:
            mute = !mute;
            if (mute)
                al_stop_sample_instance(backgroundSound);
            else
                al_play_sample_instance(backgroundSound);
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {

        if (event.mouse.button == MOUSE_LEFT && mouse_x >= 30 && mouse_x <= 555 && mouse_y >= 377 && mouse_y <= 711)
        {
            if (game_mode == STORY)
                draw_msg_box(starting_bet[main_user.story_level - 1]);
            else
                draw_msg_box(100000);
            do
            {
                al_wait_for_event(event_queue, &event);
                if (event.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    {
                        cout << "hi" << endl;
                        judge_next_window[4] = true;
                        return instruction;
                    }
                    else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        draw_lobby_scene();
                        return instruction;
                    }
                }
            } while (event.type != ALLEGRO_EVENT_KEY_DOWN);
        }
        else if (event.mouse.button == MOUSE_LEFT && mouse_x >= 454 && mouse_x <= 1166 && mouse_y >= 832 && mouse_y <= 1197)
        {
            draw_wheel_msg_box();
            do
            {
                al_wait_for_event(event_queue, &event);
                if (event.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    {
                        judge_next_window[5] = true;
                        return instruction;
                    }
                    else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        draw_lobby_scene();
                        return instruction;
                    }
                }
            } while (event.type != ALLEGRO_EVENT_KEY_DOWN);
        }
        else if (event.mouse.button == MOUSE_LEFT && mouse_x >= 1028 && mouse_x <= 1557 && mouse_y >= 372 && mouse_y <= 695)
        {
            if (game_mode == STORY)
                draw_msg_box(starting_bet[main_user.story_level - 1]);
            else
                draw_msg_box(100000);
            do
            {
                al_wait_for_event(event_queue, &event);
                if (event.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    {
                        judge_next_window[6] = true;
                        return instruction;
                    }
                    else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        draw_lobby_scene();
                        return instruction;
                    }
                }
            } while (event.type != ALLEGRO_EVENT_KEY_DOWN);
        }
    }
    // if (tmp_x != mouse_x && tmp_y != mouse_y)
    // {
    //     std::cout << "mouse move to (" << mouse_x << ", " << mouse_y << ")\n";
    //     tmp_x = mouse_x;
    //     tmp_y = mouse_y;
    // }
    al_flip_display();
    draw_lobby_scene();
    return instruction;
}
int GameWindow::Texas_event()
{
    int instruction = GAME_CONTINUE;
    al_wait_for_event(event_queue, &event);
    if (game_mode == SINGLE)
    {
        player sg_player("Visitor", "1111", 100000, -1, 1);
        sg_player.is_main_user = true;
        vector<player> player_list;
        player_list.push_back(sg_player);
        create_game(player_list, starting_bet[main_user.story_level - 1], 4, 1, 100000);
    }
    else if (game_mode == MULTI)
    {
    }
    else if (game_mode == STORY)
    {
        vector<player> player_list;
        player_list.push_back(main_user);
        create_game(player_list, starting_bet[0], 4, main_user.story_level, main_user.budget);
    }
    judge_next_window[7] = true;
    // al_flip_display();
    // draw_texas_table(CHECK);
    return instruction;
}

int GameWindow::Roulette_event()
{
    int instruction = GAME_CONTINUE;
    if (game_mode == SINGLE)
    {
        player sg_player("Visitor", "1111", 100000, -1, 1);
        sg_player.is_main_user = true;
        vector<player> player_list;
        player_list.push_back(sg_player);
        create_roulette_game(player_list);
    }
    else if (game_mode == MULTI)
    {
    }
    else if (game_mode == STORY)
    {
    }
    judge_next_window[7] = true;
    return instruction;
}
int GameWindow::Blackjack_event()
{
}
void GameWindow::draw_running_map()
{
    al_draw_bitmap(background, 0, 0, 0);
    // al_flip_display();
}

void GameWindow::draw_log_in_scene()
{
    al_draw_bitmap(vegas, 0, 0, 0);
    al_draw_bitmap(logInMsg, 50, window_height / 2, 0);
    // al_flip_display();
}

void GameWindow::draw_game_mode_scene()
{
    al_draw_bitmap(gameMode, 0, 0, 0);
}

void GameWindow::draw_lobby_scene()
{
    al_draw_bitmap(lobbyBg, 0, 0, 0);
}

void GameWindow::draw_msg_box(int money)
{
    al_draw_bitmap(msgBox, window_width / 2 - 290, window_height / 2 - 70, 0);
    al_draw_textf(Large_font, al_map_rgb(255, 255, 255), window_width / 2 - 250, window_height / 2 - 50, 0, "Starting bet is %d", money);
    al_draw_textf(Large_font, al_map_rgb(255, 255, 255), window_width / 2 - 250, window_height / 2, 0, "Press Enter To Start the game");
    al_draw_textf(Large_font, al_map_rgb(255, 255, 255), window_width / 2 - 250, window_height / 2 + 50, 0, "Press Esc To return");
    al_flip_display();
}

void GameWindow::draw_wheel_msg_box()
{
    al_draw_bitmap(msgBox, window_width / 2 - 290, window_height / 2 - 70, 0);
    al_draw_textf(Large_font, al_map_rgb(255, 255, 255), window_width / 2 - 250, window_height / 2 - 20, 0, "Press Enter To Start the game");
    al_draw_textf(Large_font, al_map_rgb(255, 255, 255), window_width / 2 - 250, window_height / 2 + 30, 0, "Press Esc To return");
    al_flip_display();
}

void GameWindow::draw_texas_table(int status)
{
    if (status == CALL)
        al_draw_bitmap(texas_table_call, 0, 0, 0);
    else if (status == CHECK)
        al_draw_bitmap(texas_table_check, 0, 0, 0);
    else if (status == ALL_IN)
        al_draw_bitmap(texas_table_all_in, 0, 0, 0);
    al_draw_bitmap(user_name_p, 0, 0, 0);
    al_draw_bitmap(budget_p, 10, 170, 0);
    al_draw_textf(XL_font, al_map_rgb(255, 255, 255), 200, 30, ALLEGRO_ALIGN_CENTRE, "%s", userName.c_str());
    al_draw_textf(XL_font, al_map_rgb(0, 0, 0), 105, 195, 0, "%d", main_user.budget);
}

void GameWindow::draw_roulette_table()
{
    al_draw_bitmap(roulette_table, 0, 0, 0);
    al_draw_rotated_bitmap(roulette, 600, 598, -50, window_height + 100, 0, 0);
}