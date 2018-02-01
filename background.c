#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "background.h"
#define UP 0 /*Macros for changing our screen while moving*/
#define LEFT 1
#define DOWN 2
#define RIGHT 3


struct _Dialogs{
	char *dlgs[MAX_NPC][MAX_DLG];
};

struct _Inventory{
	Object **inv;
	int max;
};

struct _MObjects{
	Object *objs[MAX_PLC][MAX_OBJ];
};

struct _Map{
	Place **map;
};

struct _Meeting{
	char* meet[MAX_MET][MAX_Q];
	short solution[MAX_MET][MAX_Q]; /*A short that stores which answer is the one which gives motivation.*/
};


/******************************************************Dialogs Functions*******************************************************/

/*
struct _Dialogs{
	char *dlgs[MAX_NPC][MAX_DLG];
}*/



void dialogs_destroy(Dialogs *d){
    int i=0,j=0;
    if(d){
        while(i<MAX_NPC){
            j = 0;
            while(j<MAX_DLG){
                if(d->dlgs[i][j]) free(d->dlgs[i][j]);
                else if(j == 0) goto freetag;
                else break;
                j++;
            }
            i++;
        }
    }
    freetag:free(d);
	return;
}

Dialogs* dialogs_load(char* filename){
	Dialogs *d;
	int npc=0,dlg=0;
	char buff[1024];
	FILE *in;

	if(!filename) return NULL;

	d=(Dialogs*)malloc(sizeof(Dialogs));
	if(!d) return NULL;

	in = (FILE*) fopen(filename, "r");
	if(!in){
		free(d);
		return NULL;
	}

	while(!feof(in)){
		fscanf(in,"%d\t%d\t%[^\n]",&npc,&dlg,buff);
		d->dlgs[npc][dlg]=(char*)malloc(strlen(buff)+1);
        strcpy(d->dlgs[npc][dlg],buff);
	}

	fclose(in);
	return d;
}


/************************************************************************Inventory Functions**********************************************************************/

/*struct _Inventory{
	Object **inv;
	int Max;
}*/



void inventory_destroy(Inventory *inv){
	int i=0;
	if(inv){
	    if(inv->inv){
	        for(i=0;inv->inv[i];i++){
	            object_destroy(inv->inv[i]);
	        }
	        free(inv->inv);
	    }
	free(inv);
	}
	return;
}

Inventory* inventory_load(char*filename){
	Object* Oaux;
	Inventory* inv;
	FILE *in;
	char buff[1024];
	int i=0;

	char name[256];
	int id,roomid,px,py,uses,motivation,danger,chaos,money;
	short npc,ispick,onoff,meeting;

	if(!filename) return NULL;

	inv=(Inventory*)malloc(sizeof(Inventory));
	if(!inv) return NULL;
	in = (FILE*) fopen(filename, "r");
	if(!in){
		free(inv);
		return NULL;
	}

    Oaux=object_ini();
    if(!Oaux){
		fclose(in);
		free(inv);
		return NULL;
	}

	inv->inv=(Object**)malloc(sizeof(Object*)*MAX_INV);

    while(!feof(in)){
			fgets(buff,1024,in);
      if(14 != sscanf(buff,"%[^\t]%d\t%d\t%d\t%d\t%hd\t%hd\t%hd\t%hd\t%d\t%d\t%d\t%d\t%d",name,&id,&roomid,&px,&py,&npc,&meeting,&ispick,&onoff,&uses,&motivation,&danger,&chaos,&money)) exit(-1);
      object_setName(Oaux,name);
      object_setId(Oaux,id);
      object_setRoomId(Oaux,roomid);
      object_setPosition(Oaux,px,py);
      object_setNPC(Oaux,npc);
      object_setMeeting(Oaux,meeting);
      object_setIsPick(Oaux,ispick);
      object_setOnOff(Oaux,onoff);
      object_setUses(Oaux,uses);
      object_setMotivation(Oaux,motivation);
      object_setDanger(Oaux,danger);
      object_setChaos(Oaux,chaos);
      object_setMoney(Oaux,money);
      inv->inv[i]=object_copy(Oaux);
      i++;
    }
    inv->max=i;
	object_destroy(Oaux);
	fclose(in);

	return(inv);
}

