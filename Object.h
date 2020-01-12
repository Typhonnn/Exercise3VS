#pragma once
#include <stdio.h>
#include "Face.h"
#include "Vertex.h"

typedef struct {
	// Array of all Vertexes
	int numberOfVertexes;
	Vertex* vertexes;
	// Array of all Faces
	int numberOfFaces;
	Face* faces;
} Object;

//Takes a file name for data on object and uses it to create and return an Object.
Object* createObject(char* filename);

//This method calculates the total area of triangular faces of an Object.
//It uses Heron's Formula to find the solution.
void getTotalArea(Object* ptr, void* totalAreaOfTriangularFaces);

//This function gives the number of vertexes of an Object.
void printVertexes(Object* ptr, void* numberOfVertexes);

//This function gives the number of faces with three vertexes.
void printFaces(Object* ptr, void* numberOfTriangularFaces);

//This function takes an object file and modifies it's Vertex coordinates.
void transformObject(char* originalObjectFileName, char* deformedObjectFileName);

//Saves an Object to a binary file.
void saveObjectBinary(Object* object, FILE* file);

//Loades an Object from a binary file.
void loadObjectBinary(FILE* file, Object* object);

//Saves an Object to a txt file.
void saveObjectTxt(Object* object, FILE* file);

//Loads an Object from a txt file.
void loadObjectTxt(FILE* file, Object* object);
