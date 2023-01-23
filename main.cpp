#include "grafica.h"

int main(int argc, char **argv) {
    Grafica *game = nullptr;
    input user_in;

    do {
        delete game;
        game = new Grafica(600);
        game->res_menu(user_in);
        delete game;
        switch (user_in) {
            case R_600:
                game = new Grafica(600);
                break;
            case R_800:
                game = new Grafica(800);
                break;
            case R_1000:
                game = new Grafica(1000);
                break;
                //case R_FULL: game = new Grafica(); break;
            default:
                break;
        }
        do {
            game->main_menu(user_in);
            if (user_in == PLAY_GAME) {
                do {
                    game->level_menu(user_in);
                    if (user_in == MAIN_MENU) break;
                    switch (user_in) {
                        case MAP_1:
                            game->start_game("/Users/salvatore/Google Drive/Unical/Corsi/C++/Progetto PO/map1.txt", user_in);
                            break;
                        case MAP_2:
                            game->start_game("/Users/salvatore/Google Drive/Unical/Corsi/C++/Progetto PO/map2.txt", user_in);
                            break;
                        case MAP_3:
                            game->start_game("map3.txt", user_in);
                            break;
                        case MAP_4:
                            game->start_game("map4.txt", user_in);
                            break;
                        case MAP_5:
                            game->start_game("map5.txt", user_in);
                            break;
                        default:
                            break;
                    }
                    if (user_in == L_COMPLETE) game->end_game_menu(user_in);
                } while (user_in == PLAY_AGAIN);
            } else if (user_in == QUIT_GAME) return 0;
            else break;
        } while (user_in == MAIN_MENU);
    } while (user_in == CHANGE_RES);
    delete game;
    return 0;
}