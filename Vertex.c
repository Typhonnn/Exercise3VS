#include "Vertex.h"
#include <stdio.h>
#include <stdlib.h>

void saveVertexTxt(Vertex *vertex, FILE *file) {
	fprintf(file, "v  %f  %f  %f\n", vertex->x, vertex->y, vertex->z);
}

void createVertex(char *line, Vertex *vertex) {
	sscanf(line, "%*c %f %f %f", &vertex->x, &vertex->y, &vertex->z);
}
