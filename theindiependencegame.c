#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include "interact.h"
#include "interface.h"

#define PATH "interact.txt" /*The filepath to the loading txt file*/
#define DANGERTIME 30 /*The amount of time in danger available in seconds*/
#define MAX 10000
#define INTROX CLKX + SCREENX
#define INTROY DLGY + SCREENY
#define INTRO_LINE INTROX - 55

struct termios initial;
short inDanger; /*A global variable used for knowing which clock to print and use: the main clock or the clock that starts running
once our player enters in a dangerous room.*/
short end;

/*
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init() {
	struct termios new;	          /*a termios structure contains a set of attributes about
					  how the terminal scans and outputs data*/

	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out
						 terminal (fileno returns the file descriptor
						 of stdin) and save them in initial. We'd better
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;	              /*here we are setting up new. This line tells to stop the
						canonical mode (which means waiting for the user to press
						enter before sending)*/
	new.c_lflag &= ~ECHO;                 /*by deactivating echo, we tell the terminal NOT TO
						show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;                  /*this states the minimum number of characters we have
					       to receive before sending is 1 (it means we won't wait
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;	              /*I really have no clue what this does, it must be somewhere in the book tho*/
	new.c_lflag &= ~ISIG;                 /*here we discard signals: the program won't end even if we
						press Ctrl+C or we tell it to finish*/

	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the
						    terminal. TCSANOW tells the program not to wait
						    before making this change*/
}


/*The first thread for playing contains the clock, which is printed every second and pays attention to the indanger variable.*/
void *_clock(void * cq){
  time_t clk,ini,clkDanger,iniDanger;
  double remaining,remainingDanger;
	sc_rectangle *sc_clk;
	sc_clk = win_new(0,SCREENX+2,CLKY,CLKX,BGCOLOR,CHARCOLOR);
  ini = time(&ini);
  while(inDanger==0 && end==0) {
    noDanger: clk=time(&clk);
    remaining= TIME - difftime(clk,ini);
		interface_printClock(sc_clk,remaining,inDanger);
    usleep(1000000);
  }
	if(inDanger==1){
		iniDanger = time(&iniDanger);
	}
	while(inDanger==1 && end==0){
		clkDanger = time(&clkDanger);
		remainingDanger = DANGERTIME - difftime(clkDanger,iniDanger);
		interface_printClock(sc_clk,remainingDanger,inDanger);
		usleep(1000000);
	}
	if(end!=0) return NULL;
	goto noDanger;
}

void _intro(){
    sc_rectangle *intro;
    FILE * in;
    char *dialog=NULL, *ini, line[INTRO_LINE],choice, buff[MAX], *intropath = "intro.txt";
    int len,j=0,linenumber,x,y;

    intro = win_new(0,0,INTROY,INTROX,BGCOLOR,CHARCOLOR); /*Creating the intro window, which occupies all the screen.*/
    in = (FILE*) fopen(intropath, "r"); /*Openning the intro txt file which contains every dialog from the intro.*/

	if(!in) exit(-1);
    if (!intro) exit(-1);

	while(!feof(in)){
	  win_cls(intro);
		fscanf(in,"%d\t%d\t%[^\n]s",&x,&y,buff);
		linenumber=y;
		if(dialog) free(dialog);
		len = strlen(buff);
		dialog = (char*) malloc(len+1);
		strcpy(dialog,buff);
    ini=dialog;
    j=0;
    while(j<len && linenumber<INTROY){
      dialog = ini + j;
      memcpy(line,dialog,INTRO_LINE);
      win_write_line_at(intro,linenumber,x,line);
      j+=INTRO_LINE;
      linenumber++;
    }
		dialog = ini;
		choice = fgetc(stdin);
		while(choice!='a' && choice!='A' && choice!='\n'){
		    choice = fgetc(stdin);
		}
		if(choice=='\n'){
		    break;
		}
	}

	fclose(in);
	in = (FILE*) fopen("startmenu.txt", "r");
	win_cls(intro);
    while(!feof(in)){
		fscanf(in,"%d\t%d\t%[^\n]s",&x,&y,buff);
		len = strlen(buff);
		if(dialog) free(dialog);
		dialog = (char*) malloc(len+1);
		strcpy(dialog,buff);
    ini=dialog;
    j=0;
    while(j<len && linenumber<INTROY){
      dialog = ini + j;
      memcpy(line,dialog,INTRO_LINE);
      win_write_line_at(intro,y,x,line);
      j+=INTRO_LINE;
      linenumber++;
    }
		
    }
		choice = fgetc(stdin);
		while(choice!='q' && choice!='Q' && choice!='\n'){
		    choice = fgetc(stdin);
		}
		if(choice=='q' || choice == 'Q'){
		    exit(0);
		}
    fclose(in);
    win_delete(intro);
		if(dialog) free(dialog);
	return;
}

