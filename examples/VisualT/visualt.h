//Cavasin Riccardo

/*
	1. copy and paste " #include"visualt.h" " in you program source
	2. compile adding to your command " libvisualtxx.a " or " -l:libvisualtxx.a " or " -L. -lvisualtxx "
	(where xx stands for 32 or 64)
*/

#ifndef VISUALT_H
#define VISUALT_H

typedef _Bool bool;
#define true 1
#define false 0

struct sgrp{
	int *v,n,i;
};
typedef struct sgrp grp;

//----MISC----
void about();							//info about visualt
int getch(bool m);						//like getchar but without needing to press enter, m=0 to ignore the extra characters sent by the arrow keys
void sleepm(int milliseconds);			//sleep for n milliseconds
void freem(int **mna,int sx);			//to free a dynamic 2d array mna of sx width
//----POINTERS----
char* ind(int i);						//to refer to an object's INDex
grp group(int n,...);					//to group "n" objects by passing their index
void ungroup(grp *g);					//to delete the data contained on a group; now you can reuse the group
//----GETTERS----
int gon();								//to Get the Number of Objects loaded
int goind(char c[]);					//to Get the Object's INDex
char* goname(char c[]);					//to Get the Object's NAME
int gosn(char c[]);						//to Get the Object's Sprite Number
int gosx(char c[]);						//to Get the Object's Sprite X size
int gosy(char c[]);						//to Get the Object's Sprite Y size
//----INITIALIZATION----
void libload();							//REQUIRED to initialize the library
int load(char c[]);						//to load an object
int text(char c[]);						//to create a dynamic textbox
int clone(char c[]);					//to clone (reload) the object
void libunload();						//to free all the memory allocated by the library
//----STAGE----
int gstw();								//to Get the STage Width
int gsth();								//to Get the STage Height
void setstage(int x,int y);				//to create/size a new stage
void stageborder(bool m);				//to set the stage border visibility (enabled by default, for developers)
//----REFRESH----
void draw();							//to render the scene
void fdraw(char c[]);					//to render to file
void cdraw(char c[],int i);				//to render to a printf() sequence, waiting for i milliseconds
void override(int x,int y);				//to move the cursor to a certain x y
//----TEXT----
void setext(char c[],char n[]);			//to change a textbox's content, works for every object
//----LOOK----
int gsn(char c[]);						//to Get the object's current Sprite Number
int gvisibility(char c[]);				//to Get the object's VISIBILITY state
int glayer(char c[]);					//to Get the object's LAYER
void show(char c[]);					//to show the object
void hide(char c[]);					//to hide the object
void visible(char c[],bool m);			//to set the object's visibility (1/0)
void layertop(char c[]);				//to move the object to the top layer
void layerlow(char c[]);				//to move the object to the bottom layer
void layerup(char c[]);					//to move the object up a layer
void layerdown(char c[]);				//to move the object down a layer
void spritenext(char c[]);				//to switch to the object's next sprite
void spriteprec(char c[]);				//to switch to the object's precedent sprite
void spriten(char c[],int i);			//to switch to the object's n sprite
//----PEN----
int gpen(char c[]);						//to Get the object's PEN state
int gpench();							//to Get the PEN CHaracter in use
int gpensize();							//to Get the PEN SIZE
void pensize(int i);					//to set the pen size
void pench(int i);						//to set the pen character
void penup(char c[]);					//to disable the object's pen
void pendown(char c[]);					//to enable the object's pen
void setpen(char c[],bool m);			//to set the object's pen (1/0)
void stamp(char c[]);					//to stamp the object on the pen layer
void shift(int m);						//to shift the pen layer by m direction m=0:left m=1:right m=2:top m=3:down
void fill(int i);						//to set the pen layer to a character
void penclear();						//to clear the pen layer
//----MOVE----
int gox(char c[]);						//to Get the object's x position
int goy(char c[]);						//to Get the object's y position
int otouching(char c[],char d[]);		//to know if two Objects are touching  (d=0 to check for any object)
int ctouching(char c[],char d);			//to know if the object is touching a Character (d=0 to check for border)
void move(char c[],int x,int y);		//to move the object to x y
void setx(char c[],int x);				//to move the object to x
void sety(char c[],int y);				//to move the object to y
void chx(char c[],int x);				//to change the object's x position by x
void chy(char c[],int y);				//to change the object's y position by y
void textalize(char c[],int m);			//to move the object's angle to his actual position (useful with textboxes) m=0:top-left m=1:top-right m=2:bottom-left m=3:bottom-right
//----GAMEMODE----
void gamemode(bool m);					//to activate/deactivate the gamemode (do delay input) REQUIRED to use keyhit() and kbhit()
int keyhit(bool m);						//to read a key pressed in gamemode, returns 0 if no key has been pressed, m does the same as getch(bool m)
int kbhit();							//a more advanced use of gamemode, returns non-zero value if user input is trigger, else returns 0
//----DEBUG----
void debug_global();					//to show your program general status
void debug_all();						//to show the status of all the loaded objects
void debug_obj(char c[]);				//to show the oject's status
void debug_axes();						//to draw x and y axes

//----INSERT YOUR CUSTOM FUNCTIONS BELOW HERE----


#endif
