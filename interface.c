#include <string.h>
#include <stdlib.h>
#include "interface.h"
#include "interact.h"
#include "player.h"
#define PRINT_DIALOG interface_printDialog(interact,interface)
#define PRINT_PLAYER interface_printPlayer(interact,interface)
#define PRINT_SCREEN interface_printScreen(interact,interface)
#define PRINT_STAT interface_printStat(interact,interface)
#define PRINT_INVENTORY interface_printInventory(interact,interface)
#define PRINT_SHOP interface_printShop(interact,interface)
#define PRINT_SPACE interface_blankSpace(interact,interface)

struct _Interface{
    sc_rectangle *screen, *dlgs, *stats, *clk;
};

/*interface_ini creates every window that appears in our screen:*/
Interface* interface_ini(){
    Interface *interface;
    interface = (Interface*)malloc(sizeof(Interface));
    /*The screen win, where our player moves, picks up objects, interacts... The main action in the game*/
    interface->screen = win_new(0, 0, SCREENY+2, SCREENX+3, BGCOLOR, CHARCOLOR);
    /*The dialogs window is where every dialog stored in interact_play will be printed:*/
    interface->dlgs = win_new(SCREENY+2, 0, DLGY, DLGX, BGCOLOR, CHARCOLOR);
    /*The clk window, that won't be used here, but in the main c, where it is constantly updating for showing the remaining time.
    If it was printed at the same pace as the rest of the windows, we would only get an update of the remaining time when we do something.*/
    interface->clk = win_new(0,SCREENX+5,CLKY,CLKX,BGCOLOR,CHARCOLOR);
    /*Stat screen, updated every time we do something, for showing our money, number of printers activated and so on:*/
    interface->stats = win_new(CLKY, SCREENX+2, STATY, STATX, BGCOLOR, CHARCOLOR);
    return interface;
}

/*printDialog prints in the dialog window the dialog stored in interact. Note the while i<len loop for printing long dialogs that won't
fit in just one line.*/
int interface_printDialog(Interact* interact, Interface* interface){
    char *dialog,*ini,line[2*DLG_LINE];
    int len,i=0,linenumber=2;
    if(!interact || !interface) return -1;
    win_cls(interface->dlgs);
    dialog=(char*)malloc(1024);
    strcpy(dialog,interact_getDialog(interact));
    len = strlen(dialog);
    if(len>DLG_LINE){
        ini=dialog;
        while(i<len && linenumber<DLG_LINE){
        dialog = ini + i;
        memcpy(line,dialog,DLG_LINE);
        win_write_line_at(interface->dlgs,linenumber,2,line);
        i+=DLG_LINE;
        linenumber++;
        }
    }
    else win_write_line_at(interface->dlgs,linenumber,2,dialog);
    dialog=ini;
    if(dialog)free(dialog);
    return 0;
}

/*printStat gets every important aspect of data of every game we play and prints it.*/
int interface_printStat(Interact *interact, Interface* interface){
    char line[STAT_LINE];
    Stat *s;
    if(!interact || !interface) return -1;

    win_cls(interface->stats);

    s=interact_getStat(interact);
    if(!s) return -1;

    sprintf(line,"S T A T S: ");
    win_write_line_at(interface->stats,2,2,line);

    sprintf(line,"Danger: %d/5",stat_get_stars(s));
    win_write_line_at(interface->stats,3,2,line);

    sprintf(line,"Ballot boxes: %d/5",interact_getBoxes(interact));
    win_write_line_at(interface->stats,5,2,line);

    sprintf(line,"Printers: %d/5",interact_getPrinters(interact));
    win_write_line_at(interface->stats,6,2,line);

    sprintf(line,"Meetings: %d/5",interact_getMeetings(interact));
    win_write_line_at(interface->stats,7,2,line);

    sprintf(line,"Motivation: %d",stat_get_motivation(s));
    win_write_line_at(interface->stats,8,2,line);

    sprintf(line,"Chaos: %d",stat_get_chaos(s));
    win_write_line_at(interface->stats,9,2,line);

    sprintf(line,"Money: %d Ç",stat_get_money(s));
    win_write_line_at(interface->stats,10,2,line);

    sprintf(line,"(danger: %d)",stat_get_danger(s));
    win_write_line_at(interface->stats,14,2,line);


    return 0;
}

