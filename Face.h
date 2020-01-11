#pragma once
#include <stdio.h>

typedef struct {
	int size;		// Number of vertexes of the face
	int* vertex;	// Array of Ids of the vertexes
} Face;

//Saves a Face to txt file.
void saveFaceTxt(Face* face, FILE* file);

//This function takes the pointer to a Face and uses the string to define it.
void createFace(char* line, Face* face);
