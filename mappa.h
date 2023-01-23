
#if !defined(MAPPA_H)
#define MAPPA_H

#include "Grafo.h"
#include "cella.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class mappa {
private:
    cella **matrice;
    size_t lung;
    Grafo *g;
    int numero_colori;
    bool colori_presenti[7] = {false, false, false, false, false, false, false};
    vector<unsigned> nodi_palline[7];
    vector<cella> stati_precedenti;

public:
    mappa(const char *source);

    cella **getMappa();

    cella getCella(int x, int y);

    cella getCella(int n_nodo);

    void aggiungi(int prec_nodo, cella new_cella);

    void rimuovi(int curr_nodo);

    int getLung() { return lung; }

    int getNumeroColori() { return numero_colori; }

    bool flusso(colore c);

    bool flusso(int i);

    void aggiorna_cella(cella new_cella);

    int flussi_completi();

    cella getStatoPrecedente(int n_nodo);

    cella genera_cella(int n_nodo);

    ~mappa();
};

mappa::mappa(const char *source) {
    fstream file_mappa(source);
    if (file_mappa.is_open()) {
        string linea;
        stringstream tmp;
        getline(file_mappa, linea);
        tmp << linea;
        tmp >> lung;
        tmp >> numero_colori;
        g = new Grafo(lung * lung);
        matrice = new cella *[lung];
        for (size_t i = 0; i < lung; i++) {
            matrice[i] = new cella[lung];
        }
        int i = 0;
        int nodo = 1;
        while (getline(file_mappa, linea)) {
            int j = 0;
            for (char c: linea) {
                switch (c) {
                    case '1':
                        matrice[i][j] = cella(ROSSO, PALLA);
                        nodi_palline[ROSSO].push_back(nodo);
                        break;
                    case '2':
                        matrice[i][j] = cella(GIALLO, PALLA);
                        nodi_palline[GIALLO].push_back(nodo);
                        break;
                    case '3':
                        matrice[i][j] = cella(BLU, PALLA);
                        nodi_palline[BLU].push_back(nodo);
                        break;
                    case '4':
                        matrice[i][j] = cella(VERDE, PALLA);
                        nodi_palline[VERDE].push_back(nodo);
                        break;
                    case '5':
                        matrice[i][j] = cella(ARANCIONE, PALLA);
                        nodi_palline[ARANCIONE].push_back(nodo);
                        break;
                    case '6':
                        matrice[i][j] = cella(CIANO, PALLA);
                        nodi_palline[CIANO].push_back(nodo);
                        break;
                    default:
                        break;
                }
                matrice[i][j].setNodo(nodo);
                nodo++;
                j++;
            }
            i++;
        }
    }
    file_mappa.close();
    for (int i = 0; i < 7; i++) {
        if (!nodi_palline[i].empty()) colori_presenti[i] = true;
    }
    stati_precedenti.resize(g->n() + 1);
}

void mappa::aggiorna_cella(cella new_cella) {
    bool fin = false;
    for (size_t i = 0; i < lung && !fin; i++) {
        for (size_t j = 0; j < lung && !fin; j++) {
            if (matrice[i][j] == new_cella) {
                if (matrice[i][j].getTipo() == SEMI_ORIZZONTALE_DES ||
                    matrice[i][j].getTipo() == SEMI_ORIZZONTALE_SIN || matrice[i][j].getTipo() == SEMI_VERTICALE_GIU ||
                    matrice[i][j].getTipo() == SEMI_VERTICALE_SU)
                    stati_precedenti[new_cella.getNodo()] = matrice[i][j];
                matrice[i][j] = new_cella;
                fin = true;
            }
        }
    }
}

cella mappa::genera_cella(int n_nodo) {
    for (size_t i = 1; i <= g->n(); i++) {
        if (g->operator()(n_nodo, i)) {
            int x_prec = i % lung;
            int y_prec = i / lung;
            int x_curr = n_nodo % lung;
            int y_curr = n_nodo / lung;
            if (x_curr == (x_prec + 1) && y_curr == y_prec) {
                return cella(matrice[y_prec][x_prec].getColore(), SEMI_ORIZZONTALE_DES, i);
            } else if (x_curr == (x_prec - 1) && y_curr == y_prec) {
                return cella(matrice[y_prec][x_prec].getColore(), SEMI_ORIZZONTALE_SIN, i);
            } else if (x_curr == x_prec && y_curr == (y_prec + 1)) {
                return cella(matrice[y_prec][x_prec].getColore(), SEMI_VERTICALE_SU, i);
            } else if (x_curr == x_prec && y_curr == (y_prec - 1)) {
                return cella(matrice[y_prec][x_prec].getColore(), SEMI_VERTICALE_GIU, i);
            }
        }
    }
    return cella(C_NULLO, T_NULLO, 0);
}

cella mappa::getStatoPrecedente(int n_nodo) {
    return stati_precedenti[n_nodo];
}

bool mappa::flusso(colore c) {
    assert(!nodi_palline[c].empty());
    return g->percorso(nodi_palline[c][0], nodi_palline[c][1]);
}

bool mappa::flusso(int i) {
    assert(!nodi_palline[i].empty());
    return g->percorso(nodi_palline[i][0], nodi_palline[i][1]);
}

int mappa::flussi_completi() {
    int cont = 0;
    for (int i = 0; i < 7; i++) {
        if (colori_presenti[i] && flusso(i)) cont++;
    }
    return cont;
}

cella **mappa::getMappa() { return matrice; }

//  aggiungere arco tra nodo precedente ed attuale
void mappa::aggiungi(int prec_nodo, cella new_cella) {
    g->operator()(prec_nodo, new_cella.getNodo(), true);
    g->operator()(new_cella.getNodo(), prec_nodo, true);
    aggiorna_cella(new_cella);
}

cella mappa::getCella(int x, int y) { return matrice[y][x]; }

cella mappa::getCella(int n_nodo) {
    for (size_t i = 0; i < lung; i++) {
        for (size_t j = 0; j < lung; j++) {
            if (matrice[i][j].getNodo() == n_nodo) {
                return matrice[i][j];
            }
        }
    }
    return cella(C_NULLO, T_NULLO, 0);
}

void mappa::rimuovi(int curr_nodo) {
    g->rimuovi_archi(curr_nodo);
    aggiorna_cella(cella(C_NULLO, T_NULLO, curr_nodo));


}

mappa::~mappa() {
    for (size_t i = 0; i < lung; i++) {
        delete[] matrice[i];
    }
    delete[] matrice;
    delete g;
}


#endif // MAPPA_H