/*printScreen prints all the screen in which the player is. Remember that, as done in background, it will contain every object in the room.
Appart, note that we use again the loop for printing a long line, due to the fact that the draw is stored in just one line.
Finaly, highlight the fact that this function would cause the game to run slower if we called it every time, which is another reason for using
the interact states.*/
int interface_printScreen(Interact *interact, Interface* interface){
    int linenumber=0,room;
    char line[SCREEN_LINE*2],*draw,*ini;
    int i=0;
    if(!interact || !interface) return -1;
    win_cls(interface->screen);
    room=player_get_room(interact_getPlayer(interact));
    draw=map_getRoomDraw(interact_getMap(interact),room);
    if(!draw)return -1;
    ini=draw;
    while(linenumber<=YLIMIT){
      draw = ini + i;
      memcpy(line,draw,SCREEN_LINE);
      win_write_line_at(interface->screen,linenumber,2,line);
      i+=SCREEN_LINE;
      linenumber++;
    }
    return 0;
}

/*Print player gets the current player position and prints its char there:*/
int interface_printPlayer(Interact *interact, Interface* interface){
  int px,py;
  Player*player;
  if(!interact||!interface) return -1;
  player=interact_getPlayer(interact);
  if(!player) return -1;
  px=player_get_px(player);
  py=player_get_py(player);
  if(px<0||py<0||px>XLIMIT||py>YLIMIT) return -1;
  win_write_char_at(interface->screen,py,px,PLAYER_CHAR);
  return 0;
}

/*blankSpace prints a blank space (" ") where the player was (px,py), so that when redrawing the player, where there was the PLAYER_CHAR there will be a blankSpace.*/
int interface_blankSpace(Interact *interact, Interface* interface){
  int px,py;
  if(!interact||!interface) return -1;
  px=interact_getPx(interact);
  py=interact_getPy(interact);
  if(px<0||py<0||px>XLIMIT||py>YLIMIT) return -1;
  win_write_char_at(interface->screen,py,px,' ');
  return 0;
}

/*Print inventory prints over the stats screen all the objects in our inventory with the number we should press for using it.*/
int interface_printInventory(Interact *interact, Interface *interface){
  Inventory *aux;
  int i=0,linenumber=1,j=0,len,max;
  char line[STAT_LINE],prename[STAT_LINE*10],*name,*ini;
  Object *o;
  if(!interact||!interface) return -1;
  aux = interact_getInventory(interact);
  if(!aux) return -1;
  win_cls(interface->stats);
  max = inventory_getMax(aux);
  sprintf(line,"I N V E N T O R Y:");
  win_write_line_at(interface->stats,0,2,line);
  for(i=0;i<max && linenumber<STATY;i++){
    o = inventory_get(aux,i);
    if(!o){
        name = ini;
        if(name) free(name);
        return 0;
    }
    strcpy(prename,object_getName(o));
    name=(char*)malloc(STAT_LINE*10);
    sprintf(name,"%d: %s",i,prename);
    len = strlen(name);
    ini=name;
    if(len>STAT_LINE){
    j=0;
    while(j<len && linenumber<STATY){
      name = ini + j;
      memcpy(line,name,STAT_LINE);
      win_write_line_at(interface->stats,linenumber,2,line);
      j+=STAT_LINE;
      linenumber++;
    }
    }
    else{
      win_write_line_at(interface->stats,linenumber,2,name);
      linenumber++;
    }
  }
  name=ini;
  if(name)free(name);
  return 0;
}