int inventory_erase(Inventory *in, int oId){
	int i;
	if(!in || oId<0)return -1;
	for(i=oId; i<in->max && in->inv[i]!=NULL;i++){
	    if(in->inv[i]) object_destroy(in->inv[i]);
		if(in->inv[i+1] && i+1!=in->max){
			in->inv[i] = object_copy(in->inv[i+1]);
		}
		else{
			break;
		}
	}
	in->max--;
	return 0;
}

int inventory_getMax(Inventory *in){
	if(!in)return -1;
	return in->max;
}

Object* inventory_get(Inventory *in,int id){
    if(!in || id<0)return NULL;
    return in->inv[id];
}

Inventory* inventory_insert(Inventory* inv, Object *o){
	if(!inv || !o)return NULL;

	inv->inv[inv->max]=object_copy(o);
	inv->max++;
	return inv;
}

short inventory_full(Inventory* inv){
    if(!inv) return -1;
    if (inv->max==MAX_INV) return 1;
    return 0;
}



/************************************************************************Mobjects Functions***********************************************************************/


/*
struct _Mobjects{
	Object *objs[MAX_PLC][MAX_OBJ];
}*/


void mobjects_destroy(Mobjects *m){
    int i=0,j=0;
    if(m){
        if(m->objs){
            while(i<MAX_PLC){
                j = 0;
                while(j<MAX_OBJ){
                    if(m->objs[i][j]) object_destroy(m->objs[i][j]);
                    else if(j == 0) goto freetag;
                    else break;
                    j++;
                }
                i++;
            }
        }
    }
    freetag:free(m);
	return;
}

Mobjects* mobjects_load(char*filename){
    Object* Oaux;
	Mobjects* m;
	FILE *in;
	int i;

	char name[256],buff[1024];
	int id,roomid,px,py,uses,motivation,danger,chaos,money;
	short npc,ispick,onoff,meeting;

	if(!filename) return NULL;

	m=(Mobjects*)malloc(sizeof(Mobjects));
	if(!m) return NULL;
	in = (FILE*) fopen(filename, "r");
	if(!in){
		free(m);
		return NULL;
	}

    Oaux=object_ini();
    if(!Oaux){
		fclose(in);
		free(m);
		return NULL;
	}

  while(!feof(in)){
		fgets(buff,1024,in);
		if(14 != sscanf(buff,"%[^\t]%d\t%d\t%d\t%d\t%hd\t%hd\t%hd\t%hd\t%d\t%d\t%d\t%d\t%d",name,&id,&roomid,&px,&py,&npc,&meeting,&ispick,&onoff,&uses,&motivation,&danger,&chaos,&money)) exit(-1);
		object_setName(Oaux,name);
    object_setId(Oaux,id);
    object_setRoomId(Oaux,roomid);
    object_setPosition(Oaux,px,py);
    object_setNPC(Oaux,npc);
		object_setMeeting(Oaux,meeting);
    object_setIsPick(Oaux,ispick);
    object_setOnOff(Oaux,onoff);
    object_setUses(Oaux,uses);
    object_setMotivation(Oaux,motivation);
    object_setDanger(Oaux,danger);
    object_setChaos(Oaux,chaos);
    object_setMoney(Oaux,money);
		for(i=0;m->objs[roomid][i]!=NULL;i++);
		m->objs[roomid][i]=object_copy(Oaux);
  }

	object_destroy(Oaux);
	fclose(in);

	return(m);
}

