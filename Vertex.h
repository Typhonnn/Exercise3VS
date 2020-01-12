#pragma once
#include <stdio.h>

typedef struct {
	float x;	// Coordinates of Vertex
	float y;
	float z;
} Vertex;

//Takes a line that holds information about a Vertex and returns a defined Vertex.
Vertex* createVertex(char* line);

//Saves a Vertex to a txt file.
void saveVertexTxt(Vertex* vertex, FILE* file);
