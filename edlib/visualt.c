//Cavasin Riccardo

#include"visualt.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<time.h>
#include<stdarg.h>

struct ssprite{
	int **mna,sx,sy;
};
typedef struct ssprite sprite;

struct sobj{
	int n,px,py;
	bool r,e;
	char name[6];
	sprite *s,*p;
};
typedef struct sobj obj;

struct sscream{
	int **mna,**mne,*l,sx,sy,n,es,ec;
	bool bd;
	obj *v;
	sprite e[6];
};
typedef struct sscream scream;

scream sing;

//----INTERNALs----
void visualt_print(int **mna,int sx,int sy){
	int x,y;

	printf("┌");
	for(x=0;x<sx-1;x++)
		printf("─");
	printf("─┐\n");
	for(y=0;y<sy;y++){
		printf("│");
		for(x=0;x<sx;x++){
			switch(mna[x][y]){
				case -1:
					printf("◊");
				break;
				case 0:
					printf("×");
				break;
				case 1:
					printf("│");
				break;
				case 2:
					printf("─");
				break;
				case 3:
					printf("┌");
				break;
				case 4:
					printf("┐");
				break;
				case 5:
					printf("└");
				break;
				case 6:
					printf("┘");
				break;
				case 7:
					printf("░");
				break;
				case 8:
					printf("▒");
				break;
				case 9:
					printf("▓");
				break;
				default:
					putchar(mna[x][y]);
				break;
			}
		}
		printf("│\n");
	}
	printf("└");
	for(x=0;x<sx-1;x++)
		printf("─");
	printf("─┘\n");
}
int visualt_pointer(char c[]){
	int i,b;

	if(c[0])
		for(i=0;strcmp(c,sing.v[i].name);i++);
	else{
		for(i=0,b=1;c[b];b++)
			i+=c[b];
		free(c);
	}
return i;
}
void visualt_mem(){
	int *l;
	obj *v;

	v=malloc(sizeof(obj)*(sing.n+1));
	memcpy(v,sing.v,sizeof(obj)*sing.n);
	free(sing.v);
	sing.v=v;
	l=malloc(sizeof(int)*(sing.n+1));
	memcpy(l,sing.l,sizeof(int)*sing.n);
	l[sing.n]=sing.n;
	free(sing.l);
	sing.l=l;
}
void visualt_line(int x0,int y0,int x1,int y1){
	int x,y,px,py;
	int dx=abs(x1-x0),sx=x0<x1?1:-1;
	int dy=abs(y1-y0),sy=y0<y1?1:-1;
	int err=(dx>dy?dx:-dy)/2,e2;
	sprite *s=&sing.e[sing.es-1];

	while(1){
		px=x0-s->sx/2+sing.sx/2;
		py=-y0-s->sy/2+sing.sy/2;
		for(y=0;y<s->sy;y++){
			for(x=0;x<s->sx;x++){
				if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
					sing.mne[px+x][py+y]=s->mna[x][y];
			}
		}
		if(x0==x1&&y0==y1)
			break;
		e2=err;
		if(e2>-dx){
			err-=dy;
			x0+=sx;
		}
		if(e2<dy){
			err+=dx;
			y0+=sy;
		}
	}
}
//----MISC----
void about(){
	printf("\033c                 .:Visual T:.\n            ver ");
	printf("1.0  8 March 2017");
	printf("\n           Coperight (c) 2016-2017\n\n   An ASCII graphic library created by Mr. Fox,\n read the header file for additional information\n               Thanks to: KayJay\n\n             \"insert website here\"\n\n\n              In Memory of Simba\n\n                      ");
	getch(0);
}
int getch(bool m){
	struct termios old,new;
	int c;

	tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	c=getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&old);
	if(!m&&c=='\033'){
		getchar();
		c=getchar();
	}
