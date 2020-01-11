#pragma once
#include "Object.h"

typedef struct ObjectList {
	Object* object; // pointer for the object in the node.
	struct ObjectList* next; // pointer for the next node.
} ObjectList;

typedef struct {
	int numOfObjects; // counter for the number of object, used mainly in sceneSaveBinary
	struct ObjectList* header; //A header for the first member in a linked list.
} Scene;

enum FileType {
	TextFormat, BinaryFormat
};

//This is a variadic function that takes a string of file names and creates a scene with objects in it.
Scene* createScene(char* fileName, ...);

//This function takes an existing scene with objects, file name, and file type and saves it to a file. 
void saveScene(Scene* scene, char* fileName, enum FileType type);

//This function takes a file name and defines a file type (TXT / Binary) and loads a scene from it.
Scene* loadScene(char* fileName, enum FileType type);

//Frees the allocated memory a scene holds.
void freeScene(Scene* scene);

//This function takes a pointer to a function, a string the represent type to print, and performs the pointed function.
void perform(Scene* scene, void (*func)(Object*, void*), char* type,
	char* string);
