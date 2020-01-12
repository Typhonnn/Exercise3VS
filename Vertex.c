#include "Vertex.h"
#include <stdio.h>
#include <stdlib.h>

//Takes a line that holds information about a Vertex and returns a defined Vertex.
Vertex* createVertex(char* line) {
	Vertex* vertex = malloc(sizeof(Vertex));
	if (vertex == NULL)
	{
		printf("Failed To Allocate Memory For Vertex! ABORTING!\n");
		return NULL;
	}
	sscanf_s(line, "%*c %f %f %f", &vertex->x, &vertex->y, &vertex->z);
	return vertex;
}

//Saves a Vertex to a txt file.
void saveVertexTxt(Vertex* vertex, FILE* file) {
	fprintf(file, "v  %f  %f  %f\n", vertex->x, vertex->y, vertex->z);
}