return c;
}
void sleepm(int milliseconds){
	clock_t time_end;
	time_end=clock()+milliseconds*CLOCKS_PER_SEC/1000;
	while(clock()<time_end);
}
void freem(int **mna,int sx){
	for(sx--;sx>-1;free(mna[sx]),sx--);
	free(mna);
}
//----POINTERS----
char* ind(int i){
	int b,j=i/127;
	char *t=malloc(sizeof(char)*(j+3));
	for(b=1;b<=j;b++)
		t[b]=127;
	t[0]=0;
	t[b]=i%127;
	t[b+1]=0;
return t;
}
grp group(int n,...){
	int i;
	grp g;
	va_list arg;

	g.v=malloc(sizeof(int)*n);
	va_start(arg,n);
	for(i=0;i<n;i++)
		g.v[i]=va_arg(arg,int);
	va_end(arg);
	g.n=n;
	g.i=0;
return g;
}
void ungroup(grp *g){
	free(g->v);
	g->n=0;
	g->i=0;
}
//----GETTERS----
int gon(){
return sing.n;
}
int goind(char c[]){
return visualt_pointer(c);
}
char* goname(char c[]){
return sing.v[visualt_pointer(c)].name;
}
int gosn(char c[]){
return sing.v[visualt_pointer(c)].n;
}
int gosx(char c[]){
return sing.v[visualt_pointer(c)].p->sx;
}
int gosy(char c[]){
return sing.v[visualt_pointer(c)].p->sy;
}
//----INITIALIZATION----
void libload(){
	int i,b,x,y,v[104]={1,1,35,2,2,35,35,35,35,4,3,0,35,35,0,35,35,35,35,0,35,35,0,5,3,0,35,35,35,0,35,35,35,35,35,0,35,35,35,0,7,4,0,0,35,35,35,0,0,35,35,35,35,35,35,35,35,35,35,35,35,35,35,0,0,35,35,35,0,0,8,4,0,35,35,35,35,35,35,0,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,0,35,35,35,35,35,35,0};
	sprite *s;

	for(i=0,b=0;b<6;b++){
		s=&sing.e[b];
		s->sx=v[i];
		s->sy=v[i+1];
		i+=2;
		s->mna=malloc(sizeof(int*)*s->sx);
		for(x=0;x<s->sx;x++)
			s->mna[x]=malloc(sizeof(int)*s->sy);
		for(y=0;y<s->sy;y++){
			for(x=0;x<s->sx;x++,i++)
				s->mna[x][y]=v[i];
		}
	}
	sing.bd=1;
	sing.es=1;
	sing.ec=35;
	/*sing.mna=NULL;
	sing.mne=NULL;
	sing.v=NULL;
	sing.l=NULL;
	sing.sx=0;
	sing.sy=0;
	sing.n=0;*/
}
int load(char c[]){
	int *v,x,y,i,b;
	obj *o;
	sprite *s;
	FILE *a;

	visualt_mem();
	o=&sing.v[sing.n];
	if(strchr(c,'.')){
		a=fopen(c,"r");
		for(i=0,x=0;(i=getc(a))!=-1;){
			if(i==32)
				x++;
		}
		rewind(a);
		v=malloc(sizeof(int)*x);
		for(i=0;i<x;i++)
			fscanf(a,"%d",&v[i]);
		fclose(a);
	}
	else{
		for(i=0,x=0;c[i];i++){
			if(c[i]==32)
				x++;
		}
		v=malloc(sizeof(int)*x);
		for(i=0,y=0;i<x;i++){
			sscanf(c+y,"%d",&v[i]);
			for(y++;c[y]!=32;y++);
		}
	}
	for(i=0;i<6;i++)
		o->name[i]=v[i];
	o->n=v[i];
	o->s=malloc(sizeof(sprite)*v[i]);
	for(i++,b=0;b<v[6];b++){
		s=&o->s[b];
		s->sx=v[i];
		s->sy=v[i+1];
		i+=2;
		s->mna=malloc(sizeof(int*)*s->sx);
		for(x=0;x<s->sx;x++)
			s->mna[x]=malloc(sizeof(int)*s->sy);
		for(y=0;y<s->sy;y++){
			for(x=0;x<s->sx;x++,i++)
				s->mna[x][y]=v[i];
		}

	}
	free(v);
//---------------------------------------
	o->px=0;
	o->py=0;
	o->r=1;
	o->e=0;
	o->p=o->s;
//---------------------------------------
	sing.n++;
return sing.n-1;
}
int text(char c[]){
	int i;
	obj *o;
	sprite *s;

	visualt_mem();
	o=&sing.v[sing.n];
	o->s=malloc(sizeof(sprite));
	s=o->s;
	for(i=0;c[i]&&i<5;i++)
		o->name[i]=c[i];
	s->sx=i;
	s->sy=1;
	for(;i<6;i++)
		o->name[i]=0;
	s->mna=malloc(sizeof(int*)*s->sx);
	for(i=0;i<s->sx;i++){
		s->mna[i]=malloc(sizeof(int));
		s->mna[i][0]=o->name[i];
	}
//---------------------------------------
	o->n=1;
	o->px=0;
	o->py=0;
	o->r=1;
	o->e=0;
	o->p=s;
//---------------------------------------
	sing.n++;
return sing.n-1;
}
int clone(char c[]){
	int i,x;
	obj *o1,*o2;
	sprite *s1,*s2;

	visualt_mem();
//---------------------------------------
	o1=&sing.v[visualt_pointer(c)];
	o2=&sing.v[sing.n];
	*o2=*o1;
	o2->s=malloc(sizeof(sprite)*o2->n);
//---------------------------------------
	for(i=0;i<o2->n;i++){
		s1=&o1->s[i];
		s2=&o2->s[i];
		*s2=*s1;
		s2->mna=malloc(sizeof(int*)*s2->sx);
		for(x=0;x<s2->sx;x++){
			s2->mna[x]=malloc(sizeof(int)*s2->sy);
			memcpy(s2->mna[x],s1->mna[x],sizeof(int)*s2->sy);
		}
	}
//---------------------------------------
	o2->p=&o2->s[o1->p-o1->s];
//---------------------------------------
	sing.n++;
return sing.n-1;
}
void libunload(){
	int i;
	obj *o;
	sprite *s;

	freem(sing.mna,sing.sx);
	freem(sing.mne,sing.sx);
	for(i=0;i<6;i++){
		s=&sing.e[i];
		freem(s->mna,s->sx);
	}
	for(sing.n--;sing.n>-1;sing.n--){
		o=&sing.v[sing.n];
		for(i=o->n-1;i>-1;i--){
			s=&o->s[i];
			freem(s->mna,s->sx);
		}
		free(s);
	}
	free(sing.v);
	free(sing.l);
}
//----STAGE----
int gstw(){
return sing.sx;
}
int gsth(){
return sing.sy;
}
void setstage(int x,int y){
	if(x>0&&y>0){
		freem(sing.mna,sing.sx);
		freem(sing.mne,sing.sx);
		sing.sx=x;
		sing.sy=y;
		sing.mna=malloc(sizeof(int*)*x);
		sing.mne=malloc(sizeof(int*)*x);
		for(x--;x>-1;x--){
				sing.mna[x]=malloc(sizeof(int)*y);
				sing.mne[x]=malloc(sizeof(int)*y);
		}
	}
	penclear();
}
void stageborder(bool m){
sing.bd=m;
}
//----REFRESH----
void draw(){
	int i,x,y,px,py;
	obj *o;
	sprite *s;

	printf("\033c");
	for(x=0;x<sing.sx;x++)
		memcpy(sing.mna[x],sing.mne[x],sizeof(int)*sing.sy);
	for(i=0;i<sing.n;i++){
		o=&sing.v[sing.l[i]];
		if(sing.v[i].r){
			s=o->p;
			px=o->px-s->sx/2+sing.sx/2;
			py=-o->py-s->sy/2+sing.sy/2;
			for(y=0;y<s->sy;y++){
				for(x=0;x<s->sx;x++){
					if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
						sing.mna[px+x][py+y]=s->mna[x][y];
				}
			}
		}
	}
	if(sing.bd){
		printf("┌");
		for(x=0;x<sing.sx-1;x++)
			printf("─");
		printf("─┐\n");
	}
	for(y=0;y<sing.sy;y++){
		if(sing.bd)
			printf("│");
		for(x=0;x<sing.sx;x++){
			switch(sing.mna[x][y]){
				case -1:
					printf("◊");
				break;
				case 0:
					printf("×");
				break;
				case 1:
					printf("│");
				break;
				case 2:
					printf("─");
				break;
				case 3:
					printf("┌");
				break;
				case 4:
					printf("┐");
				break;
				case 5:
					printf("└");
				break;
				case 6:
					printf("┘");
				break;
				case 7:
					printf("░");
				break;
				case 8:
					printf("▒");
				break;
				case 9:
					printf("▓");
				break;
				default:
					putchar(sing.mna[x][y]);
				break;
			}
		}
		if(sing.bd)
			printf("│");
		printf("\n");
	}
	if(sing.bd){
		printf("└");
		for(x=0;x<sing.sx-1;x++)
			printf("─");
		printf("─┘\n");
	}
}
void fdraw(char c[]){
	int i,x,y,px,py;
	obj *o;
	sprite *s;
	FILE *a;

	for(x=0;x<sing.sx;x++)
		memcpy(sing.mna[x],sing.mne[x],sizeof(int)*sing.sy);
	for(i=0;i<sing.n;i++){
		o=&sing.v[sing.l[i]];
		if(sing.v[i].r){
			s=o->p;
			px=o->px-s->sx/2+sing.sx/2;
			py=-o->py-s->sy/2+sing.sy/2;
			for(y=0;y<s->sy;y++){
				for(x=0;x<s->sx;x++){
					if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
						sing.mna[px+x][py+y]=s->mna[x][y];
				}
			}
		}
	}
	a=fopen(c,"a");
		if(sing.bd){
			fprintf(a,"┌");
			for(x=0;x<sing.sx-1;x++)
				fprintf(a,"─");
			fprintf(a,"─┐\n");
		}
		for(y=0;y<sing.sy;y++){
			if(sing.bd)
				fprintf(a,"│");
			for(x=0;x<sing.sx;x++){
				switch(sing.mna[x][y]){
					case -1:
						fprintf(a,"◊");
					break;
					case 0:
						fprintf(a,"×");
					break;
					case 1:
						fprintf(a,"│");
					break;
					case 2:
						fprintf(a,"─");
					break;
					case 3:
						fprintf(a,"┌");
					break;
					case 4:
						fprintf(a,"┐");
					break;
					case 5:
						fprintf(a,"└");
					break;
					case 6:
						fprintf(a,"┘");
					break;
					case 7:
						fprintf(a,"░");
					break;
					case 8:
						fprintf(a,"▒");
					break;
					case 9:
						fprintf(a,"▓");
					break;
					default:
						putc(sing.mna[x][y],a);
					break;
				}
			}
			if(sing.bd)
				fprintf(a,"│");
			fprintf(a,"\n");
		}
		if(sing.bd){
			fprintf(a,"└");
			for(x=0;x<sing.sx-1;x++)
				fprintf(a,"─");
			fprintf(a,"─┘\n");
		}
	fclose(a);

}
void cdraw(char c[],int i){
	int b,x,y,px,py;
	obj *o;
	sprite *s;
	FILE *a;

	for(x=0;x<sing.sx;x++)
		memcpy(sing.mna[x],sing.mne[x],sizeof(int)*sing.sy);
	for(b=0;b<sing.n;b++){
		o=&sing.v[sing.l[b]];
		if(sing.v[b].r){
			s=o->p;
			px=o->px-s->sx/2+sing.sx/2;
			py=-o->py-s->sy/2+sing.sy/2;
			for(y=0;y<s->sy;y++){
				for(x=0;x<s->sx;x++){
					if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
						sing.mna[px+x][py+y]=s->mna[x][y];
				}
			}
		}
	}
	a=fopen(c,"a");
		fprintf(a,"printf(\"\\033c");
		if(sing.bd){
			fprintf(a,"┌");
		for(x=0;x<sing.sx-1;x++)
				fprintf(a,"─");
			fprintf(a,"─┐\\n");
		}
		for(y=0;y<sing.sy;y++){
			if(sing.bd)
				fprintf(a,"│");
			for(x=0;x<sing.sx;x++){
				switch(sing.mna[x][y]){
					case -1:
						fprintf(a,"◊");
					break;
					case 0:
						fprintf(a,"×");
					break;
					case 1:
						fprintf(a,"│");
					break;
					case 2:
						fprintf(a,"─");
					break;
					case 3:
						fprintf(a,"┌");
					break;
					case 4:
						fprintf(a,"┐");
					break;
					case 5:
						fprintf(a,"└");
					break;
					case 6:
						fprintf(a,"┘");
					break;
					case 7:
						fprintf(a,"░");
					break;
					case 8:
						fprintf(a,"▒");
					break;
					case 9:
						fprintf(a,"▓");
					break;
					case 92:
						fprintf(a,"\\\\");
					break;
					case 34:
						fprintf(a,"\\\"");
					break;
					case 39:
						fprintf(a,"\\\'");
					break;
					case 37:
						fprintf(a,"%%%%");
					break;
					default:
						putc(sing.mna[x][y],a);
					break;
				}
			}
			if(sing.bd)
				fprintf(a,"│");
			fprintf(a,"\\n");
		}
		if(sing.bd){
			fprintf(a,"└");
			for(x=0;x<sing.sx-1;x++)
				fprintf(a,"─");
			fprintf(a,"─┘\\n");
		}
		if(i)
			fprintf(a,"\");\nsleepm(%d);\n",i);
		else
			fprintf(a,"\");\n");
	fclose(a);
}
void override(int x,int y){
	x+=sing.sx/2+1;
	y=-y+sing.sy/2+1;
	if(x>0&&x<=sing.sx&&y>0&&y<=sing.sy){
		if(sing.bd){
			x++;
			y++;
		}
		printf("\033[%d;%dH",y,x);
	}
}
//----TEXT----
void setext(char c[],char n[]){
	int x=0,y,i=0;
	obj *o=&sing.v[visualt_pointer(c)];
	sprite *s=o->p;

	freem(s->mna,s->sx);
	s->sx=0;
	s->sy=0;
	do{
		if(n[i]=='\n'||!n[i]){
			s->sy++;
			if(x>s->sx)
				s->sx=x;
			x=0;
		}
		else
			x++;
		i++;
	}while(n[i-1]);
	s->mna=malloc(sizeof(int*)*(s->sx));
	for(x=0;x<s->sx;x++)
		s->mna[x]=malloc(sizeof(int)*(s->sy));
	for(i=0,y=0;y<s->sy;y++){
		for(x=0;x<s->sx;x++,i++){
			if(n[i]=='\n'||!n[i]){
				while(x<s->sx){
					s->mna[x][y]=0;
					x++;
				}
			}
			else
				s->mna[x][y]=n[i];
			if(x==s->sx-1&&n[i+1]=='\n')
				i++;
		}
	}
	for(y=0;y<s->sy;y++){
		for(x=0;x<s->sx;x++){
			if(s->mna[x][y]==11)
				s->mna[x][y]=0;
		}
	}
}
//----LOOK----
int gsn(char c[]){
	obj *o=&sing.v[visualt_pointer(c)];
return o->p-o->s+1;
}
int gvisibility(char c[]){
return sing.v[visualt_pointer(c)].r;
}
int glayer(char c[]){
	int i=0,p=visualt_pointer(c);

	for(;sing.l[i]!=p;i++);
return i;
}
void show(char c[]){
	sing.v[visualt_pointer(c)].r=1;
}
void hide(char c[]){
	sing.v[visualt_pointer(c)].r=0;
}
void visible(char c[],bool m){
	sing.v[visualt_pointer(c)].r=m;
}
void layertop(char c[]){
	int i,p=visualt_pointer(c);

	for(i=0;sing.l[i]!=p;i++);
	p=sing.l[i];
	for(;i<sing.n-1;sing.l[i]=sing.l[i+1],i++);
	sing.l[sing.n-1]=p;
}
void layerlow(char c[]){
	int i,p=visualt_pointer(c);

	for(i=0;sing.l[i]!=p;i++);
	p=sing.l[i];
	for(;i>0;sing.l[i]=sing.l[i-1],i--);
	sing.l[0]=p;
}
void layerup(char c[]){
	int i,p=visualt_pointer(c);

	if(p<sing.n-1){
		for(i=0;sing.l[i]!=p;i++);
		p=sing.l[i];
		sing.l[i]=sing.l[i+1];
		sing.l[i+1]=p;
	}
}
void layerdown(char c[]){
	int i,p=visualt_pointer(c);

	if(p>0){
		for(i=0;sing.l[i]!=p;i++);
		p=sing.l[i];
		sing.l[i]=sing.l[i-1];
		sing.l[i-1]=p;
	}
}
void spritenext(char c[]){
	int p=visualt_pointer(c);
	obj *o=&sing.v[p];

	o->p=&o->s[(o->p-o->s+1)%o->n];
}
void spriteprec(char c[]){
	int p=visualt_pointer(c);
	obj *o=&sing.v[p];

	o->p=&o->s[(o->p-o->s+o->n-1)%o->n];
}
void spriten(char c[],int i){
	obj *o=&sing.v[visualt_pointer(c)];

	if(i)
		i--;
	if(i>-1&&i<o->n)
		o->p=&o->s[i];
}
//----PEN----
int gpen(char c[]){
return sing.v[visualt_pointer(c)].e;
}
int gpench(){
return sing.ec;
}
int gpensize(){
return sing.es;
}
void pensize(int i){
	if(i>0&&i<7)
		sing.es=i;
}
void pench(int i){
	int x,y,b;

	if(i){
		for(b=0;b<6;b++){
			for(y=0;y<sing.e[b].sy;y++){
				for(x=0;x<sing.e[b].sx;x++){
					if(sing.e[b].mna[x][y])
						sing.e[b].mna[x][y]=i;
				}
			}
		}
	}
}
void penup(char c[]){
	sing.v[visualt_pointer(c)].e=0;
}
void pendown(char c[]){
	sing.v[visualt_pointer(c)].e=1;
}
void setpen(char c[],bool m){
	sing.v[visualt_pointer(c)].e=m;
}
void stamp(char c[]){
	int x,y,px,py;
	obj *o=&sing.v[visualt_pointer(c)];
	sprite *s=o->p;

	px=o->px-s->sx/2+sing.sx/2;
	py=-o->py-s->sy/2+sing.sy/2;
	for(y=0;y<s->sy;y++){
		for(x=0;x<s->sx;x++){
			if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
				sing.mne[px+x][py+y]=s->mna[x][y];
		}
	}
}
void shift(int m){
	int x,y;

	switch(m){
		case 0:
			for(y=0;y<sing.sy;y++){
				for(x=0;x<sing.sx-1;x++)
					sing.mne[x][y]=sing.mne[x+1][y];
			}
			for(y=0;y<sing.sy;y++)
				sing.mne[sing.sx-1][y]=32;
		break;
		case 1:
			for(y=0;y<sing.sy;y++){
				for(x=sing.sx-1;x>0;x--)
					sing.mne[x][y]=sing.mne[x-1][y];
			}
			for(y=0;y<sing.sy;y++)
				sing.mne[0][y]=32;
		break;
		case 2:
			for(x=0;x<sing.sx;x++){
				for(y=0;y<sing.sy-1;y++)
					sing.mne[x][y]=sing.mne[x][y+1];
			}
			for(x=0;x<sing.sx;x++)
				sing.mne[x][sing.sy-1]=32;
		break;
		case 3:
			for(x=0;x<sing.sx;x++){
				for(y=sing.sy-1;y>0;y--)
					sing.mne[x][y]=sing.mne[x][y-1];
			}
			for(x=0;x<sing.sx;x++)
				sing.mne[x][0]=32;
		break;
	}
}
void fill(int i){
	int x;

	for(x=0;x<sing.sx;x++)
		memset(sing.mne[x],i,sizeof(int)*sing.sy);
}
void penclear(){
	int x;

	for(x=0;x<sing.sx;x++)
		memset(sing.mne[x],32,sizeof(int)*sing.sy);
}
//----MOVE----
int gox(char c[]){
return sing.v[visualt_pointer(c)].px;
}
int goy(char c[]){
return sing.v[visualt_pointer(c)].py;
}
int otouching(char c[],char d[]){
	int x,y,px,py,i=0;
	obj *o1=&sing.v[visualt_pointer(c)],*o2;
	sprite *s;

	if(o1->r){
		for(x=0;x<sing.sx;x++)
			memset(sing.mna[x],0,sizeof(int)*sing.sy);
		if(d){
			o2=&sing.v[visualt_pointer(d)];
			if(o2->r){
				s=o1->p;
				px=o1->px-s->sx/2+sing.sx/2;
				py=-o1->py-s->sy/2+sing.sy/2;
				for(y=0;y<s->sy;y++){
					for(x=0;x<s->sx;x++){
						if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
							sing.mna[px+x][py+y]=1;
					}
				}
				s=o2->p;
				px=o2->px-s->sx/2+sing.sx/2;
				py=-o2->py-s->sy/2+sing.sy/2;
				for(y=0;!i&&y<s->sy;y++){
					for(x=0;!i&&x<s->sx;x++){
						if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy&&sing.mna[px+x][py+y])
							i=1;
					}
				}
			}
		}
		else{
			for(i=0;i<sing.n;i++){
				o2=&sing.v[i];
				if(o2->r&&i!=o1-sing.v){
					s=o2->p;
					px=o2->px-s->sx/2+sing.sx/2;
					py=-o2->py-s->sy/2+sing.sy/2;
					for(y=0;y<s->sy;y++){
						for(x=0;x<s->sx;x++){
							if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
								sing.mna[px+x][py+y]=1;
						}
					}
				}
			}
			s=o1->p;
			px=o1->px-s->sx/2+sing.sx/2;
			py=-o1->py-s->sy/2+sing.sy/2;
			for(i=0,y=0;!i&&y<s->sy;y++){
				for(x=0;!i&&x<s->sx;x++){
					if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy&&sing.mna[px+x][py+y])
						i=1;
				}
			}
		}
	}
