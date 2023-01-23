#if !defined(ENUMS_H)
#define ENUMS_H

enum colore {
    C_NULLO = 0,
    ROSSO,
    GIALLO,
    BLU,
    VERDE,
    ARANCIONE,
    CIANO
};

enum tipo { 
    T_NULLO = 0,
    PALLA,
    SEMI_ORIZZONTALE_SIN,
    SEMI_ORIZZONTALE_DES,
    ORIZZONTALE,
    SEMI_VERTICALE_SU,
    SEMI_VERTICALE_GIU,
    VERTICALE,
    CURVA_DES_GIU,
    CURVA_DES_SU,
    CURVA_SIN_GIU,
    CURVA_SIN_SU
};

enum input {
    PLAY_GAME = 0,
    CHANGE_RES,
    MAP_1,
    MAP_2,
    MAP_3,
    MAP_4,
    MAP_5,
    QUIT_GAME,
    PLAY_AGAIN,
    MAIN_MENU,
    R_600,
    R_800,
    R_1000,
    R_FULL,
    L_COMPLETE
};

#endif // ENUMS_H