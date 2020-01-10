#pragma once
#include <stdio.h>

typedef struct {
	int size;		// Number of vertexes of the face
	int *vertex;	// Array of Ids of the vertexes
} Face;

void saveFaceTxt(Face *face, FILE *file);

void createFace(char *line, Face *face);
