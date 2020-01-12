#include "Face.h"
#include <string.h>
#include <stdlib.h>

//This function takes the pointer to a Face and uses the string to define it.
void createFace(char* line, Face* face) {
	face->vertex = calloc(1, sizeof(int));
	if (face->vertex == NULL) {
		printf("Failed To Allocate Memory For Face! ABORTING!\n");
		return;
	}
	int* vertexes = face->vertex;
	char* delimiters = "f ";
	char* nextSplitLine = NULL;
	char* splitLine = strtok_s(line, delimiters, &nextSplitLine);
	face->size = 0;
	while (splitLine != NULL) {
		int size = face->size + 1;
		face->vertex = realloc(vertexes, size * sizeof(int));
		if (face->vertex == NULL) {
			printf("Failed To Reallocate Memory For New vertexes! ABORTING!\n");
			return;
		}
		vertexes = face->vertex;
		face->vertex[face->size++] = atoi(splitLine);
		splitLine = strtok_s(NULL, delimiters, &nextSplitLine);
	}
}

//Saves a Face to txt file.
void saveFaceTxt(Face* face, FILE* file) {
	int i;
	fprintf(file, "f ");
	for (i = 0; i < face->size; ++i) {
		fprintf(file, " %d", face->vertex[i]);
	}
	fprintf(file, "\n");
}
