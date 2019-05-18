//Cavasin Riccardo

#include<stdio.h>
#include"../../VisualT/visualt.h"

void initialize(){
	spriten(ind(0),2);
	sety(ind(1),-1);
	sety(ind(2),-1);
	setext(ind(3),"Window B");
	sety(ind(3),5);

	spriten(ind(4),2);
	sety(ind(5),-1);
	sety(ind(6),-1);
	setext(ind(7),"Window A");
	sety(ind(7),5);

	setext("desc","|arrow keys| to move the top window, |wasd| to move the bottom window\n\v\v\v\v\v|space| to bring a window on the first layer and |x| to exit");
	sety("desc",9);
}

int main(int argv, char**argc){
	int i,t=1;
	libload();

	load("obj/wndow.obj");//the window frame
	load("obj/bck.obj");//the background
	load("obj/plane.obj");//the flying plane
	text("title");//the window's title
	//here i load the Window B's components

	load("obj/wndow.obj");//the window frame
	load("obj/bck.obj");//the background
	load("obj/plane.obj");//the flying plane
	text("title");//the window's title
	//here i load the Window A's components

	text("desc");
	//here i create the textbox that contains the instructions

	grp w1=group(4,0,1,2,3);//window B
	grp w2=group(4,4,5,6,7);//window A
	//here i make two groups: one for the first window and one for the second

	initialize();//here i run the initialize() functions
	setstage(100,20);//here i make the stage
	pensize(6);

	gamemode(1);

	do{
		draw();
		spritenext(ind(2));
		spriteprec(ind(6));
		i=keyhit(0);
		switch(i){
			case ' ':
				if(t){
					for(w1.i=0;w1.i<w1.n;w1.i++)
						layertop(ind(w1.v[w1.i]));
				}
				else{
					for(w2.i=0;w2.i<w2.n;w2.i++)
						layertop(ind(w2.v[w2.i]));
				}
				t=!t;
				layertop("desc");
			break;

			case 'C':
				if(t){
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chx(ind(w2.v[w2.i]),1);
				}
				else{
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chx(ind(w1.v[w1.i]),1);
				}
			break;
			case 'D':
				if(t){
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chx(ind(w2.v[w2.i]),-1);
				}
				else{
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chx(ind(w1.v[w1.i]),-1);
				}
			break;
			case 'A':
				if(t){
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chy(ind(w2.v[w2.i]),1);
				}
				else{
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chy(ind(w1.v[w1.i]),1);
				}
			break;
			case 'B':
				if(t){
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chy(ind(w2.v[w2.i]),-1);
				}
				else{
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chy(ind(w1.v[w1.i]),-1);
				}
			break;

			case 'd':
				if(t){
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chx(ind(w1.v[w1.i]),1);
				}
				else{
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chx(ind(w2.v[w2.i]),1);
				}
			break;
			case 'a':
				if(t){
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chx(ind(w1.v[w1.i]),-1);
				}
				else{
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chx(ind(w2.v[w2.i]),-1);
				}
			break;
			case 'w':
				if(t){
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chy(ind(w1.v[w1.i]),1);
				 }
				 else{
					 for(w2.i=0;w2.i<w2.n;w2.i++)
						chy(ind(w2.v[w2.i]),1);
				 }
			break;
			case 's':
				if(t){
					for(w1.i=0;w1.i<w1.n;w1.i++)
						chy(ind(w1.v[w1.i]),-1);
				}
				else{
					for(w2.i=0;w2.i<w2.n;w2.i++)
						chy(ind(w2.v[w2.i]),-1);
				}
			break;
		}
		sleepm(70);
	}while(i!='x');

	gamemode(0);

	for(w1.i=0;w1.i<w1.n;w1.i++)
		hide(ind(w1.v[w1.i]));
	for(w2.i=0;w2.i<w2.n;w2.i++)
		hide(ind(w2.v[w2.i]));
	setext("desc","\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\v\vThank You!\nstay in touch for more tutorials and demo projects");
	move("desc",0,0);
	draw();

	ungroup(&w1);
	ungroup(&w2);
	libunload();
return 0;
}
