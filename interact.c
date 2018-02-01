#include <string.h>
#include "interact.h"
#include "player.h"
#define dialog(...) strcpy(interact->dialog,__VA_ARGS__)
#define IN(X) interact->X

/*The interact struct has everything needed for playing the game, as well as the next dialog that will be printed and game data for the score and danger timer:*/
struct _Interact{
    Player *player;
    Stat *stat;
    Map *map;
    Mobjects *objects;
    Inventory *inventory;
    Inventory *shop;
    Dialogs *dialogs;
    Meeting *meeting;
    char dialog[1024];
    int boxes;
    int printers;
    int meetings;
    short danger;
    int px;
    int py;
    int correct;
};

/*interact_load needs a filename with the filenames for loading the shop, the objects, the map...*/
Interact *interact_load(char*filename){
  Interact *interact;
	char path[512],name[128];
	FILE *in;

	if(!filename) return NULL;

	interact=(Interact*)malloc(sizeof(Interact));
	if(!interact) return NULL;

	in = (FILE*) fopen(filename, "r");
	if(!in){
		free(interact);
		return NULL;
	}

	while(!feof(in)){
		fscanf(in,"%s\t%s\n",name,path);
        if(strcmp(name,"Objects")==0){
            IN(objects)=mobjects_load(path);
        }
        else if(strcmp(name,"Inventory")==0){
            IN(inventory)=inventory_load(path);
        }
        else if(strcmp(name,"Shop")==0){
            IN(shop)=inventory_load(path);
        }
        else if(strcmp(name,"Map")==0){
            IN(map)=map_load(path);
        }
        else if(strcmp(name,"Dialogs")==0){
            IN(dialogs)=dialogs_load(path);
        }
        else if(strcmp(name,"Meetings")==0){
            IN(meeting)=meeting_load(path);
        }
	}

	IN(player)=player_ini();
	IN(stat)=stat_ini();

	if(!IN(objects) || !IN(inventory) || !IN(shop) || !IN(map) || !IN(dialogs) || !IN(meeting) || !IN(player) || !IN(stat)) exit(0);
    IN(boxes)=0;
    IN(printers)=0;
    IN(meetings)=0;
    IN(danger)=0;
    strcpy(IN(dialog),"Welcome to Barçalona Président! Press I to open the inventory, Q for quitting the game, or A for interacting. Good luck on not being arrested!");
    IN(px)=INIX;
    IN(py)=INIY;
    IN(correct)=1;
    bg_updateRoom(IN(objects),IN(map),IN(player));
	fclose(in);
	return interact;
}

/*interact_getDialog returns THE DIRECTION of the last dialog got from interact_play, which is the dialog to be printed in the dialog window:*/
char* interact_getDialog(Interact *interact){
    if(!interact){
        return NULL;
    }
    if(!IN(dialog)){
        return NULL;
    }
    return IN(dialog);
}

/*The following get functions return an int or the direction to an interact field.*/
int interact_getBoxes(Interact *interact){
    if(!interact){
        return -1;
    }
    return IN(boxes);
}

int interact_getPrinters(Interact *interact){
    if(!interact){
        return -1;
    }
    return IN(printers);
}

int interact_getMeetings(Interact *interact){
    if(!interact){
        return -1;
    }
    return IN(meetings);
}

short interact_getDanger(Interact *interact){
    if(!interact){
        return -1;
    }
    return IN(danger);
}

Player* interact_getPlayer(Interact *interact){
  if(!interact) return NULL;
  return IN(player);
}

Map* interact_getMap(Interact *interact){
  if(!interact) return NULL;
  return IN(map);
}

Stat* interact_getStat(Interact *interact){
  if(!interact) return NULL;
  return IN(stat);
}

Inventory* interact_getInventory(Interact *interact){
  if(!interact) return NULL;
  return IN(inventory);
}

Inventory* interact_getShop(Interact *interact){
  if(!interact) return NULL;
  return IN(shop);
}

