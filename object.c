#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

struct _Object {
  char *name;
  int   id;
  short roomid;
  int   px;
  int   py;
  short npc; /*0 means not an npc, 1 means it's an NPC.*/
  short ispick;
  short onoff;
  short meeting;
  int uses; /*-1 for unlimited uses, 0 means not usable,a positive number is the number of uses.*/
  int motivation;
  int danger;
  int chaos;
  int money;
};


Object* object_ini(){
  Object *O;

  O=(Object*)malloc(sizeof(Object));
  if(!O)return NULL;
  O->name=(char*)malloc(256*sizeof(char));
  if(O->name==NULL){
    free(O);
    return NULL;
  }
  O->id=0;
	O->roomid=0;
	O->px=0;
	O->py=0;
	O->npc=0;
  O->meeting=0;
  O->ispick=0;
  O->onoff=0;
	O->uses=0;
	O->motivation=0;
	O->danger=0;
	O->chaos=0;
	O->money=0;


  return O;
}

void object_destroy(Object *O){
  if(O){
  if(O->name) free(O->name);
  free(O);
  }
  O = NULL;
}

/****************************************Set Functions****************************************/


Object* object_setName(Object *O,char *name){
  if(!O || !name)return NULL;

  strcpy(O->name,name);

  return O;
}

Object* object_setId(Object *O, int id){
  if(!O)return NULL;

  O->id=id;

  return O;
}

Object* object_setRoomId(Object *O, short roomid){
  if(!O || roomid<0)return NULL;

  O->roomid=roomid;

  return O;
}

Object* object_setPosition(Object *O, int px, int py){
  if(!O ||  px<0 || py<0)return NULL;

  O->px=px;
  O->py=py;

  return O;
}

Object* object_setNPC(Object *O, short npc){
  if(!O || (npc!=0 && npc!=1))return NULL;

  O->npc=npc;

  return O;
}

Object* object_setMeeting(Object *O, short meeting){
  if(!O || (meeting!=0 && meeting!=1))return NULL;

  O->meeting=meeting;

  return O;
}

Object* object_setIsPick(Object *O, short ispick){
  if(!O || (ispick!=0 && ispick!=1))return NULL;

  O->ispick=ispick;

  return O;
}

Object* object_setOnOff(Object *O, short onoff){
  if(!O || (onoff!=0 && onoff!=1))return NULL;

  O->onoff=onoff;

  return O;
}

Object* object_setUses(Object *O, int uses){
  if(!O || uses < -1)return NULL;

  O->uses=uses;

  return O;
}

Object* object_setMotivation(Object *O, int motivation){
  if(!O)return NULL;

  O->motivation=motivation;

  return O;
}

Object* object_setDanger(Object *O, int danger){
  if(!O)return NULL;

  O->danger=danger;

  return O;
}

Object* object_setChaos(Object *O, int chaos){
  if(!O)return NULL;

  O->chaos=chaos;

  return O;
}

Object* object_setMoney(Object *O, int money){
  if(!O)return NULL;

  O->money=money;

  return O;
}



/****************************************Get Functions****************************************/



char* object_getName(Object *O){
  char *name;
  if(!O)return NULL;

  name=O->name;

  return name;
}

int object_getId(Object *O){
  int id;
  if(!O)return -1;

  id=O->id;

  return id;
}

short object_getRoomId(Object *O){
  short roomid;
  if(!O)return -1;

  roomid=O->roomid;

  return roomid;
}

int object_getPx(Object *O){
  int px;
  if(!O)return -1;

  px=O->px;

  return px;
}

int object_getPy(Object *O){
  int py;
  if(!O)return -1;

  py=O->py;

  return py;
}

short object_getNPC(Object *O){
  short npc;
  if(!O)return -1;

  npc=O->npc;

  return npc;
}

short object_getMeeting(Object *O){
  short meeting;
  if(!O)return -1;

  meeting=O->meeting;

  return meeting;
}

short object_getIsPick(Object *O){
  short ispick;
  if(!O)return -1;

  ispick=O->ispick;

  return ispick;
}

short object_getOnOff(Object *O){
  short onoff;
  if(!O)return -1;

  onoff=O->onoff;

  return onoff;
}

int object_getUses(Object *O){
  int uses;
  if(!O)return -2; /*Returning -1 would mean it is of infinite uses*/

  uses=O->uses;

  return uses;
}

int object_getMotivation(Object *O){
  int motivation;
  if(!O)return -1;

  motivation=O->motivation;

  return motivation;
}

int object_getDanger(Object *O){
  int danger;
  if(!O)return -1;

  danger=O->danger;

  return danger;
}

int object_getChaos(Object *O){
  int chaos;
  if(!O)return -1;

  chaos=O->chaos;

  return chaos;
}

int object_getMoney(Object *O){
  int money;
  if(!O)return -1;

  money=O->money;

  return money;
}



/****************************************Copy Functions****************************************/


Object* object_copy(Object *O){
  Object *O2;

  O2=object_ini();
  if(!O2)return NULL;

  strcpy(O2->name,O->name);
  O2->id=O->id;
  O2->roomid=O->roomid;
  O2->px=O->px;
  O2->py=O->py;
	O2->npc=O->npc;
  O2->meeting=O->meeting;
  O2->ispick=O->ispick;
  O2->onoff=O->onoff;
	O2->uses=O->uses;
	O2->motivation=O->motivation;
	O2->danger=O->danger;
	O2->chaos=O->chaos;
	O2->money=O->money;

  return O2;
}
