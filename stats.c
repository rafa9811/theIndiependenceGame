#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stats.h"

/* Stat consists on the info about the five resources in the game. */
struct _Stat {
    int motivation;
    int chaos;
    int danger;
    int stars;
    int money;
    int nodanger;
};

/*New stat in which we will be adding the info about every new game.*/
Stat* stat_ini (){
    Stat* stat;
    stat=(Stat*)malloc(sizeof(Stat));
    if(!stat) {
        return NULL;
    }
    stat->motivation = 0;
    stat->chaos = INI_CHAOS;
    stat->danger = 0;
    stat->stars = 0;
    stat->money = INI_MONEY;
    stat->nodanger = 0;
    return stat;
}

/*Once we end the game we have to destroy the memory allocated from the stat struct.*/
void stat_destroy(Stat* stat){
    if (stat) {
        free(stat);
    }
}

/*Now there are functions about adding or setting every stat excep the stars.*/
Stat* stat_set_motivation (Stat* stat, int motivation){
    if (!stat){
        return NULL;
    }
    stat->motivation=motivation;
    return stat;
}

Stat* stat_add_motivation (Stat* stat, int motivation){
    if (!stat){
        return NULL;
    }
    stat->motivation+=motivation;
    return stat;
}

int stat_get_motivation (Stat* stat){
	if(!stat) return -1;
	return stat->motivation;
}

Stat* stat_set_chaos (Stat* stat, int chaos){
    if (!stat){
        return NULL;
    }
    stat->chaos=chaos;
    return stat;
}

Stat* stat_add_chaos (Stat* stat, int chaos){
    if (!stat){
        return NULL;
    }
    stat->chaos+=chaos;
    if(stat->chaos<0) stat->chaos = 0;
    return stat;
}

int stat_get_chaos (Stat* stat){
	if(!stat) return -1;
	return stat->chaos;
}

Stat* stat_set_danger (Stat* stat, int danger){
    if (!stat){
        return NULL;
    }
    stat->danger=danger;
    return stat;
}

Stat* stat_add_danger (Stat* stat, int danger){
    if (!stat){
        return NULL;
    }
    stat->danger+=danger;
    return stat;
}

int stat_get_danger (Stat* stat){
	if(!stat) return -1;
	return stat->danger;
}


Stat* stat_set_money (Stat* stat, int money){
    if (!stat){
        return NULL;
    }
    stat->money=money;
    return stat;
}

Stat* stat_add_money (Stat* stat, int money){
    if (!stat){
        return NULL;
    }
    stat->money+=money;
    return stat;
}

int stat_get_money (Stat* stat){
	if(!stat) return -1;
	return stat->money;
}

Stat* stat_add_nodanger (Stat* stat, int nodanger){
    if (!stat){
        return NULL;
    }
    stat->nodanger+=nodanger;
    return stat;
}

int stat_get_nodanger (Stat* stat){
	if(!stat) return -1;
	return stat->nodanger;
}

/*In stat_stars we check the danger level and set the stars which represent that level of danger.*/
Stat* stat_stars (Stat* stat){
    int stars;
    if (!stat){
        return NULL;
    }
    stars = (int) stat->danger/DPERSTAR;
    if (stars > 5){
        stat->stars=5;
    }
    else if (stars < 0){
        return NULL;
    }
    else{
        stat->stars=stars;
    }
    return stat;
}

int stat_get_stars (Stat* stat){
	if(!stat) return -1;
	return stat->stars;
}
