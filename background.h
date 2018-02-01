#ifndef BACKGROUND_H
#define BACKGROUND_H


/*Macros for limiting our number of NPC's, dialogs per NPC, places, objects per place, objects per inventory and questions per meeting:*/
#define MAX_NPC 30
#define MAX_DLG 15
#define MAX_PLC 26
#define MAX_OBJ 10
#define MAX_INV 10
#define MAX_Q 3
#define MAX_MET 5

/*Macros for defining the most important id's. In the case of our videogame we have put these names:*/
#define ID_PRINTER 1
#define ID_BOX 0
#define ID_SHOP 2

/*Macros for defining some important returns in functions like picking or leaving an object:*/
#define B_MEETING 9
#define B_NPC 7
#define B_PRINTER 10
#define B_BOX 6
#define B_SHOP 8
#define B_NOTHING 0
#define B_PICK 1
#define B_MOVE 1
#define B_SCREEN 2
#define B_FULL 11
#define BOX_MOTIVATION 250
#define PRINTER_MOTIVATION 150

/*Macros for defining the characters for printing some objects in the game:*/
#define MEETING_CHAR 'X'
#define NPC_CHAR '@'
#define BOX_CHAR 'O'
#define PRINTER_CHAR '*'
#define SHOP_CHAR 'X'
#define DEFAULT_CHAR '*'


/*In the game motor we have an event which modifies one stat. In our case it's called meeting, and these are the macros for summing to the stat:*/
#define MEETING_CHAOS 100

/*Macros for limiting our game screen:*/
#define YLIMIT 36 /*The lowest row in the playable screen*/
#define XLIMIT 80 /*The furthest column in the playable screen*/


#include "object.h"
#include "stats.h"
#include "places.h"
#include "player.h"

typedef struct _Dialogs Dialogs;

typedef struct _Inventory Inventory;

typedef struct _MObjects Mobjects;

typedef struct _Map Map;

typedef struct _Meeting Meeting;


/******************************************************Dialogs Functions*******************************************************/

/*
struct _Dialogs{
	char *dlgs[MAX_NPC][MAX_DLG];
}*/



void dialogs_destroy(Dialogs *d);

Dialogs* dialogs_load(char* filename);


/************************************************************************Inventory Functions**********************************************************************/

/*struct _Inventory{
	Object **inv;
	int Max;
}*/


void inventory_destroy(Inventory *inv);

Inventory* inventory_load(char*filename);

int inventory_erase(Inventory *in, int oId);

Object* inventory_get(Inventory *in,int id);

int inventory_getMax(Inventory *in);

Inventory* inventory_insert(Inventory* inv, Object *o);

short inventory_full(Inventory* inv);



/************************************************************************MObjects Functions***********************************************************************/


/*
struct _Mobjects{
	Object *objs[MAX_PLC][MAX_OBJ];
}*/


void mobjects_destroy(Mobjects *m);

Mobjects* mobjects_load(char*filename);

int mobjects_insert(Mobjects *m, Object *o);

int mobjects_search(Mobjects *m, int roomid, int opx, int opy);

Object* mobjects_get(Mobjects *m, int roomid, int id);

Mobjects* mobjects_erase(Mobjects *m, int roomid, int id);



/************************************************************************Map Functions************************************************************************/


/*struct _Map{
	Place **map
}*/


void map_destroy(Map *m);

Map * map_load (char *filename);

char* map_getRoomDraw (Map *m,int room);

/**************************************************************************Meeting Functions**********************************************************************/

/*struct _Meeting{
	char* meet[MAX_MET][MAX_Q];
	short solution[MAX_MET][MAX_Q]; A short that stores which answer is the one which gives motivation.
};*/

void meeting_destroy(Meeting *d);

Meeting* meeting_load(char* filename);


/**************************************************************************Interaction Functions******************************************************************/


/*********************************************************************         Moving        *********************************************************************/


int bg_move(Player *c, int x, int y, Map *map,char*desc);

int bg_changeRoom(Player *c,short direction,int room,Map*map,char*desc);

int bg_updateRoom(Mobjects *m, Map* map, Player *c);

short bg_checkDanger(Map* map, Player *c, Stat *s);


/*********************************************************************Picking/Using/Leaving***********************************************************************/



int bg_pick(Inventory* in, Player *c,Mobjects *m,char *name,Map *map);

int bg_box(Inventory* in,Player *c, Map *m, Stat*s, int oId) ;

int bg_use(Inventory* in, Stat *s, int oId, char *name, int* id);


/*********************************************************************Buying**************************************************************************************/


/*the shop consists on an inventory. The price of each object is included in the money field from the object struct.
inventory *shop;
shop = inventory_load(filename);
*/

short bg_buy(Inventory*shop, Inventory *in, Stat *s, int id, char *name);


/*********************************************************************Talking*************************************************************************************/


int bg_talk(Dialogs *d, Player *c, Mobjects *m, char*dialog, char*name);

int bg_meeting(Meeting *meet, Player *c, Mobjects *m, Stat *s, int question, int answer, char *nextq, int *correct);

#endif