int interact_getPx (Interact *interact){
    if(!interact) return -1;
  return IN(px);
}

int interact_getPy (Interact *interact){
    if(!interact) return -1;
  return IN(py);
}

/*updateClk recalculates the current danger in the game and thus the danger stars. Note the use of the DANGER macros for changing the formula.*/
void interact_updateClk(double remaining, Interact *interact){
  int newDanger, motivation, chaos, remainingtime, nodanger;
  if(!interact) return;
  if(!IN(stat)) return;
  motivation = stat_get_motivation(IN(stat));
  chaos = stat_get_chaos(IN(stat));
  nodanger = stat_get_nodanger(IN(stat));
  remainingtime = TIME - remaining;
  remainingtime = (remainingtime*remainingtime)/DANGER1;
  newDanger = motivation - chaos + remainingtime + nodanger;
  /*if(newDanger<0){
    newDanger=0;
  }*/
  stat_set_danger(IN(stat),newDanger);
  stat_stars(IN(stat));
  return;
}

/*This is the main funcion. We should be able to play the game without interface with this function and a while(1) loop. We read the key introduced by the player,
depending on which state we are we call different background functions, and depending the return of these functions we save the dialog and we return the state where
we go. (For example we are currently in Move, we read the key to move again, we call player move and we get that we changed screen, so we store the screen desc as
well as putting the state in screen):*/
State interact_play(State state, Interact *interact) {
  char choice;
  char name[256], msg[1024], dlg[1024];
  int ret,px,py,correctans=0,id=0;

  if(state>MEETING3||!interact){
      exit(-1);
  }
  
  if(IN(meetings)==5 && IN(boxes)==5 && IN(printers)==5) return QUIT2;

  /*We read the pressed key*/
  choice = fgetc(stdin);
  /*And now depending on our state what we pressed has different effects:*/
  switch (state){
      /*If we have just picked and object. (This state is for erasing from our screen the object we picked)*/
    case(PICK):{
        if(choice=='a'||choice=='A'){
            dialog(" ");
            return NOTHING;
        }
        if(choice=='q'||choice=='Q'){
            goto quit;
        }
        else return PICK;
    }
      /*If we just talked to an NPC.*/
    case(TALK):{
        if(choice=='a'||choice=='A'){
            dialog(" ");
            return NOTHING;
        }
        if(choice=='q'||choice=='Q'){
            goto quit;
        }
        else return TALK;
    }
      /*We just opened the inventory window.*/
    case(INVENTORY):{
      /*We close the inventory window*/
        if(choice=='i'||choice=='I'){
            dialog(" ");
            return NOTHING;
        }
        if(choice=='q'||choice=='Q'){
            goto quit;
        }
      /*We want to use one of our 10 objects.*/
        if(choice>='0'&&choice<='9'){
            ret = bg_use(IN(inventory),IN(stat),choice-'0',name,&id);
            switch(ret){
              /*Using a ballot box*/
                case(B_BOX):{
                    ret = bg_box(IN(inventory),IN(player),IN(map),IN(stat),choice-'0');
                    if(ret == 1){
                        dialog("You placed a ballot box! Now more people will vote.");
                        IN(boxes)++;
                        return INVENTORY;
                    }
                    else if(ret == 2){
                        dialog("You already have one ballot box placed here...");
                        return INVENTORY;
                    }
                    else if(ret == 0){
                        dialog("You can't place a ballot box here...");
                        return INVENTORY;
                    }
                    else exit(0);
                    }
              /*Using another object*/
                case(1):{
                  switch(id){
                    case(3):{
                      dialog("You used the power of fake news to spread your message even more! Everything went better, just take a look at the stats now...");
                      break;
                    }
                    case(7):{
                      dialog("The canvas was meant to cover the Looney Tunes boat all the time! A lot of civil guards got now in. Danger decreased!");
                      break;
                    }
                    case(8):{
                      dialog("As El Pais will suspect, you now have the full control of Twitter!");
                      break;
                    }
                    case(6):{
                      dialog("Thanks to the sacrifice that means wearing this, your danger decreased!");
                      break;
                    }
                    case(5):{
                      dialog("It's impossible to brush that perfect hair! Chaos increased!");
                      break;
                    }
                    case(13):{
                      dialog("You threw the PujolToken to the floor, and from the smoke JordiPujol appeared to give you money. You won 100 Ç!");
                      break;
                    }
                    case(14):{
                      dialog("Your incredibly disgusting supper made angry the civil guard, so they are occupied now tweeting. Danger decreased!");
                      break;
                    }
                    case(15):{
                      dialog("Filling your pockets with sand won't make Catalonia independent. What are you doing?");
                      break;
                    }
                    default:{
                      sprintf(msg,"You used %s.",name);
                      dialog(msg);
                      break;
                    }
                    return INVENTORY;
                  }
                }
                case(0):{
                    return INVENTORY;
                }
                default: exit(0);
            }
        }
        else return INVENTORY;
    }
    /*We interacted with the shop-keeper, so we are in the shop menu:*/
    case(SHOP):{
        /*Closing the shop window*/
        if(choice=='i'||choice=='I'){
            dialog("See you soon, President!");
            return NOTHING;
        }
        if(choice=='q'||choice=='Q'){
            goto quit;
        }
        /*Buying an object*/
        if(choice>='0'&&choice<='9'){
            ret = bg_buy(IN(shop),IN(inventory),IN(stat),choice-'0',name);
            switch(ret){
                case(1):{
                    sprintf(msg,"You bought %s.",name);
                    dialog(msg);
                return SHOP;
                }
                case(0):{
                    dialog("You don't have enough money...");
                    return SHOP;
                }
                case(B_FULL):{
                    dialog("Your inventory is full! Use one of all your objects before buying another one.");
                    return SHOP;
                }
                default: exit(0);
            }
        }
        else return SHOP;
  }
    /*We have just interacted with a lectern, so that we entered in meeting mode, with the game waiting for answering the first question.*/
    case(MEETING1):{
        if(choice>='1'&&choice<='2'){ /*The if is written like this in case of adding more answers per question.*/
          bg_meeting(IN(meeting),IN(player),IN(objects),IN(stat),0,choice-'0',msg,&correctans);
          if(correctans==0)IN(correct)=0;
          dialog(msg);
          return MEETING2;
        }
        else return MEETING1;
    }
    /*In a meeting answering the second question.*/
    case(MEETING2):{
        if(choice>='1'&&choice<='2'){ /*The if is written like this in case of adding more answers per question.*/
          bg_meeting(IN(meeting),IN(player),IN(objects),IN(stat),1,choice-'0',msg,&correctans);
          if(correctans==0)IN(correct)=0;
          dialog(msg);
          return MEETING3;
        }
        else return MEETING2;
    }
    /*Answering the third question*/
    case(MEETING3):{
        if(choice>='1'&&choice<='2'){ /*The if is written like this in case of adding more answers per question.*/
          bg_meeting(IN(meeting),IN(player),IN(objects),IN(stat),2,choice-'0',msg,&correctans);
          if(correctans==0)IN(correct)=0;
          if(IN(correct)==1)dialog("Excelent meeting President! Now your people will keep distracted the civil guard as your chaos went up!");
          else dialog("What have you done? Do you even know yourself? Your meeting has not been good at all, and you know chaos is important so as to keep the civil guard distracted... Next time talk more with the people.");
          return TALK;
        }
        else return MEETING3;
    }
    /*We pressed Q and the game is asking us to confirm by pressing again Q that we'd like to end the game.*/
    case(QUIT1):{
      if(choice=='q' || choice=='Q') return QUIT2;
      else{
          dialog(" ");
          return NOTHING;
      }
    }
    /*The rest of the situations: When we are moving in the city and we can either open the inventory, quit the game, interact with something as well as keep moving*/
    default:{
        if(choice=='q' || choice=='Q') goto quit;
        if(choice=='i' || choice=='I'){
            dialog("Welcome to your inventory! Press I to close it or press a number for using an object.");
            return INVENTORY;
        }
        if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
            choice = fgetc(stdin);
            px = player_get_px(IN(player));
            py = player_get_py(IN(player));
            IN(px)=px;
            IN(py)=py;
            switch(choice) {
                case('A'):{ /*Up*/
                    ret = bg_move(IN(player),px,py-1,IN(map),msg);
                    break;
                }
                case('B'):{ /*Down*/
                    ret = bg_move(IN(player),px,py+1,IN(map),msg);
                    break;
                }
                case('C'):{ /*Left*/
                    ret = bg_move(IN(player),px+1,py,IN(map),msg);
                    break;
                }
                case('D'):{ /*Right*/
                    ret = bg_move(IN(player),px-1,py,IN(map),msg);
                    break;
                }
            }
            switch(ret){
                /*We can't move to that position.*/
                case(B_NOTHING):return NOTHING;
                /*We moved in our screen.*/
                case(B_MOVE):return MOVE;
                /*We changed room. Note that we check if the civil guard is in the screen we moved to, so as to initiate a danger clock.*/
                case(B_SCREEN):{
                    bg_updateRoom(IN(objects),IN(map),IN(player));
                    IN(danger)=bg_checkDanger(IN(map),IN(player),IN(stat));
                    if(IN(danger)==0) dialog(msg);
                    else dialog("Watch out! The civil guard will find you if you stay here too much time!");
                    return SCREEN;
                }
                default: exit(0);
            }
        }
        /*We pressed the interact button.*/
        if(choice=='a' || choice=='A'){
            ret = bg_pick(IN(inventory),IN(player),IN(objects),name,IN(map));
            switch(ret){
                /*There's nothing near to us.*/
                case(B_NOTHING):{
                    return NOTHING;
                }
                /*We picked an object.*/
                case(B_PICK):{
                    sprintf(msg,"You picked %s.",name);
                    dialog(msg);
                    bg_updateRoom(IN(objects),IN(map),IN(player));
                    return PICK;
                }
                /*We talked to someone.*/
                case(B_NPC):{
                    ret = bg_talk(IN(dialogs),IN(player),IN(objects),dlg,name);
                    if (ret==1){
                        sprintf(msg,"%s: %s",name,dlg);
                        dialog(msg);
                        return TALK;
                    }
                }
                /*We started a meeting.*/
                case(B_MEETING):{
                  IN(correct)=1;
                    ret = bg_meeting(IN(meeting),IN(player),IN(objects),IN(stat),0,0,msg,&correctans);
                    if(ret == 3){
                      dialog("You've already done this meeting, President!");
                      return TALK;
                    }
                    dialog(msg);
                    IN(meetings)++;
                    return MEETING1;
                }
                /*We interacted with the shopkeeper, so that we open the shop menu.*/
                case(B_SHOP):{
                    dialog("Welcome to the shop! To exit press I, to buy any product press a number.");
                    return SHOP;
                }
                /*We turned on a printer.*/
                case(B_PRINTER):{
                    dialog("You just turned on a printer! Now the people will have more ballots to vote.");
                    stat_add_motivation(IN(stat),PRINTER_MOTIVATION);
                    IN(printers)++;
                    return TALK;
                }
                case(B_FULL):{
                  dialog("Your inventory is full! Use one of all your objects before picking up another one.");
                  return TALK;
                }
                default: exit(0);
            }
        }
        else return NOTHING;
    }

}
    /*Tag for the quit dialog:*/
    quit: dialog("Are you sure you want to quit? Press Q again to quit, press any key to continue playing");
    return QUIT1;
}