return i;
}
int ctouching(char c[],char d){
	int x,y,px,py,i;
	obj *o1=&sing.v[visualt_pointer(c)],*o2;
	sprite *s;

	if(o1->r){
		if(!d){
			s=o1->p;
			px=o1->px-s->sx/2+sing.sx/2;
			py=-o1->py-s->sy/2+sing.sy/2;
			for(i=0,y=0;!i&&y<s->sy;y++){
				for(x=0;!i&&x<s->sx;x++){
					if(s->mna[x][y]&&!(px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)){
						i=1;
					}
				}
			}
		}
		else{
			for(x=0;x<sing.sx;x++)
				memcpy(sing.mna[x],sing.mne[x],sizeof(int)*sing.sy);
			for(i=0;i<sing.n;i++){
				o2=&sing.v[sing.l[i]];
				if(o2->r&&sing.l[i]!=o1-sing.v){
					s=o2->p;
					px=o2->px-s->sx/2+sing.sx/2;
					py=-o2->py-s->sy/2+sing.sy/2;
					for(y=0;y<s->sy;y++){
						for(x=0;x<s->sx;x++){
							if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy)
								sing.mna[px+x][py+y]=s->mna[x][y];
						}
					}
				}
			}
			s=o1->p;
			px=o1->px-s->sx/2+sing.sx/2;
			py=-o1->py-s->sy/2+sing.sy/2;
			for(i=0,y=0;!i&&y<s->sy;y++){
				for(x=0;!i&&x<s->sx;x++){
					if(s->mna[x][y]&&px+x>-1&&px+x<sing.sx&&py+y>-1&&py+y<sing.sy&&sing.mna[px+x][py+y]==d){
						i=1;
					}
				}
			}
		}
	}
