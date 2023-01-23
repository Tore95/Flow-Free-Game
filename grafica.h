#ifndef GRAFICA_H
#define GRAFICA_H

#include "mappa.h"
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <limits>
#include <list>

using namespace std;


struct bottone {
    int x, y, resolution, x1, x2, y1, y2, fps, raggio_max;

    bottone(int _resolution, int _x, double _y) {
        resolution = _resolution;
        fps = 1;
        x = _x;
        y = _y;
        auto px = static_cast<int>(resolution * 0.2);
        x1 = px;
        x2 = resolution - px;
        y1 = static_cast<int>(y - (resolution * 0.04));
        y2 = static_cast<int>(y + (resolution * 0.1));
        raggio_max = static_cast<int>(resolution * 0.70);
    }

    bool draw(int mouse_x, int mouse_y, bool pressed) {
        if (!collide(mouse_x, mouse_y) || !pressed) fps = 0;
        float alpha = 0.5f;
        if (collide(mouse_x, mouse_y)) {
            al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba_f(static_cast<float>(0.75 * alpha),
                                                                   static_cast<float>(0.75 * alpha),
                                                                   static_cast<float>(0.75 * alpha),
                                                                   alpha));
            if (pressed && fps < 30) {
                fps++;
                int raggio_curr = (raggio_max / 30) * fps;
                al_draw_filled_circle(mouse_x, mouse_y, raggio_curr, al_map_rgba_f(static_cast<float>(0.45 * alpha),
                                                                                   static_cast<float>(0.45 * alpha),
                                                                                   static_cast<float>(0.45 * alpha),
                                                                                   alpha));
                al_draw_filled_rectangle(0, 0, resolution, y1, al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(0, y1, x1, y2, al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(0, y2, static_cast<float>(resolution + (resolution * 0.1)),
                                         static_cast<float>(resolution + (resolution * 0.1)),
                                         al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(x2, y1, resolution, y2, al_map_rgb(255, 255, 255));
            } else if (pressed && fps >= 30) {
                al_draw_filled_circle(mouse_x, mouse_y, raggio_max, al_map_rgba_f(static_cast<float>(0.45 * alpha),
                                                                                  static_cast<float>(0.45 * alpha),
                                                                                  static_cast<float>(0.45 * alpha),
                                                                                  alpha));
                al_draw_filled_rectangle(0, 0, resolution, y1, al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(0, y1, x1, y2, al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(0, y2, static_cast<float>(resolution + (resolution * 0.1)),
                                         static_cast<float>(resolution + (resolution * 0.1)),
                                         al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(x2, y1, resolution, y2, al_map_rgb(255, 255, 255));
                return true;
            }
        }
        return false;
    }

    bool collide(int mouse_x, int mouse_y) {
        return mouse_x > x1 && mouse_x < x2 && mouse_y > y1 && mouse_y < y2;
    }
};


class Grafica {
private:
    int n_celle;
    int prec_mouse_x, prec_mouse_y;
    int curr_mouse_x, curr_mouse_y;
    int res_monitor_x;
    int res_monitor_y;
    int l_cella;
    int nodo_partenza[7];
    unsigned curr_nodo;
    unsigned prec_nodo;
    bool mouse_premuto;
    ALLEGRO_COLOR colori[7];
    ALLEGRO_COLOR colori_alpha[7];
    ALLEGRO_FONT *text_font;
    ALLEGRO_FONT *title_font;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    colore curr_colore;
    colore prec_colore;
    colore colore_premuto;
    list<unsigned> celle_aggiunte[7];
    mappa *m;
    tipo curr_tipo;
    tipo prec_tipo;

    void draw_game(cella **matrice) {
        al_clear_to_color(al_map_rgb(70, 70, 70));
        for (int i = 0; i < res_monitor_x; i += l_cella) {
            al_draw_line(0, i, res_monitor_x, i, al_map_rgb(230, 230, 230), 1.0);
            al_draw_line(i, 0, i, res_monitor_x, al_map_rgb(230, 230, 230), 1.0);
        }

        for (int i = 0; i < n_celle; i++) {
            for (int j = 0; j < n_celle; j++) {
                int y_pos = (l_cella / 2) + (i * l_cella);
                int x_pos = (l_cella / 2) + (j * l_cella);
                ALLEGRO_COLOR current_color;
                current_color = colori[m->getCella(j, i).getColore()];
                switch (matrice[i][j].getTipo()) {
                    case PALLA:
                        al_draw_filled_circle(x_pos, y_pos, static_cast<float>(l_cella * 0.40), current_color);
                        break;
                    case SEMI_ORIZZONTALE_SIN:
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.75)),
                                                 static_cast<float>(y_pos - (l_cella * 0.25)), x_pos,
                                                 static_cast<float>(y_pos + (l_cella * 0.25)), current_color);
                        al_draw_filled_circle(x_pos, y_pos, static_cast<float>(l_cella * 0.30), current_color);
                        break;
                    case SEMI_ORIZZONTALE_DES:
                        al_draw_filled_rectangle(static_cast<float>(x_pos + (l_cella * 0.75)),
                                                 static_cast<float>(y_pos - (l_cella * 0.25)), x_pos,
                                                 static_cast<float>(y_pos + (l_cella * 0.25)), current_color);
                        al_draw_filled_circle(x_pos, y_pos, static_cast<float>(l_cella * 0.30), current_color);
                        break;
                    case ORIZZONTALE:
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.75)),
                                                 static_cast<float>(y_pos - (l_cella * 0.25)),
                                                 static_cast<float>(x_pos + (l_cella * 0.75)),
                                                 static_cast<float>(y_pos + (l_cella * 0.25)), current_color);
                        break;
                    case SEMI_VERTICALE_SU:
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                 static_cast<float>(y_pos - (l_cella * 0.75)),
                                                 static_cast<float>(x_pos + (l_cella * 0.25)), y_pos, current_color);
                        al_draw_filled_circle(x_pos, y_pos, static_cast<float>(l_cella * 0.30), current_color);
                        break;
                    case SEMI_VERTICALE_GIU:
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                 static_cast<float>(y_pos + (l_cella * 0.75)),
                                                 static_cast<float>(x_pos + (l_cella * 0.25)), y_pos, current_color);
                        al_draw_filled_circle(x_pos, y_pos, static_cast<float>(l_cella * 0.30), current_color);
                        break;
                    case VERTICALE:
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                 static_cast<float>(y_pos - (l_cella * 0.75)),
                                                 static_cast<float>(x_pos + (l_cella * 0.25)),
                                                 static_cast<float>(y_pos + (l_cella * 0.75)), current_color);
                        break;
                    case CURVA_DES_GIU:
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.75)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.75)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                 static_cast<float>(y_pos + (l_cella * 0.25)),
                                                 static_cast<float>(x_pos - (l_cella * 0.375)),
                                                 static_cast<float>(y_pos + (l_cella * 0.375)), current_color);
                        al_draw_filled_circle(static_cast<float>(x_pos - (l_cella * 0.35)),
                                              static_cast<float>(y_pos + (l_cella * 0.35)),
                                              static_cast<float>(l_cella * 0.1), al_map_rgb(70, 70, 70));
                        break;
                    case CURVA_DES_SU:
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.75)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.75)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                 static_cast<float>(y_pos - (l_cella * 0.25)),
                                                 static_cast<float>(x_pos - (l_cella * 0.375)),
                                                 static_cast<float>(y_pos - (l_cella * 0.375)), current_color);
                        al_draw_filled_circle(static_cast<float>(x_pos - (l_cella * 0.35)),
                                              static_cast<float>(y_pos - (l_cella * 0.35)),
                                              static_cast<float>(l_cella * 0.1), al_map_rgb(70, 70, 70));
                        break;
                    case CURVA_SIN_GIU:
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.75)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.75)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rectangle(static_cast<float>(x_pos + (l_cella * 0.25)),
                                                 static_cast<float>(y_pos + (l_cella * 0.25)),
                                                 static_cast<float>(x_pos + (l_cella * 0.375)),
                                                 static_cast<float>(y_pos + (l_cella * 0.375)), current_color);
                        al_draw_filled_circle(static_cast<float>(x_pos + (l_cella * 0.35)),
                                              static_cast<float>(y_pos + (l_cella * 0.35)),
                                              static_cast<float>(l_cella * 0.1), al_map_rgb(70, 70, 70));
                        break;
                    case CURVA_SIN_SU:
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.25)),
                                                         static_cast<float>(x_pos + (l_cella * 0.75)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rounded_rectangle(static_cast<float>(x_pos - (l_cella * 0.25)),
                                                         static_cast<float>(y_pos - (l_cella * 0.75)),
                                                         static_cast<float>(x_pos + (l_cella * 0.25)),
                                                         static_cast<float>(y_pos + (l_cella * 0.25)),
                                                         static_cast<float>(l_cella * 0.125),
                                                         static_cast<float>(l_cella * 0.125), current_color);
                        al_draw_filled_rectangle(static_cast<float>(x_pos + (l_cella * 0.25)),
                                                 static_cast<float>(y_pos - (l_cella * 0.25)),
                                                 static_cast<float>(x_pos + (l_cella * 0.375)),
                                                 static_cast<float>(y_pos - (l_cella * 0.375)), current_color);
                        al_draw_filled_circle(static_cast<float>(x_pos + (l_cella * 0.35)),
                                              static_cast<float>(y_pos - (l_cella * 0.35)),
                                              static_cast<float>(l_cella * 0.1), al_map_rgb(70, 70, 70));
                        break;

                    default:
                        break;
                    case T_NULLO:
                        break;
                }
            }
        }

        string punteggio =
                "flussi completati: " + to_string(m->flussi_completi()) + '/' + to_string(m->getNumeroColori());
        al_draw_text(text_font, al_map_rgb(255, 70, 70), static_cast<float>(res_monitor_x * 0.01),
                     static_cast<float>(res_monitor_x + (res_monitor_x * 0.02)), 0, punteggio.c_str());
    }

    void insert_cells(bool &mouse_premuto) {
        if (curr_tipo != T_NULLO && curr_tipo != PALLA && curr_colore != colore_premuto) {

            auto trovato = find(celle_aggiunte[curr_colore].begin(), celle_aggiunte[curr_colore].end(), curr_nodo);
            auto fine = celle_aggiunte[curr_colore].end();

            for (auto iter = trovato; iter != fine; iter++) {
                m->rimuovi(*iter);
            }
            celle_aggiunte[curr_colore].erase(trovato, fine);

            m->aggiorna_cella(m->getStatoPrecedente(celle_aggiunte[curr_colore].back()));
        }
        if (curr_tipo == PALLA && curr_colore == colore_premuto && prec_tipo == T_NULLO) {
            for (unsigned int &i: celle_aggiunte[colore_premuto]) {
                m->rimuovi(i);
            }
            celle_aggiunte[colore_premuto].clear();
        }
            //  Risolvere il bug del ciclo continuo
        else if (curr_colore == colore_premuto &&
                 !(curr_tipo == SEMI_ORIZZONTALE_DES || curr_tipo == SEMI_ORIZZONTALE_SIN ||
                   curr_tipo == SEMI_VERTICALE_GIU || curr_tipo == SEMI_VERTICALE_SU || curr_tipo == PALLA)) {
            m->rimuovi(celle_aggiunte[colore_premuto].back());
            celle_aggiunte[colore_premuto].pop_back();
            m->aggiorna_cella(m->getStatoPrecedente(celle_aggiunte[colore_premuto].back()));


        }
            //  entrare nella palla da cui sei partito FATTO
        else if (curr_tipo == PALLA && curr_colore != colore_premuto) {
            mouse_premuto = false;
            prec_tipo = T_NULLO;
            prec_colore = C_NULLO;
            if (celle_aggiunte[colore_premuto].empty()) nodo_partenza[colore_premuto] = 0;
        } else {
            if (curr_nodo == nodo_partenza[colore_premuto]) {
                mouse_premuto = false;
                prec_tipo = T_NULLO;
                prec_colore = C_NULLO;
                if (celle_aggiunte[colore_premuto].empty()) nodo_partenza[colore_premuto] = 0;
            }

                //  non passare sul proprio flusso FATTO
            else {
                if (prec_mouse_x == curr_mouse_x - 1 && prec_mouse_y == curr_mouse_y && prec_colore == colore_premuto) {

                    if (prec_tipo == SEMI_ORIZZONTALE_SIN)
                        m->aggiorna_cella(cella(colore_premuto, ORIZZONTALE, prec_nodo));
                    else if (prec_tipo == SEMI_VERTICALE_SU)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_SIN_SU, prec_nodo));
                    else if (prec_tipo == SEMI_VERTICALE_GIU)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_SIN_GIU, prec_nodo));
                    else if (prec_tipo == PALLA) nodo_partenza[colore_premuto] = prec_nodo;

                    if (curr_tipo != PALLA) {
                        celle_aggiunte[colore_premuto].push_back(curr_nodo);
                        m->aggiungi(prec_nodo, cella(colore_premuto, SEMI_ORIZZONTALE_SIN, curr_nodo));
                    } else if (!celle_aggiunte[colore_premuto].empty() && !m->flusso(colore_premuto)) {
                        m->aggiungi(prec_nodo, cella(colore_premuto, PALLA, curr_nodo));
                    }

                } else if (prec_mouse_x == curr_mouse_x && prec_mouse_y == curr_mouse_y - 1 &&
                           prec_colore == colore_premuto) {

                    if (prec_tipo == SEMI_VERTICALE_SU) m->aggiorna_cella(cella(colore_premuto, VERTICALE, prec_nodo));
                    else if (prec_tipo == SEMI_ORIZZONTALE_SIN)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_DES_GIU, prec_nodo));
                    else if (prec_tipo == SEMI_ORIZZONTALE_DES)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_SIN_GIU, prec_nodo));
                    else if (prec_tipo == PALLA) nodo_partenza[colore_premuto] = prec_nodo;

                    if (curr_tipo != PALLA) {
                        celle_aggiunte[colore_premuto].push_back(curr_nodo);
                        m->aggiungi(prec_nodo, cella(colore_premuto, SEMI_VERTICALE_SU, curr_nodo));
                    } else if (!celle_aggiunte[colore_premuto].empty() && !m->flusso(colore_premuto)) {
                        m->aggiungi(prec_nodo, cella(colore_premuto, PALLA, curr_nodo));
                    }

                } else if (prec_mouse_x == curr_mouse_x + 1 && prec_mouse_y == curr_mouse_y &&
                           prec_colore == colore_premuto) {

                    if (prec_tipo == SEMI_ORIZZONTALE_DES)
                        m->aggiorna_cella(cella(colore_premuto, ORIZZONTALE, prec_nodo));
                    else if (prec_tipo == SEMI_VERTICALE_SU)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_DES_SU, prec_nodo));
                    else if (prec_tipo == SEMI_VERTICALE_GIU)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_DES_GIU, prec_nodo));
                    else if (prec_tipo == PALLA) nodo_partenza[colore_premuto] = prec_nodo;

                    if (curr_tipo != PALLA) {
                        celle_aggiunte[colore_premuto].push_back(curr_nodo);
                        m->aggiungi(prec_nodo, cella(colore_premuto, SEMI_ORIZZONTALE_DES, curr_nodo));
                    } else if (!celle_aggiunte[colore_premuto].empty() && !m->flusso(colore_premuto)) {
                        m->aggiungi(prec_nodo, cella(colore_premuto, PALLA, curr_nodo));
                    }

                } else if (prec_mouse_x == curr_mouse_x && prec_mouse_y == curr_mouse_y + 1 &&
                           prec_colore == colore_premuto) {

                    if (prec_tipo == SEMI_VERTICALE_GIU) m->aggiorna_cella(cella(colore_premuto, VERTICALE, prec_nodo));
                    else if (prec_tipo == SEMI_ORIZZONTALE_SIN)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_DES_SU, prec_nodo));
                    else if (prec_tipo == SEMI_ORIZZONTALE_DES)
                        m->aggiorna_cella(cella(colore_premuto, CURVA_SIN_SU, prec_nodo));
                    else if (prec_tipo == PALLA) nodo_partenza[colore_premuto] = prec_nodo;

                    if (curr_tipo != PALLA) {
                        celle_aggiunte[colore_premuto].push_back(curr_nodo);
                        m->aggiungi(prec_nodo, cella(colore_premuto, SEMI_VERTICALE_GIU, curr_nodo));
                    } else if (!celle_aggiunte[colore_premuto].empty() && !m->flusso(colore_premuto)) {
                        m->aggiungi(prec_nodo, cella(colore_premuto, PALLA, curr_nodo));
                    }
                }
            }
        }
    }

