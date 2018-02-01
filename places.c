#include "places.h"
#include <string.h>

struct _Place{
  	int placeID;
	short is_locked;
	short leavebox;
	short leftbox;
	int stars; /*The number of danger stars that make this room dangerous. For example, a 3 means that with 3 or more stars there is danger in the room*/
	int up; /*The upper adjacent room, which is -1 when there is no upper room.*/
	int down; /*The bottom adjacent room, which is... " */
	int left;
	int right;
	char desc[1024];
	char draw[3500];
};

Place* place_ini (){
	Place *p;
	p = (Place*) malloc(sizeof(Place));
	if (!p) return NULL;
	p->is_locked = 0;
	p->stars=0;
	p->up=-1;
	p->down=-1;
	p->left=-1;
	p->right=-1;
	return p;
}

void place_destroy (Place*p){
	if (p) free(p);
	return;
}


/****************************************Set Functions****************************************/


Place* place_setId (Place *p, int id){
	if (!p || id < 0) return NULL;
	p->placeID=id;
	return p;
}

Place* place_setLock (Place *p, short lock){
	if (!p || lock < 0 || lock > 1) return NULL;
	p->is_locked=lock;
	return p;
}

Place* place_setLeaveBox (Place *p, short box){
	if (!p || box < 0 || box > 1) return NULL;
	p->leavebox=box;
	return p;
}

Place* place_setLeftBox (Place *p, short box){
	if (!p || box < 0 || box > 1) return NULL;
	p->leftbox=box;
	return p;
}

Place* place_setStars (Place *p, int stars){
	if (!p || stars < 0) return NULL;
	p->stars=stars;
	return p;
}

Place* place_setUp (Place *p, int room){
	if (!p || room < -1) return NULL;
	p->up=room;
	return p;
}

Place* place_setDown (Place *p, int room){
	if (!p || room < -1) return NULL;
	p->down=room;
	return p;
}

Place* place_setLeft (Place *p, int room){
	if (!p || room < -1) return NULL;
	p->left=room;
	return p;
}

Place* place_setRight (Place *p, int room){
	if (!p || room < -1) return NULL;
	p->right=room;
	return p;
}

Place* place_setDesc (Place *p, char* desc){
	if (!p || !desc) return NULL;
	strcpy(p->desc,desc);
	return p;
}

Place* place_setDraw (Place *p, char* draw){
	if (!p || !draw) return NULL;
	strcpy(p->draw,draw);
	return p;
}


/****************************************Get Functions****************************************/


int place_getId (Place *p){
	if (!p) return -1;
	return p->placeID;
}

short place_getLock (Place *p){
	if (!p) return -1;
	return p->is_locked;
}

short place_getLeaveBox (Place *p){
	if (!p) return -1;
	return p->leavebox;
}

short place_getLeftBox (Place *p){
	if (!p) return -1;
	return p->leftbox;
}

int place_getStars (Place *p){
	if (!p) return -1;
	return p->stars;
}

int place_getUp (Place *p){
	if (!p) return -2;
	return p->up;
}

int place_getDown (Place *p){
	if (!p) return -2;
	return p->down;
}

int place_getLeft (Place *p){
	if (!p) return -2;
	return p->left;
}

int place_getRight (Place *p){
	if (!p) return -2;
	return p->right;
}

char* place_getDesc (Place *p){
	if (!p) return NULL;
	return p->desc;
}

char* place_getDraw (Place *p){
	if (!p) return NULL;
	return p->draw;
}


/****************************************Copy Functions****************************************/


Place* place_copy (Place *src){
	Place *p;
	if (!src) return NULL;
	p = place_ini();
	if (!p) return NULL;
	p->placeID = src->placeID;
	p->is_locked = src->is_locked;
	p->stars = src->stars;
	p->leavebox = src -> leavebox;
	p->leftbox = src -> leftbox;
	p->up = src->up;
	p->down = src->down;
	p->left = src->left;
	p->right = src->right;

	strcpy(p->draw,src->draw);
	strcpy(p->desc,src->desc);
	return p;
}



/****************************************Misc Functions****************************************/


Place* place_drawchar(Place* p, int x, int y, char c){
	if (!p || x<0 || y<0) return NULL;
  if((y*ROWSPCOL+x-2)<2) return p;
	p->draw[y * ROWSPCOL + x - 2] = c;
	return p;
}

int place_blankSpace(Place* p, int x, int y){
	if (!p || x<0 || y<0) return -1;
	if (p->draw[y*ROWSPCOL + x - 2] == ' ') return 1;
	else return 0;
}