int mobjects_insert(Mobjects *m, Object *o){
	int i,roomid;
	if(!m || !o)return -1;
	roomid=object_getRoomId(o);
	for(i=0; m->objs[roomid][i]!=NULL && i<MAX_OBJ;i++);
	if(i<MAX_OBJ){
	    m->objs[roomid][i]=object_copy(o);
	    return 1;
	}
	else return 0;
}

int mobjects_search(Mobjects *m, int roomid, int opx, int opy){
	int mid,mpx,mpy;
	int i=0;
	if(roomid < 0 || roomid > MAX_PLC || opx < 0 || opx > XLIMIT || opy < 0 || opy > YLIMIT || !m) return -1;
	while(i<MAX_OBJ || m->objs[roomid][i]){
		mid=object_getRoomId(m->objs[roomid][i]);
		mpx=object_getPx(m->objs[roomid][i]);
		mpy=object_getPy(m->objs[roomid][i]);
		if((roomid==mid && opx==mpx+1 && opy==mpy)||(roomid==mid && opx==mpx-1 && opy==mpy)||(roomid==mid && opx==mpx && opy==mpy+1)||(roomid==mid && opx==mpx && opy==mpy-1)){
			return i;
		}
		i++;
	}
		return -1;
}

Object* mobjects_get(Mobjects *m, int roomid, int id){

	if(roomid < 0 || roomid > MAX_PLC || !m) return NULL;

    return m->objs[roomid][id];
}


Mobjects* mobjects_erase(Mobjects *m, int roomid, int id){
	int i=0;

	if(roomid < 0 || roomid > MAX_PLC || !m) return NULL;
	for(i=id; m->objs[roomid][i];i++){
			if(m->objs[roomid][i]) object_destroy(m->objs[roomid][i]);
		if(m->objs[roomid][i+1]){
			m->objs[roomid][i] = object_copy(m->objs[roomid][i+1]);
		}
		else{
			break;
		}
	}
	m->objs[roomid][i]=NULL;
	return m;

}



/************************************************************************Map Functions************************************************************************/


/*struct _Map{
	Place **map
}*/


void map_destroy(Map *m){
	int i=0;
	if(m){
	    if(m->map){
	        for(i=0;m->map[i];i++){
	            place_destroy(m->map[i]);
	        }
	        free(m->map);
	    }
	free(m);
	}
	return;
}

Map * map_load (char *filename){
    Place* paux;
    Map* map;
     int i;
	FILE *in;

	int id,stars,up,down,left,right;
	short is_locked,leavebox,leftbox;
	char desc[1024],draw[3500],buff[4092];

	if(!filename) return NULL;

	map=(Map*)malloc(sizeof(Map));
	if(!map) return NULL;

	map->map=(Place**)malloc(sizeof(Place*)*MAX_PLC);

	in = (FILE*) fopen(filename, "r");
	if (!in) {
		free(map->map);
		free(map);
		return NULL;
	}
    paux=place_ini();
    if(!paux){
		fclose(in);
		free(map->map);
		free(map);
		return NULL;
	}

  for(i=0;i<MAX_PLC && !feof(in);i++){
		fgets(buff,4092,in);
    if(11!=sscanf(buff,"%d\t%[^\t]%hd\t%hd\t%hd\t%d\t%d\t%d\t%d\t%d\t%[^\n]",&id,desc,&is_locked,&leavebox,&leftbox,&stars,&up,&down,&left,&right,draw)) exit(-1);
    place_setId(paux,id);
    place_setDesc(paux,desc);
    place_setLock(paux,is_locked);
    place_setLeaveBox(paux,leavebox);
    place_setLeftBox(paux,leftbox);
    place_setStars(paux,stars);
    place_setUp(paux,up);
    place_setDown(paux,down);
    place_setLeft(paux,left);
    place_setRight(paux,right);
    place_setDraw(paux,draw);
    map->map[i]=place_copy(paux);
  }
	place_destroy(paux);
	fclose(in);
	return map;
}

char* map_getRoomDraw (Map *m,int room){
    if(!m||room<0||room>MAX_PLC) return NULL;
    return place_getDraw(m->map[room]);
}

