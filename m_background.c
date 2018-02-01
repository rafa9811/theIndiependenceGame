/*Cosas que faltan:
	1. funciones de dialogs y de meetings (no sabemos hacer el archivo de texto)
	ARCHIVO DE TEXTO DE DIALOGS:
	fscanf(in,"%d\t%d\t%[^\n]s",&npc,&dlg,buff);
	esta funcion es la que lee del txt, donde primero se pone el id del npc al que le pertenece el diálogo, luego el número de dialogo correspondiente, y ya el dialogo en sí. (tabulando siempre entre estas tres cosas).

	Por ejemplo, si del Val, NPC de ID 0, dice en su diálogo 3 "Este no es mi laboratorio" ponéis esto
	0   3   Este no es mi laboratorio


	ARCHIVO DE TEXTO DE MEETINGS:
	fscanf(in,"%hd\t%d\t%d\t%[^\n]s",&solution,&meet,&q,buff);
	Este fscanf es el que lee de los meetings. Primero se pone cuál de las respuestas es la que sube motivacion (1 o 2 si son dos al final las respuestas posibles), despues el id de meeting al que pertenece la pregunta, luego el número de pregunta correspondiente (que la pregunta es tanto pregunta como poner 1.respuesta y 2.respuesta). Todo esto tabulando entre medias.

	Por ejemplo, para un primer meeting, de ID 0, la tercera pregunta es sobre la cabalgata de reyes, metéis esto:
	2   0   3   ¿Qué pondréis como carroza especial conmemorando el aniversario de los JJOO de España? 1.Una Drag Queen con esteladas. 2.¿Por qué conmemoraría el aniversario de otro país?



  2. moving: funciones de changeroom y move (no sabemos lo que es desc)
    Como os he dicho por Whatsapp, desc es una string que guardará el diálogo que se muestra al cambiar de sala en caso de cambiar de sala. Se ve en esta parte de código de interact.c:

            //  We changed room. Note that we check if the civil guard is in the screen we moved to, so as to initiate a danger clock.
                case(B_SCREEN):{
                    bg_updateRoom(IN(objects),IN(map),IN(player));
                    IN(danger)=bg_checkDanger(IN(map),IN(player),IN(stat));
                    if(IN(danger)==0) dialog(msg);
                    else dialog("Watch out! The civil guard will find you if you stay here too much time!");
                    return SCREEN;
                }

  3. Picking/Using/Leaving: bg_box (no sabemos que es directamente)
    Como os he dicho por Whatsapp, es una función que se utiliza si intentas usar en el inventario una urna, para comprobar si la puedes dejar o no. En esta parte del código de interact.c se entiende:

        //We want to use one of our 10 objects.
        if(choice>='0'&&choice<='9'){
            ret = bg_use(IN(inventory),IN(stat),choice-'0',name);
            switch(ret){
              //Using a ballot box
                case(B_BOX):{
                    ret = bg_box(IN(inventory),IN(player),IN(map),IN(stat),choice-'0');
                    if(ret == 1){
                        dialog("You placed a ballot box! Now more people will vote.");
                        IN(boxes)++;
                        return INVENTORY;
                    }


  4. talk: porque no tenemos cargado el archivo de dialogs

    Ya ci


  5. Falta cerrar todo lo que hemos inicializado.

  Na eso a la mierda no pasa na






  ********************************************************* I M P O R T A N T E : *****************************************************

  Pasadle un compilador o un makefile rapidito para ver los errores de sintaxis.

  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "background.h"
#define SCREEN_LINE 80
#define SCREENY 42

int printScreen(char *draw){
    char line[SCREEN_LINE*2],*ini;
    int i=0,linenumber=0;
    ini=draw;
    while(linenumber<SCREENY){
      draw = ini + i;
      memcpy(line,draw,SCREEN_LINE);
      printf("%s\n",line);
      i+=SCREEN_LINE;
      linenumber++;
    }
    return 0;
}

int main(int argc, char const *argv[]) {
Inventory *shop,*in;
Stat *stat;
short x;
char name[256],*roomdraw,dial[1024],nextq[1024];
Map *map;
Mobjects *mobj;
Object * obj;
int err;
Player *c;
Meeting *meeting;
Dialogs *dialog;

obj=object_ini();
stat=stat_ini ();
c=player_ini();

/*funciones de inventory*/

in = inventory_load("inventory.txt");
if (!in) fprintf(stderr, "Error in loading the inventory");

err= inventory_erase(in, 1);
if (err==-1) fprintf(stderr, "Error in erasing the object in the inventory");

err= inventory_erase(in,1);

obj=inventory_get(in,0);
if (!obj) fprintf(stderr, "Error in inventory get");

x=inventory_full(in);
if (x==-1) fprintf(stderr, "Error in inventory full: error en los parametros de entrada");
if (x==1) fprintf(stderr, "Error in inventory full: the inventory is full");

in=inventory_insert(in, obj);
if (!in) fprintf(stderr, "Error in inventory insert");


/*funciones de shop*/

shop = inventory_load("shop.txt");
if (!shop) fprintf(stderr, "Error in loading the shop");

x=bg_buy(shop, in, stat, 2, name);
if(x==0)fprintf(stderr, "Error no money");
if(x==-1)fprintf(stderr, "Error in function");


/*funciones de map*/

map=map_load ("map.txt");
if (!map) fprintf(stderr, "Error in loading the map");

roomdraw=map_getRoomDraw (map, 23);
if (!roomdraw) fprintf(stderr, "Error in drawing the room");

printScreen(roomdraw);

/*funciones de objects*/

mobj=mobjects_load("objects.txt");
if (!mobj) fprintf(stderr, "Error in loading the mobjects");

err=mobjects_insert(mobj, obj);
if (err==-1) fprintf(stderr, "Error in inserting the object in mboject");

err=mobjects_search(mobj, 12, 4, 5);
if (err==-1) fprintf(stderr, "Error in searching the object in mboject");

obj=mobjects_get(mobj, 14, 0);
if (!obj) fprintf(stderr, "Error in getting the object in mboject");

mobj= mobjects_erase(mobj, 3, 1);
if (!mobj) fprintf(stderr, "Error in erasing an element of the mobjects");

/*funciones de background*/
err=bg_updateRoom(mobj, map, c);
if(err==-1) fprintf(stderr, "Error in updateRoom");

roomdraw=map_getRoomDraw (map, 0);
if (!roomdraw) fprintf(stderr, "Error in drawing the room");

printScreen(roomdraw);


x=bg_checkDanger(map, c, stat);
if (x==-1) fprintf(stderr, "Error in checking danger");

err=bg_pick(in, c, mobj, name,map);
if(err==-1) fprintf(stderr, "Error in background pick");

err= bg_use(in, stat, 0, name,&err);
if(err==-1)  fprintf(stderr, "Error in background use");
if(err==0)  fprintf(stderr, "There are no more uses for the object");

meeting = meeting_load("meetings.txt");
dialog = dialogs_load("dialogs.txt");
/*Meeting*/
/*lo que falta de bg*/
err=bg_talk(dialog, c, mobj, dial, name);
if (err==-1) fprintf(stderr, "Error in background talk");

err= bg_meeting(meeting, c, mobj, stat, 1, 2, nextq,&err);
if (err==-1) fprintf(stderr, "Error in background meeting");

return 0;
}
