#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct _Player {
		int px;
		int py;
		int room_id;
};


Player* player_ini(){
	Player *c;
	c = (Player*) malloc(sizeof(Player));
	if(!c) return NULL;
	c->px = INIX;
	c->py = INIY;
	c->room_id = INIROOM;
	return c;
}

void player_destroy(Player *c){
	if(c) free(c);
	return;
}

Player* player_move(Player *c,int x, int y){
	if(!c) return NULL;
	c->px=x;
	c->py=y;
	return c;
}

Player* player_set_room(Player *c, int room){
	if(!c) return NULL;
	c->room_id=room;
	return c;
}

int player_get_room(Player *c){
	if(!c) return -1;
	return c->room_id;
}

int player_get_px(Player* c){
	int px;
	if(!c) return -1;
	px= c->px;
	return px;
}

int player_get_py(Player* c){
	int py;
	if(!c) return -1;
	py= c->py;
	return py;
}

int player_distance(Player *c, int x, int y){
	int px, py;
	if (!c) return -1;
	px = player_get_px(c);
	py = player_get_py(c);
	if ((abs(x-px)) == (abs(y-py))) return abs(x-px+1);
	else if ((abs(x-px)) > (abs(y-py))) return (abs(x-px));
	else return (abs(y-py));
}

int player_isNear(Player *c, int x, int y){ /*x e y son las del objeto*/
	if (!c) return -1;
	if (player_distance(c, x, y) == 1) return 1;
	else return 0;
}
