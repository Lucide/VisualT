//Cavasin Riccardo

#include<stdio.h>
#include"../../VisualT/visualt.h"

int main(int argv, char**argc){
	int i;
	libload();
	load("obj/fcat.obj");
	text("t");

	setstage(100,30);
	setext("t","press something to make a cat land");
	getch(0);

	while(1){
		hide("fcat");
		show("t");
		draw();
		getch(0);
		hide("t");
		show("fcat");
		for(i=0;i<6;i++){
			draw();
			spritenext("fcat");
			sleepm(100);
		}
	}
	libunload();
return 0;
}
