//Cavasin Riccardo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	-64	enable x64 compile mode
	-o	disable gcc optimisations
	-e	disable editor compilation
	-l	disable library compilation
	-a	disable archive creation
	-d	disable object file deletion
*/

int i, x64= 0, optimize= 1, editor= 1, library= 1, archive= 1, delete= 1;
char c[50];

void todo() {
  if(x64)
    printf("(compiling for x64)\n");
  else
    printf("(compiling for x86)\n");
  if(editor) {
    printf("{ editor.c\tto editor");
    if(x64)
      printf("64 }\n");
    else
      printf("32 }\n");
  }
  if(library)
    printf("{ visualt.c\tto visualt.o }\n");
  if(archive) {
    printf("{ visualt.o\tto visualt");
    if(x64)
      printf("64.a }\n");
    else
      printf("32.a }\n");
  }
  if(delete)
    printf("( delete visualt.o )\n");
  if(optimize)
    printf("( optimize -O2 )");
  else
    printf("( no optimizations )");
}
void dodo() {
  printf("\n\n( deleting old editor32, editor64, libvisualt32.a and libvisualt64.a )\n");
  strcpy(c, "rm -f editor32 editor64 libvisualt32.a libvisualt64.a");
  printf("{ %s }\n", c);
  system(c);
  printf("( done )");

  if(editor) {
    printf("\n\n( compiling editor.c )\n");
    strcpy(c, "gcc");
    if(optimize)
      strcat(c, " -O2");
    strcat(c, " -o editor");
    if(x64)
      strcat(c, "64");
    else
      strcat(c, "32");
    strcat(c, " ../editor.c");
    printf("{ %s }\n", c);
    system(c);
    printf("( done )");
  }

  if(library) {
    printf("\n\n( compiling visualt.c )\n");
    strcpy(c, "gcc");
    if(optimize)
      strcat(c, " -O2");
    strcat(c, " -c ../visualt.c");
    printf("{ %s }\n", c);
    system(c);
    printf("( done )");
  }

  if(archive) {
    printf("\n\n( building library )\n");
    strcpy(c, "ar rcs libvisualt");
    if(x64)
      strcat(c, "64");
    else
      strcat(c, "32");
    strcat(c, ".a visualt.o");
    printf("{ %s }\n", c);
    system(c);
    printf("( done )");
  }

  if(delete) {
    printf("\n\n( deleting visualt.o )\n");
    strcpy(c, "rm -f visualt.o");
    printf("{ %s }\n", c);
    system(c);
    printf("( done )");
  }
}

int main(int argc, char** argv) {
  for(i= 1; i < argc && strcmp(argv[i], "-64"); i++)
    ;
  if(i != argc)
    x64= 1;
  for(i= 1; i < argc && strcmp(argv[i], "-o"); i++)
    ;
  if(i != argc)
    optimize= 0;
  for(i= 1; i < argc && strcmp(argv[i], "-e"); i++)
    ;
  if(i != argc)
    editor= 0;
  for(i= 1; i < argc && strcmp(argv[i], "-l"); i++)
    ;
  if(i != argc)
    library= 0;
  for(i= 1; i < argc && strcmp(argv[i], "-a"); i++)
    ;
  if(i != argc)
    archive= 0;
  for(i= 1; i < argc && strcmp(argv[i], "-d"); i++)
    ;
  if(i != argc)
    delete= 0;

  printf("[ LAUNCHED ]\n\n");
  todo();
  getchar();
  dodo();
  printf("\n\n[ END ]\n");
  return 0;
}