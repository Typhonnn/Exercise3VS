#pragma once
#include <stdio.h>

typedef struct {
	float x;	// Coordinates of Vertex
	float y;
	float z;
} Vertex;

void saveVertexTxt(Vertex* vertex, FILE* file);

Vertex* createVertex(char* line);
