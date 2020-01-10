#pragma once
#include "Object.h"

typedef struct ObjectList {
	Object *object;
	struct ObjectList *next;
} ObjectList;

typedef struct {
	struct ObjectList *header;
} Scene;

enum FileType {
	TextFormat, BinaryFormat
};

Scene* createScene(char *fileName, ...);

void saveScene(Scene *scene, char *fileName, enum FileType type);

Scene* loadScene(char *fileName, enum FileType type);

void freeScene(Scene *scene);

void perform(Scene *scene, void (*func)(Object*, void*), char *type,
		char *string);
