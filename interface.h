#ifndef INTERFACE_H
#define INTERFACE_H

#include "interact.h"
#include "rectangles.h"
#include "background.h"

/*Macros for the color of char and background.*/

#define BGCOLOR 1
#define CHARCOLOR 0

/*Macros for defining the limits for every window in interface, used in interface_ini.*/

#define SCREENX XLIMIT
#define SCREENY YLIMIT
#define CLKX 40
#define CLKY 5
#define STATX CLKX
#define STATY SCREENY-5
#define DLGX SCREENX+CLKX
#define DLGY 10



/*Macros for defining how long is a line in these windows, used in their corresponding print function.*/

#define DLG_LINE DLGX-15
#define SCREEN_LINE SCREENX
#define STAT_LINE STATX

/*Macro for printing the player.*/

#define PLAYER_CHAR '@'

/*Interface is a struct which consists on the three windows that will be shown in the game:
    -The screen window is the playable one, with the map drawing, player, objects...
    -The dialog(dlg) window is the one which is showing descriptions of the map, what are you using, the meeting questions, dialogs from the NPC's... Corresponds to the string stored in Interact that every time something happens it changes.
    -The stat window shows data about your current game like the number of meetings done, ballot boxes placed, motivation, chaos, money...
    -Note that we need one more window containing the clock, but it is declared in the game .c file, because of being updated at a different pace than the rest of the screen. If the clk window was in this struct, due to interface_play it would only update until we press any button.*/
typedef struct _Interface Interface;

/*interface_ini creates every window that appears in our screen:*/
Interface* interface_ini();

/*printDialog prints in the dialog window the dialog stored in interact. Note the while i<len loop for printing long dialogs that won't
fit in just one line.*/
int interface_printDialog(Interact* interact, Interface* interface);

/*printStat gets every important aspect of data of every game we play and prints it.*/
int interface_printStat(Interact *interact, Interface* interface);

/*printScreen prints all the screen in which the player is. Remember that, as done in background, it will contain every object in the room.
Appart, note that we use again the loop for printing a long line, due to the fact that the draw is stored in just one line.
Finaly, highlight the fact that this function would cause the game to run slower if we called it every time, which is another reason for using
the interact states.*/
int interface_printScreen(Interact *interact, Interface* interface);

/*Print player gets the current player position and prints its char there:*/
int interface_printPlayer(Interact *interact, Interface* interface);

/*blankSpace prints a blank space (" ") where the player was (px,py), so that when redrawing the player, where there was the PLAYER_CHAR there will be a blankSpace.*/
int interface_blankSpace(Interact *interact, Interface* interface);

/*Print inventory prints over the stats screen all the objects in our inventory with the number we should press for using it.*/
int interface_printInventory(Interact *interact, Interface *interface);

/*printShop is very similar to printInventory, with the exception that we are printing the shop objects, thus including the price.*/
int interface_printShop(Interact *interact, Interface *interface);

/*printClock is destined for being used in a thread appart, so it uses the clk window instead of all the interface. It prints the remaining time
as well as the remaining danger time, depending on the inDanger short.*/
void interface_printClock(sc_rectangle *clk,double remaining, short inDanger);

void interface_start(Interact *interact, Interface* interface);

/*The main interface function: Similar to interact play, it gets the state returned from interact_play, so as to know what to print for
not printing not enough information or for not printing too much (the game would wun slower). Note the PRINT_X macros for making the function
much more simpler in every case of this switch that depends on the state.*/
void interface_play(Interact *interact, Interface* interface, State state);

#endif