/*Once the game is finished, we call this function for calculating our score as well as closing the game restoring our terminal.*/
void GetEnd(Interact *interact){
	    sc_rectangle *intro;
	    FILE * in;
			Stat* stat;
	    char *dialog=NULL, *ini, line[INTRO_LINE],choice, buff[MAX], endpath[INTRO_LINE];
	    int len,j=0,linenumber,x,y,boxes,printers,meetings,motivation, chaos, nodanger, score, money;
			
			boxes = interact_getBoxes(interact);
			printers = interact_getPrinters(interact);
			meetings = interact_getMeetings(interact);
			stat = interact_getStat(interact);
			motivation = stat_get_motivation(stat);
			chaos = stat_get_chaos(stat);
			nodanger = -stat_get_nodanger(stat);
			money = stat_get_money(stat);
			
			if(end==2){
				strcpy(endpath,"endarrested.txt");
				goto endlabel;
			}
						
			if (boxes == 0 && printers == 0 && meetings == 0) {
					strcpy(endpath, "endsuicide.txt");
					goto endlabel;
			}
			if (boxes == 3 && printers == 3 && meetings == 3) {
					strcpy(endpath, "endbrussels.txt");
					goto endlabel;
			}
			if ((boxes + meetings + printers <= 10) || boxes<3 || meetings<3 || printers<3) {
					if((boxes==0 && meetings + printers > 6)|| (meetings==0 && boxes + printers > 6) || (printers==0 && boxes + meetings >6)) goto nosomething;
					strcpy(endpath, "endlose.txt");
					goto endlabel;
			}

			if (boxes == 5 && printers == 5 && meetings == 5) {
					strcpy(endpath, "endempire.txt");
					goto endlabel;
			}
			if ((boxes + meetings + printers >= 11) || boxes==0 || meetings==0 || printers==0) {
					nosomething: if (boxes == 0) {
							strcpy(endpath, "endnoboxes.txt");
							goto endlabel;
					}
					if (meetings == 0) {
							strcpy(endpath, "endnomeetings.txt");
							goto endlabel;
					}
					if (printers == 0) {
							strcpy(endpath, "endnoprinters.txt");
							goto endlabel;
					}
					strcpy(endpath, "endwin.txt");
					goto endlabel;
			}
			
			
	    endlabel: intro = win_new(0,0,INTROY,INTROX,BGCOLOR,CHARCOLOR); /*Creating the intro window, which occupies all the screen.*/
	    in = (FILE*) fopen(endpath, "r"); /*Openning the intro txt file which contains every dialog from the intro.*/
	
		if(!in) exit(-1);
	  if (!intro) exit(-1);
			win_cls(intro);
			usleep(1000000);
		while(!feof(in)){
			fscanf(in,"%d\t%d\t%[^\n]s",&x,&y,buff);
			if(dialog) free(dialog);
			len = strlen(buff);
			dialog = (char*) malloc(len+1);
			strcpy(dialog,buff);
	    win_write_line_at(intro,y,x,dialog);
	    j+=INTRO_LINE;
			choice = fgetc(stdin);
			while(choice!='a' && choice!='A' && choice!='\n'){
			    choice = fgetc(stdin);
			}
			if(choice=='\n'){
			    break;
			}
		}
	
		fclose(in);
		in = (FILE*) fopen("endmenu.txt", "r");
		win_cls(intro);
	    while(!feof(in)){
			fscanf(in,"%d\t%d\t%[^\n]s",&x,&y,buff);
			len = strlen(buff);
			if(dialog) free(dialog);
			dialog = (char*) malloc(len+1);
			strcpy(dialog,buff);
	    ini=dialog;
	    j=0;
	    while(j<len && linenumber<INTROY){
	      dialog = ini + j;
	      memcpy(line,dialog,INTRO_LINE);
	      win_write_line_at(intro,y,x,line);
	      j+=INTRO_LINE;
	      linenumber++;
	    }
			
	    }
			score = (int)(2.5*motivation + 2*chaos + 1.5*nodanger + money);
			sprintf(line,"YOUR SCORE: %d PuidgePoints",score);
			win_write_line_at(intro,33,15,line);
			choice = fgetc(stdin);
			while(choice!='\n'){
			    choice = fgetc(stdin);
			}
			if(choice=='\n'){
				fclose(in);
				win_delete(intro);
				if(dialog) free(dialog);
				tcsetattr(fileno(stdin), TCSANOW, &initial);
				exit(0);
			}
}

/*
  Plays the game. Pay attention to the interact_play - interface_play relation we have been talking in the previous files.
*/
void *_play(void * xq) {
Interact *interact;
Interface *interface;
time_t clk, ini, iniDanger, clkDanger;
double remaining,remainingDanger;
State currentState=NOTHING;
inDanger=0;
interact=interact_load(PATH);
interface=interface_ini();
interface_start(interact,interface);
ini = time(&ini);
while(inDanger==0) {
    noDangerPlay: clk = time(&clk);
    remaining= TIME - difftime(clk,ini);
		currentState=interact_play(currentState,interact);
		if(currentState==QUIT2 || remaining<=0){
			end = 1;
			GetEnd(interact);
		}
		inDanger=interact_getDanger(interact);
		interface_play(interact,interface,currentState);
		interact_updateClk(remaining,interact);
	}
	if(inDanger==1){
		iniDanger = time(&iniDanger);

		while(inDanger==1){
			clkDanger = time(&clkDanger);
			remainingDanger = DANGERTIME - difftime(clkDanger,iniDanger);
			currentState=interact_play(currentState,interact);
			if(currentState==QUIT2 || remainingDanger<=0) {
				if(remainingDanger<=0) end = 2;
				else end = 1;
				GetEnd(interact);
			}
			inDanger=interact_getDanger(interact);
			interface_play(interact,interface,currentState);
			interact_updateClk(remaining,interact);
		}

		goto noDangerPlay;
  }
}


int main(int argc, char **argv) {
  pthread_t key;
  pthread_t clk;

  _term_init();

	_intro();
	end = 0;
	pthread_create(&clk,NULL,_clock,NULL);

	_play((void *) key);


}