return i;
}
void move(char c[],int x,int y){
	obj *o=&sing.v[visualt_pointer(c)];

	if(o->e)
		visualt_line(o->px,o->py,x,y);
	o->px=x;
	o->py=y;
}
void setx(char c[],int x){
	obj *o=&sing.v[visualt_pointer(c)];

	if(o->e)
		visualt_line(o->px,o->py,x,o->py);
	o->px=x;
}
void sety(char c[],int y){
	obj *o=&sing.v[visualt_pointer(c)];

	if(o->e)
		visualt_line(o->px,o->py,o->px,y);
	o->py=y;
}
void chx(char c[],int x){
	obj *o=&sing.v[visualt_pointer(c)];

	if(o->e)
		visualt_line(o->px,o->py,o->px+x,o->py);
	o->px+=x;
}
void chy(char c[],int y){
	obj *o=&sing.v[visualt_pointer(c)];

	if(o->e)
		visualt_line(o->px,o->py,o->px,o->py+y);
	o->py+=y;
}
void textalize(char c[],int m){
	obj *o=&sing.v[visualt_pointer(c)];

	switch(m){
		case 0:
			o->px+=o->p->sx/2;
			o->py-=o->p->sy/2;
		break;
		case 1:
			o->px-=o->p->sx/2;
			o->py-=o->p->sy/2;
			if(!(o->p->sx%2))
				o->px++;
		break;
		case 2:
			o->px+=o->p->sx/2;
			o->py+=o->p->sy/2;
			if(!(o->p->sy%2))
				o->py--;
		break;
		case 3:
			o->px-=o->p->sx/2;
			o->py+=o->p->sy/2;
			if(!(o->p->sx%2))
				o->px--;
			if(!(o->p->sy%2))
				o->py--;
		break;
	}
}
//----GAMEMODE----
void gamemode(bool m){
	struct termios tty;

	tcgetattr(STDIN_FILENO,&tty);
	if(m){
		tty.c_lflag&=~(ICANON|ECHO);
		tty.c_cc[VMIN]=1;
	}
	else
		tty.c_lflag|=(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&tty);
}
int keyhit(bool m){
	int c=0;

	while(kbhit()){
		c=getchar();
		if(!m&&c=='\033'){
			getchar();
			c=getchar();
		}
	}
return c;
}
int kbhit(){
	struct timeval tv;
	fd_set fds;

	tv.tv_sec=0;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO,&fds);
	select(STDIN_FILENO+1,&fds,NULL,NULL,&tv);
