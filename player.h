#ifndef PLAYER_H
#define PLAYER_H
#define INIX 70
#define INIY 20
#define INIROOM 0
#include <stdio.h>

typedef struct _Player Player;

Player* player_ini();

void player_destroy();

Player* player_move();

Player* player_set_room(); /*manolo esto estaba mal, falta grave*/

int player_get_room(Player *c);

int player_get_px();

int player_get_py();

int player_distance(Player *c, int x, int y);

int player_isNear(Player* c, int x, int y);

#endif /* PLAYER_H */