public:
    //  Creare grafica full screen

    Grafica(int res_x) : res_monitor_x(res_x), res_monitor_y(res_x + (res_x / 10)), mouse_premuto(false) {
        m = nullptr;

        assert(al_init());

        assert(al_init_font_addon());

        assert(al_init_ttf_addon());

        colori[ROSSO] = al_map_rgb_f(1.0, 0.0, 0.0);
        colori[GIALLO] = al_map_rgb_f(1.0, 1.0, 0);
        colori[BLU] = al_map_rgb_f(0.0, 0.0, 1.0);
        colori[VERDE] = al_map_rgb_f(0.0, 0.8, 0.0);
        colori[ARANCIONE] = al_map_rgb_f(1.0, 0.6, 0.0);
        colori[CIANO] = al_map_rgb_f(0.0, 1.0, 1.0);

        float alpha = 0.25f;

        colori_alpha[ROSSO] = al_map_rgba_f(static_cast<float>(1.0 * alpha), static_cast<float>(0.0 * alpha),
                                            static_cast<float>(0.0 * alpha), alpha);
        colori_alpha[GIALLO] = al_map_rgba_f(static_cast<float>(1.0 * alpha), static_cast<float>(1.0 * alpha),
                                             static_cast<float>(0.0 * alpha), alpha);
        colori_alpha[BLU] = al_map_rgba_f(static_cast<float>(0.0 * alpha), static_cast<float>(0.0 * alpha),
                                          static_cast<float>(1.0 * alpha), alpha);
        colori_alpha[VERDE] = al_map_rgba_f(static_cast<float>(0.0 * alpha), static_cast<float>(0.8 * alpha),
                                            static_cast<float>(0.0 * alpha), alpha);
        colori_alpha[ARANCIONE] = al_map_rgba_f(static_cast<float>(1.0 * alpha), static_cast<float>(0.6 * alpha),
                                                static_cast<float>(0.0 * alpha), alpha);
        colori_alpha[CIANO] = al_map_rgba_f(static_cast<float>(0.0 * alpha), static_cast<float>(1.0 * alpha),
                                            static_cast<float>(1.0 * alpha), alpha);

        text_font = al_load_font("/Users/salvatore/Google Drive/Unical/Corsi/C++/Progetto PO/Roboto-Medium.ttf", static_cast<int>(res_x * 0.05), 0);
        title_font = al_load_font("/Users/salvatore/Google Drive/Unical/Corsi/C++/Progetto PO/Roboto-Medium.ttf", static_cast<int>(res_x * 0.075), 0);
        assert(title_font);

        display = al_create_display(res_monitor_x, res_monitor_y);

        assert(display);

        al_set_window_title(display, "Flow Free");

        assert(al_install_keyboard());

        assert(al_install_mouse());

        assert(al_init_primitives_addon());

        event_queue = al_create_event_queue();
        assert(event_queue);

        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_mouse_event_source());
    }

    void res_menu(input &in) {
        bottone a(600, 600 / 2, 200);
        bottone b(600, 600 / 2, 300);
        bottone c(600, 600 / 2, 400);
        bottone d(600, 600 / 2, 500);

        bool run = true;
        bool pressed = false;

        while (run) {
            al_rest(1.0 / 60);

            al_clear_to_color(al_map_rgb(255, 255, 255));

            ALLEGRO_EVENT ev;

            while (!al_is_event_queue_empty(event_queue)) {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    pressed = true;
                } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if (a.collide(ev.mouse.x, ev.mouse.y)) {
                        in = R_600;
                        run = false;
                    } else if (b.collide(ev.mouse.x, ev.mouse.y)) {
                        in = R_800;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = R_1000;
                        run = false;
                    } else if (d.collide(ev.mouse.x, ev.mouse.y)) {
                        in = R_FULL;
                        run = false;
                    }

                    pressed = false;
                }
            }
            a.draw(ev.mouse.x, ev.mouse.y, pressed);
            b.draw(ev.mouse.x, ev.mouse.y, pressed);
            c.draw(ev.mouse.x, ev.mouse.y, pressed);
            d.draw(ev.mouse.x, ev.mouse.y, pressed);

            al_draw_text(title_font, al_map_rgb(70, 70, 70), 600 / 2, 50, ALLEGRO_ALIGN_CENTRE,
                         "Seleziona una Risoluzione:");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), 600 / 2, 200, ALLEGRO_ALIGN_CENTRE, "600 x 600");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), 600 / 2, 300, ALLEGRO_ALIGN_CENTRE, "800 x 800");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), 600 / 2, 400, ALLEGRO_ALIGN_CENTRE, "1000 x 1000");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), 600 / 2, 500, ALLEGRO_ALIGN_CENTRE, "Schermo Intero");

            al_flip_display();
        }
    }

    void main_menu(input &in) {
        bottone a(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.35));
        bottone b(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.55));
        bottone c(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.75));

        bool run = true;
        bool pressed = false;

        while (run) {
            al_rest(1.0 / 60);

            al_clear_to_color(al_map_rgb(255, 255, 255));

            ALLEGRO_EVENT ev;

            while (!al_is_event_queue_empty(event_queue)) {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    pressed = true;
                } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if (a.collide(ev.mouse.x, ev.mouse.y)) {
                        in = PLAY_GAME;
                        run = false;
                    } else if (b.collide(ev.mouse.x, ev.mouse.y)) {
                        in = CHANGE_RES;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = QUIT_GAME;
                        run = false;
                    }

                    pressed = false;
                }
            }
            a.draw(ev.mouse.x, ev.mouse.y, pressed);
            b.draw(ev.mouse.x, ev.mouse.y, pressed);
            c.draw(ev.mouse.x, ev.mouse.y, pressed);

            al_draw_text(title_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.1),
                         ALLEGRO_ALIGN_CENTRE, "Main Menu");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.35),
                         ALLEGRO_ALIGN_CENTRE, "Gioca");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.55),
                         ALLEGRO_ALIGN_CENTRE, "Cambia Risoluzione");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.75),
                         ALLEGRO_ALIGN_CENTRE, "Esci");

            al_flip_display();
        }
    }

    void level_menu(input &in) {
        bottone a(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.21));
        bottone b(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.37));
        bottone c(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.53));
        bottone d(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.69));
        bottone e(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.85));

        bool run = true;
        bool pressed = false;

        while (run) {
            al_rest(1.0 / 60);

            al_clear_to_color(al_map_rgb(255, 255, 255));

            ALLEGRO_EVENT ev;

            while (!al_is_event_queue_empty(event_queue)) {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    pressed = true;
                } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if (a.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAP_1;
                        run = false;
                    } else if (b.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAP_2;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAP_3;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAP_4;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAP_5;
                        run = false;
                    }

                    pressed = false;
                }
            }
            a.draw(ev.mouse.x, ev.mouse.y, pressed);
            b.draw(ev.mouse.x, ev.mouse.y, pressed);
            c.draw(ev.mouse.x, ev.mouse.y, pressed);
            d.draw(ev.mouse.x, ev.mouse.y, pressed);
            e.draw(ev.mouse.x, ev.mouse.y, pressed);

            al_draw_text(title_font, al_map_rgb(70, 70, 70), res_monitor_x / 2,
                         static_cast<float>(res_monitor_y * 0.05), ALLEGRO_ALIGN_CENTRE, "Seleziona il Livello");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.21),
                         ALLEGRO_ALIGN_CENTRE, "Livello: 1");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.37),
                         ALLEGRO_ALIGN_CENTRE, "Livello: 2");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.53),
                         ALLEGRO_ALIGN_CENTRE, "Livello: 3");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.69),
                         ALLEGRO_ALIGN_CENTRE, "Livello: 4");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.85),
                         ALLEGRO_ALIGN_CENTRE, "Livello: 5");

            al_flip_display();
        }
    }

    void end_game_menu(input &in) {
        bottone a(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.35));
        bottone b(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.55));
        bottone c(res_monitor_x, res_monitor_x / 2, static_cast<int>(res_monitor_y * 0.75));

        bool run = true;
        bool pressed = false;

        while (run) {
            al_rest(1.0 / 60);

            al_clear_to_color(al_map_rgb(255, 255, 255));

            ALLEGRO_EVENT ev;

            while (!al_is_event_queue_empty(event_queue)) {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    pressed = true;
                } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if (a.collide(ev.mouse.x, ev.mouse.y)) {
                        in = PLAY_AGAIN;
                        run = false;
                    } else if (b.collide(ev.mouse.x, ev.mouse.y)) {
                        in = MAIN_MENU;
                        run = false;
                    } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                        in = QUIT_GAME;
                        run = false;
                    }

                    pressed = false;
                }
            }
            a.draw(ev.mouse.x, ev.mouse.y, pressed);
            b.draw(ev.mouse.x, ev.mouse.y, pressed);
            c.draw(ev.mouse.x, ev.mouse.y, pressed);

            al_draw_text(title_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.1),
                         ALLEGRO_ALIGN_CENTRE, "Livello Completo!");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.35),
                         ALLEGRO_ALIGN_CENTRE, "Gioca Ancora");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.55),
                         ALLEGRO_ALIGN_CENTRE, "Menù Principale");
            al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2, static_cast<float>(res_monitor_y * 0.75),
                         ALLEGRO_ALIGN_CENTRE, "Esci");

            al_flip_display();
        }
    }

    void start_game(const char *map_source, input &in) {
        m = new mappa(map_source);

        int closer_fps = 0;

        n_celle = m->getLung();

        l_cella = res_monitor_x / n_celle;

        bool run = true;
        bool pause = false;

        while (run) {
            al_rest(1.0 / 60);

            if (!pause) {

                draw_game(m->getMappa());

                ALLEGRO_EVENT ev;

                while (!al_is_event_queue_empty(event_queue)) {
                    al_wait_for_event(event_queue, &ev);
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        pause = true;
                    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !mouse_premuto) {
                        curr_mouse_x = ev.mouse.x / l_cella;
                        curr_mouse_y = ev.mouse.y / l_cella;
                        curr_tipo = m->getCella(curr_mouse_x, curr_mouse_y).getTipo();
                        curr_colore = m->getCella(curr_mouse_x, curr_mouse_y).getColore();
                        curr_nodo = m->getCella(curr_mouse_x, curr_mouse_y).getNodo();
                        if (curr_tipo == PALLA || curr_tipo == SEMI_ORIZZONTALE_DES ||
                            curr_tipo == SEMI_ORIZZONTALE_SIN || curr_tipo == SEMI_VERTICALE_GIU ||
                            curr_tipo == SEMI_VERTICALE_SU) {
                            mouse_premuto = true;
                            colore_premuto = m->getCella(curr_mouse_x, curr_mouse_y).getColore();
                        }
                    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && mouse_premuto) {
                        mouse_premuto = false;
                        prec_tipo = T_NULLO;
                        prec_colore = C_NULLO;
                    }
                }
                if (mouse_premuto) {
                    int x = ev.mouse.x / l_cella;
                    int y = ev.mouse.y / l_cella;
                    if (x != curr_mouse_x || y != curr_mouse_y) {
                        prec_mouse_x = curr_mouse_x;
                        prec_mouse_y = curr_mouse_y;
                        curr_mouse_x = x;
                        curr_mouse_y = y;
                        curr_tipo = m->getCella(curr_mouse_x, curr_mouse_y).getTipo();
                        curr_colore = m->getCella(curr_mouse_x, curr_mouse_y).getColore();
                        curr_nodo = m->getCella(curr_mouse_x, curr_mouse_y).getNodo();
                        prec_tipo = m->getCella(prec_mouse_x, prec_mouse_y).getTipo();
                        prec_colore = m->getCella(prec_mouse_x, prec_mouse_y).getColore();
                        prec_nodo = m->getCella(prec_mouse_x, prec_mouse_y).getNodo();
                    }
                    al_draw_filled_circle(ev.mouse.x, ev.mouse.y, (l_cella * 0.75), colori_alpha[colore_premuto]);
                    insert_cells(mouse_premuto);
                } else if (m->flussi_completi() == m->getNumeroColori()) {
                    in = L_COMPLETE;
                    run = false;
                }
            } else {
                bottone a(res_monitor_x, res_monitor_x / 2, res_monitor_y * 0.35);
                bottone b(res_monitor_x, res_monitor_x / 2, res_monitor_y * 0.55);
                bottone c(res_monitor_x, res_monitor_x / 2, res_monitor_y * 0.75);

                al_clear_to_color(al_map_rgb(255, 255, 255));

                ALLEGRO_EVENT ev;

                while (!al_is_event_queue_empty(event_queue)) {
                    al_wait_for_event(event_queue, &ev);
                    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        mouse_premuto = true;
                    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                        if (a.collide(ev.mouse.x, ev.mouse.y)) {
                            pause = false;
                        } else if (b.collide(ev.mouse.x, ev.mouse.y)) {
                            in = MAIN_MENU;
                            run = false;
                        } else if (c.collide(ev.mouse.x, ev.mouse.y)) {
                            in = QUIT_GAME;
                            run = false;
                        }

                        mouse_premuto = false;
                    }
                }
                a.draw(ev.mouse.x, ev.mouse.y, mouse_premuto);
                b.draw(ev.mouse.x, ev.mouse.y, mouse_premuto);
                c.draw(ev.mouse.x, ev.mouse.y, mouse_premuto);

                al_draw_text(title_font, al_map_rgb(70, 70, 70), res_monitor_x / 2,
                             static_cast<float>(res_monitor_y * 0.1), ALLEGRO_ALIGN_CENTRE, "Pausa:");
                al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2,
                             static_cast<float>(res_monitor_y * 0.35), ALLEGRO_ALIGN_CENTRE, "Riprendi");
                al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2,
                             static_cast<float>(res_monitor_y * 0.55), ALLEGRO_ALIGN_CENTRE, "Menù Principale");
                al_draw_text(text_font, al_map_rgb(70, 70, 70), res_monitor_x / 2,
                             static_cast<float>(res_monitor_y * 0.75), ALLEGRO_ALIGN_CENTRE, "Esci");
            }
            al_flip_display();
        }

        while (closer_fps < 60.0f) {
            al_rest(1.0 / 60);
            closer_fps++;
            float alpha = (1.0f / 60.0f) * (float) closer_fps;
            draw_game(m->getMappa());
            al_draw_filled_rectangle(0, 0, res_monitor_x, res_monitor_y, al_map_rgba_f(static_cast<float>(1.0 * alpha),
                                                                                       static_cast<float>(1.0 * alpha),
                                                                                       static_cast<float>(1.0 * alpha),
                                                                                       alpha));
            al_flip_display();
        }
    }

    ~Grafica() {
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_font(text_font);
        delete m;
    }
};

#endif
