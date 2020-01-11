#include "Vertex.h"
#include <stdio.h>
#include <stdlib.h>

void saveVertexTxt(Vertex* vertex, FILE* file) {
	fprintf(file, "v  %f  %f  %f\n", vertex->x, vertex->y, vertex->z);
}

Vertex* createVertex(char* line) {
	Vertex* vertex = malloc(sizeof(Vertex));
	if (vertex == NULL)
	{
		printf("Failed To Allocate Memory For Vertex! ABORTING!");
		return NULL;
	}
	sscanf_s(line, "%*c %f %f %f", &vertex->x, &vertex->y, &vertex->z);
	return vertex;
}
