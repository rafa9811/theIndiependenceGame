#ifndef PLACE_H
#define PLACE_H
#define ROOM 26
#define ROWSPCOL 80
#include <stdio.h>
#include <stdlib.h>

typedef struct _Place Place;

Place* place_ini ();
void place_destroy (Place*p);


/****************************************Set Functions****************************************/


Place* place_setId (Place *p, int id);
Place* place_setLock (Place *p, short lock);
Place* place_setLeaveBox (Place *p, short box);
Place* place_setLeftBox (Place *p, short box);
Place* place_setStars (Place *p, int stars);
Place* place_setUp (Place *p, int room);
Place* place_setDown (Place *p, int room);
Place* place_setLeft (Place *p, int room);
Place* place_setRight (Place *p, int room);
Place* place_setDesc (Place *p, char* desc);
Place* place_setDraw (Place *p, char* draw);


/****************************************Get Functions****************************************/


int place_getId (Place *p);
short place_getLock (Place *p);
short place_getLeaveBox (Place *p);
short place_getLeftBox (Place *p);
int place_getStars (Place *p);
int place_getUp (Place *p);
int place_getDown (Place *p);
int place_getLeft (Place *p);
int place_getRight (Place *p);
char* place_getDesc (Place *p);
char* place_getDraw (Place *p);


/****************************************Copy Functions****************************************/


Place* place_copy (Place *src);
/*The textfile should be: "id	desc	is_locked	up	down	left	right	draw" per line*/


/****************************************Misc Functions****************************************/


Place* place_drawchar(Place* p, int x, int y, char c);
int place_blankSpace(Place* p, int x, int y);

#endif /* PLACE_H */
