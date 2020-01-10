#pragma once
#include <stdio.h>
#include "Face.h"
#include "Vertex.h"

typedef struct {
	// Array of all Vertexes
	int numberOfVertexes;
	Vertex *vertexes;
	// Array of all Faces
	int numberOfFaces;
	Face *faces;
} Object;

Object* createObject(char *filename);

void printFaces(Object *ptr, void *numberOfTriangularFaces);

void printVertexes(Object *ptr, void *numberOfVertexes);

void getTotalArea(Object *ptr, void *totalAreaOfTriangularFaces);

void saveObjectBinary(Object *object, FILE *file);

void saveObjectTxt(Object *object, FILE *file);

void loadObjectBinary(FILE *file, Object *object);

void loadObjectTxt(FILE *file, Object *object);

void transformObject(char *originalObjectFileName, char *deformedObjectFileName);
