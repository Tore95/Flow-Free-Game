#if !defined(CELLA_H)
#define CELLA_H

#include "enums.h"

using namespace std;

class cella {

private:
    colore col;
    tipo tp;
    unsigned nodo;


public:
    cella();
    cella(colore c, tipo t);
    cella(colore c, tipo t, unsigned i);
    unsigned getNodo();
    void setNodo(unsigned i);
    colore getColore();
    tipo getTipo();
    void setTipo(tipo t);
    bool operator==(cella &c) { return nodo == c.nodo; }
};

cella::cella(): col(C_NULLO), tp(T_NULLO), nodo(0) {}

cella::cella(colore c, tipo t): col(c), tp(t), nodo(0) {}

cella::cella(colore c, tipo t, unsigned i) : col(c), tp(t), nodo(i) {}

unsigned cella::getNodo() { return nodo; }

void cella::setNodo(unsigned i) { nodo = i; }

colore cella::getColore() { return col; }

tipo cella::getTipo() { return tp; }

void cella::setTipo(tipo t) {tp = t;}

#endif // CELLA_H
