#include "visualt-internals.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define ERROR_HEADER "VT map file loader error: "

typedef struct {
	uint32_t from;
	VTChar to;
} Tuple;

static Tuple *dct;
static int dctLength;

static int countLines(FILE *const file) {
	int lines = 0, c, pc = '\n';
	while((c = getc(file)) != EOF) {
		if(c == '\n') {
			lines++;
		}
		pc = c;
	}
	if(pc != '\n') {
		lines++;
	}
	if(ferror(file) != 0) {
		perror(ERROR_HEADER);
		return -1;
	}
	rewind(file);
	return lines;
}

static int binarySearch(int p, int q, uint32_t const k) {
	int pivot = (p+q)/2;

	if(k == dct[pivot].from) {
		return pivot;
	} else if(q-p <= 1) {
		return -1;
	} else if(k < dct[pivot].from) {
		return binarySearch(p, pivot, k);
	} else {
		return binarySearch(pivot+1, q, k);
	}
}

void freeDictionary() {
	free(dct);
	dctLength = 0;
}

VTChar translate(uint32_t from) {
	int i = binarySearch(0, dctLength, from);
	if(i < 0) {
		return 0;
	} else {
		return dct[i].to;
	}
}

int loadDictionary(FILE *const mapFile) {
	int error = false;
	dctLength = countLines(mapFile);
	if(dctLength <= 0) {
		flagErrorAndGoto(closeFile);
	}
	dct = malloc((unsigned)dctLength*sizeof(Tuple));
	for(int i = 0; i < dctLength; i++) {
		if(fscanf(mapFile, "%"SCNu32" %"SCNu32, &dct[i].from, &dct[i].to) < 2) { // NOLINT(cert-err34-c)
			perror(ERROR_HEADER);
			flagErrorAndGoto(freeDictionary);
		}
		dct[i].to = codeToVTChar(dct[i].to);
		int c;
		do {
			c = fgetc(mapFile);
		} while(c != '\n' && c != EOF);
		if(c == EOF && ferror(mapFile)) {
			perror(ERROR_HEADER);
			flagErrorAndGoto(freeDictionary);
		}
	}
	freeDictionary:
	if(error) {
		free(dct);
	}
	closeFile:
	if(fclose(mapFile) == EOF) {
		perror(ERROR_HEADER);
		flagErrorAndGoto(error);
	}
	error:
	if(error) {
		return -1;
	}
	return 0;
}
