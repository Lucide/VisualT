#include <visualt/visualt.h>
#include <stdio.h>
#include <stdalign.h>

typedef struct Canvas Canvas;
typedef struct Obj Obj;

int main(int argv, char **argc) {
	// for(unsigned int y=0; y<height;y++) {
	//   for(unsigned int x = 0; x < width; x++) {
	//     printf("%08" PRIx32 "(%s) ", chars[x+y*width],chars[x+y*width]);
	//   }
	//   putchar('\n');
	// }

	printf("sizeof(void*)=%lu alignof(void*)=%lu\n", sizeof(void *), alignof(void *));
	printf("sizeof(short)=%lu alignof(short)=%lu\n", sizeof(short), alignof(short));
	printf("sizeof(int)=%lu alignof(int)=%lu\n", sizeof(int), alignof(int));
	printf("sizeof(bool)=%lu alignof(bool)=%lu\n", sizeof(bool), alignof(bool));
	putchar('\n');
	printf("sizeof(CharMap)=%lu alignof(CharMap)=%lu\n", sizeof(struct CharMap), alignof(struct CharMap));
	printf("sizeof(Obj)=%lu alignof(Obj)=%lu\n", sizeof(struct Obj), alignof(struct Obj));
	printf("sizeof(Canvas)=%lu alignof(Canvas)=%lu\n", sizeof(struct Canvas), alignof(struct Canvas));

	getchar();
}