return FD_ISSET(STDIN_FILENO,&fds);
}
//----DEBUG----
void debug_global(){
	int i;
	sprite *s;

	printf("[global debug launched]\n\nstage:\tsx=%d sy=%d border=%d\nobject:\tcounter=%d\npen:\tsize=%d ch=%d(%c)\nlayer:\t[",sing.sx,sing.sy,sing.bd,sing.n,sing.es,sing.ec,sing.ec);
	for(i=0;i<sing.n;i++)
		printf("%d,",sing.l[i]);
	printf("]\n");
	for(i=0;i<6;i++){
		s=&sing.e[i];
		visualt_print(s->mna,s->sx,s->sy);
	}
	printf("[end debug]\n");
}
void debug_all(){
	int i,b;
	obj *o;
	sprite *s;

	printf("[all debug launched]\n\n");
	for(i=0;i<sing.n;i++){
		o=&sing.v[i];
		printf("(object \"%s\" %d/%d)\nposition:\tx=%d y=%d\nflags:\t\tvisible=%d pen=%d\nsprite:\t\tcounter=%d\n",o->name,i+1,sing.n,o->px,o->py,o->r,o->e,o->n);
		for(b=0;b<o->n;b++){
			s=&o->s[b];
			printf("(\"%s\", sprite %d/%d)\nsize: sx=%d sy=%d\n",o->name,b+1,o->n,s->sx,s->sy);
			visualt_print(s->mna,s->sx,s->sy);
		}
		printf("\n");
	}
	printf("[end debug]\n");
}
void debug_obj(char c[]){
	int i;
	obj *o=&sing.v[visualt_pointer(c)];
	sprite *s;

	printf("[obj debug launched]\n\n");
		i=o-sing.v+1;
		printf("(object \"%s\" %d/%d)\nposition:\tx=%d y=%d\nflags:\t\tvisible=%d pen=%d\nsprite:\t\tcounter=%d\n",o->name,i,sing.n,o->px,o->py,o->r,o->e,o->n);
		for(i=0;i<o->n;i++){
			s=&o->s[i];
			printf("(\"%s\", sprite %d/%d)\nsize: sx=%d sy=%d\n",o->name,i+1,o->n,s->sx,s->sy);
			visualt_print(s->mna,s->sx,s->sy);
		}
		printf("\n");
	printf("[end debug]\n");
}
void debug_axes(){
	int i;

	if(sing.mne){
		for(i=0;i<sing.sx;i++)
			sing.mne[i][sing.sy/2]=45;
		for(i=0;i<sing.sy;i++)
			sing.mne[sing.sx/2][i]=124;
		sing.mne[0][sing.sy/2]=45;
		sing.mne[sing.sx/2][0]=43;
		sing.mne[sing.sx-1][sing.sy/2]=43;
		sing.mne[sing.sx/2][sing.sy-1]=45;
		sing.mne[sing.sx/2][sing.sy/2]=48;
	}
}

//int main(int argv, char**argc){
	//libload();

	//libunload();
//return 0;
//}
