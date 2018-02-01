#ifndef INTERACT_H
#define INTERACT_H

#include "background.h"

/*Macros for adjusting the danger/stars formula:*/
#define DANGER1 33
#define DANGER2 15
#define DANGER3 100
#define TIME 600 /*The amount of initial time available in seconds*/

/*In interact.h, we define an struct which consists on the previous structs plus a string which will be what appears in the dialog window.
This is done with the purpose that our game will consist on two threads: the first one is for printing constantly the remaining time.
The second one is what is interact.c and interface.c for: we have a while(1) loop in which we wait until the user introduces any key.
With that, we do the corresponding actions, which can result in changing the character position, one object, the whole screen, the stats,
talk with an npc, enter in inventory mode, enter in shop mode, enter in meeting mode, or maybe nothing happens. All these posibles changes are
stored in the return of the interact.c function, so that interface.c reads what happened with the key the user introduced, as well as getting the
string in our struct for printing it in the dialog window. With that, here is the interact struct:*/

typedef struct _Interact Interact;

typedef enum state {QUIT1,QUIT2,NOTHING,MOVE,PICK,SCREEN,TALK,INVENTORY,SHOP,MEETING1,MEETING2,MEETING3} State;

/*interact_load needs a filename with the filenames for loading the shop, the objects, the map...*/
Interact *interact_load(char*filename);

/*interact_getDialog returns THE DIRECTION of the last dialog got from interact_play, which is the dialog to be printed in the dialog window:*/
char* interact_getDialog(Interact *interact);

int interact_getBoxes(Interact *interact);

int interact_getPrinters(Interact *interact);

int interact_getMeetings(Interact *interact);

short interact_getDanger(Interact *interact);

Player* interact_getPlayer(Interact *interact);

Map* interact_getMap(Interact *interact);

Stat* interact_getStat(Interact *interact);

Inventory* interact_getInventory(Interact *interact);

Inventory* interact_getShop(Interact *interact);

int interact_getPx (Interact *interact);

int interact_getPy (Interact *interact);

void interact_updateClk(double remaining, Interact *interact);

/*This is the main funcion. We should be able to play the game without interface with this function and a while(1) loop. We read the key introduced by the player,
depending on which state we are we call different background functions, and depending the return of these functions we save the dialog and we return the state where
we go. (For example we are currently in Move, we read the key to move again, we call player move and we get that we changed screen, so we store the screen desc as
well as putting the state in screen):*/
State interact_play(State state, Interact *interact);

#endif