/**************************************************************************Meeting Functions**********************************************************************/
/*struct _Meeting{
	char* meet[MAX_MET][MAX_Q];
	short solution[MAX_MET][MAX_Q]; A short that stores which answer is the one which gives motivation.
};*/

void meeting_destroy(Meeting *d){
    int i=0,j=0;
    if(d){
        while(i<MAX_MET){
            j = 0;
            while(j<MAX_Q){
                if(d->meet[i][j]) free(d->meet[i][j]);
                else if(j == 0) goto freetag;
                else break;
                j++;
            }
            i++;
        }
    }
    freetag:free(d);
	return;
}

Meeting* meeting_load(char* filename){
	Meeting *d;
	int meet=0,q=0;
	short solution=0;
	char buff[512];
	FILE *in;

	if(!filename) return NULL;

	d=(Meeting*)malloc(sizeof(Meeting));
	if(!d) return NULL;

	in = (FILE*) fopen(filename, "r");
	if(!in){
		free(d);
		return NULL;
	}

	while(!feof(in)){
		fscanf(in,"%hd\t%d\t%d\t%[^\n]",&solution,&meet,&q,buff);
		d->meet[meet][q]=(char*)malloc(strlen(buff)+1);
    strcpy(d->meet[meet][q],buff);
		d->solution[meet][q]=solution;
	}

	fclose(in);
	return d;
}

/**************************************************************************Interaction Functions******************************************************************/


/*********************************************************************         Moving        *********************************************************************/


int bg_move(Player *c, int x, int y, Map *map,char*desc){
	int room;
	if(!c || !map)return -1;
	room = player_get_room(c);
	if(x<2) return bg_changeRoom(c,LEFT,room,map,desc);
	if (y>YLIMIT) return bg_changeRoom (c,DOWN,room,map,desc);
	if (x>XLIMIT) return bg_changeRoom (c,RIGHT,room,map,desc);
	if (y<0) return bg_changeRoom (c,UP,room,map,desc);
	if(place_blankSpace(map->map[room], x, y)==1){
			c=player_move(c, x, y);
			return B_MOVE;
	}
	return B_NOTHING;
}

int bg_changeRoom(Player *c,short direction,int room,Map*map,char*desc){
    int newroom;
    if(!c || direction<0 || direction > 3 || room < 0 || !map) return -1;
    switch (direction){
        case(UP):{
            newroom = place_getUp(map->map[room]);
            if (newroom == -1) return B_NOTHING;
            player_set_room(c,newroom);
            strcpy(desc,place_getDesc(map->map[newroom]));
            player_move(c,player_get_px(c),YLIMIT);
            return B_SCREEN;
        }
        case(LEFT):{
            newroom = place_getLeft(map->map[room]);
            if (newroom == -1) return B_NOTHING;
            player_set_room(c,newroom);
            strcpy(desc,place_getDesc(map->map[newroom]));
            player_move(c,XLIMIT,player_get_py(c));
            return B_SCREEN;
        }
        case(DOWN):{
            newroom = place_getDown(map->map[room]);
            if (newroom == -1) return B_NOTHING;
            player_set_room(c,newroom);
            strcpy(desc,place_getDesc(map->map[newroom]));
            player_move(c,player_get_px(c),0);
            return B_SCREEN;
        }
        case(RIGHT):{
            newroom = place_getRight(map->map[room]);
            if (newroom == -1) return B_NOTHING;
            player_set_room(c,newroom);
            strcpy(desc,place_getDesc(map->map[newroom]));
            player_move(c,2,player_get_py(c));
            return B_SCREEN;
        }
        default: return -1;
    }
}

