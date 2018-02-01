#ifndef STAT_H
#define STAT_H

/*This is a macro for defining how much danger supposes one star.*/
#define DPERSTAR 1000
/*INI_MONEY refers to with how much money you start in the game.*/
#define INI_MONEY 2100
#define INI_CHAOS 0

#include <stdio.h>

typedef struct _Stat Stat;

/*New stat in which we will be adding the info about every new game.*/
Stat* stat_ini ();

/*Once we end the game we have to destroy the memory allocated from the stat struct.*/
void stat_destroy(Stat* stat);

/*Now there are functions about adding or setting every stat except the stars.*/
Stat* stat_set_motivation (Stat* stat, int motivation);

Stat* stat_add_motivation (Stat* stat, int motivation);

int stat_get_motivation (Stat* stat);

Stat* stat_set_chaos (Stat* stat, int chaos);

Stat* stat_add_chaos (Stat* stat, int chaos);

int stat_get_chaos (Stat* stat);

Stat* stat_set_danger (Stat* stat, int danger);

Stat* stat_add_danger (Stat* stat, int danger);

int stat_get_danger (Stat* stat);

Stat* stat_set_money (Stat* stat, int money);

Stat* stat_add_money (Stat* stat, int money);

int stat_get_money (Stat* stat);

Stat* stat_add_nodanger (Stat* stat, int nodanger);

int stat_get_nodanger (Stat* stat);

/*In stat_stars we check the danger level and set the stars which represent that level of danger.*/
Stat* stat_stars (Stat* stat);

int stat_get_stars (Stat* stat);

#endif
