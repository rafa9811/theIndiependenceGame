#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>

typedef struct _Object Object;

Object *object_ini();
void object_destroy(Object *O);

/****************************************Set Functions****************************************/


Object* object_setName(Object *O,char *name);
Object* object_setId(Object *O, int id);
Object* object_setRoomId(Object *O, short roomid);
Object* object_setPosition(Object *O, int px, int py);
Object* object_setNPC(Object *O, short npc);
Object* object_setMeeting(Object *O, short meeting);
Object* object_setIsPick(Object *O, short ispick);
Object* object_setOnOff(Object *O, short onoff);
Object* object_setUses(Object *O, int uses);
Object* object_setMotivation(Object *O, int motivation);
Object* object_setDanger(Object *O, int danger);
Object* object_setChaos(Object *O, int chaos);
Object* object_setMoney(Object *O, int money);


/****************************************Get Functions****************************************/



char* object_getName(Object *O);
int object_getId(Object *O);
short object_getRoomId(Object *O);
int object_getPx(Object *O);
int object_getPy(Object *O);
short object_getNPC(Object *O);
short object_getMeeting(Object *O);
short object_getIsPick(Object *O);
short object_getOnOff(Object *O);
int object_getUses(Object *O);
int object_getMotivation(Object *O);
int object_getDanger(Object *O);
int object_getChaos(Object *O);
int object_getMoney(Object *O);



/****************************************Copy Functions****************************************/


Object* object_copy(Object *O); /*Allocates memory for the new object.*/

#endif /* OBJECT_H */