/*printShop is very similar to printInventory, with the exception that we are printing the shop objects, thus including the price.*/
int interface_printShop(Interact *interact, Interface *interface){
  Inventory *aux;
  int i=0,linenumber=1,j=0,price,len,max;
  char line[STAT_LINE],prename[STAT_LINE*10],*name,*ini;
  Object *o;
  if(!interact||!interface) return -1;
  win_cls(interface->stats);
  aux = interact_getShop(interact);
  max = inventory_getMax(aux);
  if(!aux) return -1;
  sprintf(line,"S H O P: ");
  win_write_line_at(interface->stats,0,2,line);
  for(i=0;i<max && linenumber<STATY;i++){
    o = inventory_get(aux,i);
    if(!o){
        name = ini;
        if(name) free(name);
        return 0;
    }
    strcpy(prename,object_getName(o));
    price = object_getMoney(o);
    name=(char*)malloc(STAT_LINE*10);
    sprintf(name,"%d (%d Ç): %s",i,price,prename);
    len = strlen(name);
    ini=name;
    if(len>STAT_LINE){
    j=0;
    while(j<len && linenumber<STATY){
      name = ini + j;
      memcpy(line,name,STAT_LINE);
      win_write_line_at(interface->stats,linenumber,2,line);
      j+=STAT_LINE;
      linenumber++;
    }
    }
    else{
      win_write_line_at(interface->stats,linenumber,2,name);
      linenumber++;
    }
  }
  name = ini;
  if(name)free(name);
  return 0;
}

/*printClock is destined for being used in a thread appart, so it uses the clk window instead of all the interface. It prints the remaining time
as well as the remaining danger time, depending on the inDanger short.*/
void interface_printClock(sc_rectangle *clk,double remaining, short inDanger){
  char line[STAT_LINE];
  if(inDanger < 0 || inDanger > 1) return;
  win_cls(clk);
  if(inDanger == 0){
    if(remaining>0){
      if(((int)remaining)%60<10) sprintf(line,"Remaining time: %d:0%d\n",(int)remaining/60,(int)remaining%60);
      else sprintf(line,"Remaining time: %d:%d\n",(int)remaining/60,(int)remaining%60);
    }
    else sprintf(line,"TIME'S UP!");
  }
  else{
    if(remaining>0){
      if(((int)remaining)%60<10) sprintf(line,"Time until detention: %d:0%d\n",(int)remaining/60,(int)remaining%60);
      else sprintf(line,"Time until detention: %d:%d\n",(int)remaining/60,(int)remaining%60);
    }
    else sprintf(line,"ARRESTED!");
  }
  win_write_line_at(clk,1,2,line);
}

void interface_start(Interact *interact, Interface* interface){
  if(!interact || !interface) exit(-1);
  PRINT_DIALOG;
  PRINT_STAT;
  PRINT_SCREEN;
  PRINT_PLAYER;
  return;
}

/*The main interface function: Similar to interact play, it gets the state returned from interact_play, so as to know what to print for
not printing not enough information or for not printing too much (the game would wun slower). Note the PRINT_X macros for making the function
much more simpler in every case of this switch that depends on the state.*/
void interface_play(Interact *interact, Interface* interface, State state){
  if(!interact || !interface || state > MEETING3) exit(-1);
  switch(state){
    case(QUIT1):{
      PRINT_DIALOG;
      PRINT_STAT;
      return;
    }
    case(QUIT2):{
      return;
    }
    case(NOTHING):{
      PRINT_STAT;
      return;
    }
    case(MOVE):{
      PRINT_PLAYER;
      PRINT_SPACE;
      PRINT_STAT;
      return;
    }
    case(PICK):{
      PRINT_DIALOG;
      PRINT_STAT;
      PRINT_SCREEN;
      PRINT_PLAYER;
      return;
    }
    case(SCREEN):{
      PRINT_DIALOG;
      PRINT_STAT;
      PRINT_SCREEN;
      PRINT_PLAYER;
      return;
    }
    case(TALK):{
      PRINT_DIALOG;
      PRINT_STAT;
      return;
    }
    case(INVENTORY):{
      PRINT_DIALOG;
      PRINT_INVENTORY;
      return;
    }
    case(SHOP):{
      PRINT_DIALOG;
      PRINT_SHOP;
      return;
    }
    case(MEETING1):{
      PRINT_DIALOG;
      PRINT_STAT;
      return;
    }
    case(MEETING2):{
      PRINT_DIALOG;
      PRINT_STAT;
      return;
    }
    case(MEETING3):{
      PRINT_DIALOG;
      PRINT_STAT;
      return;
    }
    default:exit(-1);
  }
}