int bg_updateRoom(Mobjects *m, Map* map, Player *c){
    int roomid,px,py,i=0,id;
    short onoff,npc,meeting;

    if(!m || !map || !c) return -1;

    roomid=player_get_room(c);

    while(i<MAX_OBJ && m->objs[roomid][i]){
		onoff=object_getOnOff(m->objs[roomid][i]);
		px=object_getPx(m->objs[roomid][i]);
		py=object_getPy(m->objs[roomid][i]);
		if(onoff==1){
	    	npc=object_getNPC(m->objs[roomid][i]);
	    	meeting=object_getMeeting(m->objs[roomid][i]);
	    	id=object_getId(m->objs[roomid][i]);
	    	if(meeting==1){
	    	    place_drawchar(map->map[roomid],px,py,MEETING_CHAR);
	    	}
	    	else if (npc==1){
	    	    place_drawchar(map->map[roomid],px,py,NPC_CHAR);
	    	}
	    	else switch(id){
	    	    case(ID_BOX):{
	    	        if(object_getIsPick(m->objs[roomid][i])!=0)place_drawchar(map->map[roomid],px,py,BOX_CHAR);
	    	        break;
	    	    }
	    	    case(ID_PRINTER):{
	    	        place_drawchar(map->map[roomid],px,py,PRINTER_CHAR);
	    	        break;
	    	    }
	    	    case(ID_SHOP):{
	    	        place_drawchar(map->map[roomid],px,py,SHOP_CHAR);
	    	        break;
	    	    }
	    	    default: place_drawchar(map->map[roomid],px,py,DEFAULT_CHAR);
	    	}
		}
		i++;
    }
    return 0;
}

/*Every time we change room we must check the danger in it:*/

short bg_checkDanger(Map* map, Player *c, Stat *s){
    int statStars, roomStars;
    if (!map || !c || !s) return -1;
    statStars = stat_get_stars(s);
    roomStars = place_getStars(map->map[player_get_room(c)]);
    if(statStars == -1 || roomStars == -1) return -1;
		if(statStars >= roomStars) return 1;
    return 0;
}


/*********************************************************************Picking/Using/Leaving***********************************************************************/



int bg_pick(Inventory* in, Player *c,Mobjects *m,char *name,Map *map){
	Object *o, *oaux;
	int id,roomid,px,py,uses;

	if(!in || !c || !m || !map) return -1;

	roomid=player_get_room(c);
	px=player_get_px(c);
	py=player_get_py(c);

    id = mobjects_search(m,roomid,px,py);

    if(id==-1) return B_NOTHING;
    o = mobjects_get(m,roomid,id);
	if(!o) return B_NOTHING;
	if(object_getIsPick(o)==1){
	    if(inventory_full(in)) return B_FULL;
	    strcpy(name,object_getName(o));
			object_setOnOff(o,0);
			px=object_getPx(o);
			py=object_getPy(o);
			place_drawchar(map->map[roomid],px,py,' ');
			inventory_insert(in,o);
	    mobjects_erase(m,roomid,id);
	    return B_PICK;
	}
	else if(object_getNPC(o)==1) return B_NPC;
	else if(object_getMeeting(o)==1) return B_MEETING;
	else if(object_getId(o)==ID_SHOP) return B_SHOP;
	else if(object_getId(o)==ID_PRINTER){
	    if((uses = object_getUses(o))>0){
	        object_setUses(o,uses-1);
	        return B_PRINTER;
	    }
	    else return B_NOTHING;
	}
	return -1;
}

int bg_box(Inventory* in,Player *c, Map *m, Stat*s,int oId) {
    if(!in || !c || !m) return -1;

    if(place_getLeaveBox(m->map[player_get_room(c)])==1 && place_getLeftBox(m->map[player_get_room(c)])==0 ){
        inventory_erase(in,oId);
        stat_add_motivation(s,BOX_MOTIVATION);
				place_setLeftBox(m->map[player_get_room(c)],1);
        return 1;
    }

    else if(place_getLeaveBox(m->map[player_get_room(c)])==1)return 2;
    else return 0;
}

int bg_use(Inventory* in, Stat *s, int oId, char* name, int*id){
	Object* o;
	int us,nchaos,ndanger,nmot,nmoney,objectId;
	if(!in || !s || oId<0 || oId>MAX_INV)return -1;
	if(oId >= in->max) return 0;
	o=inventory_get(in,oId);
	if(!o)return 0;
	if(object_getId(o)==ID_BOX){
		return B_BOX;
	}
	us=object_getUses(o);
	if(us==0)return 0;
	object_setUses(o, us-1);
	nchaos=object_getChaos(o);
	if(nchaos!=0){
		stat_add_chaos(s,nchaos);
	}
	ndanger=object_getDanger(o);
	if(ndanger!=0){
		stat_add_nodanger(s,ndanger);
	}
	nmot=object_getMotivation(o);
	if(nmot!=0){
		stat_add_motivation(s,nmot);
	}
	nmoney=object_getMoney(o);
	if(nmoney!=0){
		stat_add_money(s,nmoney);
	}
	us--;
	strcpy(name,object_getName(o));
	*id = object_getId(o);
	if(us==0){
		inventory_erase(in, oId);
	}
	return 1;
	}


/*********************************************************************Buying**************************************************************************************/


/*the shop consists on an inventory. The price of each object is included in the money field from the object struct. (as a positive number)
inventory *shop;
shop = inventory_load(filename);
*/

short bg_buy(Inventory*shop, Inventory *in, Stat *s, int id, char* name){
	Object *o;
	int price;
	if(!shop || !in || !s || id<0 || id > MAX_INV) return -1;
	if(inventory_full(in)) return B_FULL;
	o=inventory_get(shop, id);
	if(!o)return -1;
	price = -object_getMoney(o);
	if(stat_get_money(s)<(-price)){
	    return 0;
	}
	stat_add_money(s,price);
	object_setMoney(o,0);
	strcpy(name,object_getName(o));
	if((inventory_insert(in, o))==NULL)return-1;
	inventory_erase(shop,id);
	return 1;
}


/*********************************************************************Talking*************************************************************************************/


int bg_talk(Dialogs *d, Player *c, Mobjects *m, char*dialog, char*name){
    Object *npc;
    int npcid, uses, oid, roomid, px, py;
	if(!m || !d )return -1;

	roomid=player_get_room(c);
	px=player_get_px(c);
	py=player_get_py(c);

    oid = mobjects_search(m,roomid,px,py);

    if(oid==-1) return 0;

	npc = mobjects_get(m,roomid,oid);

	if(object_getNPC(npc)!=1)return -1;
	strcpy(name,object_getName(npc));
  npcid = object_getId(npc);
  uses = object_getUses(npc);
	strcpy(dialog,d->dlgs[npcid][uses]);
	if(uses>0)object_setUses(npc,uses-1);

	return 1;
}

int bg_meeting(Meeting *meet, Player *c, Mobjects *m, Stat *s, int question, int answer, char *nextq, int*correct){
	int id,roomid,px,py,oid,solution,uses;
	Object *meeting;

	if(!meet || !c || !m || !s || question>MAX_Q) return -1;

	roomid=player_get_room(c);
	px=player_get_px(c);
	py=player_get_py(c);

  oid = mobjects_search(m,roomid,px,py);
	meeting = mobjects_get(m,roomid,oid);
	id = object_getId(meeting);
	uses = object_getUses(meeting);
	if(uses<=0)return 3;	

	if(question==0 && answer == 0){
		strcpy(nextq,meet->meet[id][0]);
		return 0;
	}
	solution=meet->solution[id][question];
	if(answer==solution){
		stat_add_chaos(s,MEETING_CHAOS);
		*correct = 1;
	}
	else{
		stat_add_chaos(s,-MEETING_CHAOS);
		*correct = 0;
	}
	if(question+1<MAX_Q){
		strcpy(nextq,meet->meet[id][question+1]);
		return 1;
	}
	else{
		object_setUses(meeting,0);
		return 2;
	} 
}
