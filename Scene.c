#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

Scene* createScene(char *fileName, ...) {
	Scene *scene = malloc(sizeof(Scene));
	if (scene == NULL) {
		printf("Failed To Allocate Memory For New Scene! ABORTING!");
		return NULL;
	}
	scene->header = malloc(sizeof(struct ObjectList));
	if (scene->header == NULL) {
		printf("Failed To Allocate Memory For New Object List! ABORTING!");
		return NULL;
	}
	scene->header->next = NULL;
	ObjectList *objList = scene->header;
	va_list allFile;
	va_start(allFile, fileName);
	char *currentFile = fileName;
	if (currentFile != NULL) {
		objList->object = createObject(currentFile);
		currentFile = va_arg(allFile, char*);
	}
	while (currentFile != NULL) {
		objList->next = malloc(sizeof(struct ObjectList));
		if (objList->next == NULL) {
			printf("Failed To Allocate Memory For New Object List! ABORTING!");
			return NULL;
		}
		objList = objList->next;
		objList->next = NULL;
		objList->object = createObject(currentFile);
		currentFile = va_arg(allFile, char*);
	}
	return scene;
}

Scene* loadScene(char *fileName, enum FileType type) {
	FILE *file;
	if (type == TextFormat) {
		file = fopen(fileName, "r");
	} else if (type == BinaryFormat) {
		file = fopen(fileName, "rb");
	}
	if (file == NULL) {
		printf("File Open Failed! ABORTING!");
		return NULL;
	}
	Scene *scene = malloc(sizeof(Scene));
	if (scene == NULL) {
		printf("Failed To Allocate Memory For New Scene! ABORTING!");
		return NULL;
	}
	scene->header = malloc(sizeof(struct ObjectList));
	if (scene->header == NULL) {
		printf(
				"Failed To Allocate Memory For New Scene Object List! ABORTING!");
		return NULL;
	}
	scene->header->next = NULL;
	ObjectList *objList = scene->header;
	ObjectList *preObjList = objList;
	while (!feof(file)) {
		objList->object = malloc(sizeof(Object));
		if (objList->object == NULL) {
			printf("Failed To Allocate Memory For New Object! ABORTING!");
			return NULL;
		}
		if (type == TextFormat) {
			loadObjectTxt(file, objList->object);
		} else if (type == BinaryFormat) {
			loadObjectBinary(file, objList->object);
		}
		objList->next = malloc(sizeof(struct ObjectList));
		if (objList->next == NULL) {
			printf("Failed To Allocate Memory For New Object List! ABORTING!");
			return NULL;
		}
		preObjList = objList;
		objList = objList->next;
		objList->next = NULL;
	}
	fclose(file);
	preObjList->next = NULL;
	return scene;
}

void saveScene(Scene *scene, char *fileName, enum FileType type) {
	FILE *file;
	if (type == TextFormat) {
		file = fopen(fileName, "w");
	} else if (type == BinaryFormat) {
		file = fopen(fileName, "wb");
	}
	if (file == NULL) {
		printf("File Open Failed! ABORTING!");
		return;
	}
	ObjectList *objList = scene->header;
	while (objList != NULL) {
		if (type == TextFormat) {
			saveObjectTxt(objList->object, file);
		} else if (type == BinaryFormat) {
			saveObjectBinary(objList->object, file);
		}
		objList = objList->next;
	}
	fclose(file);
}

void perform(Scene *scene, void (*func)(Object*, void*), char *type,
		char *string) {
	ObjectList *objList = scene->header;
	if (strcmp(type, "INT") == 0) {
		void *total = calloc(1, sizeof(int));
		*(int*) total = 0;
		while (objList != NULL) {
			func(objList->object, total);
			objList = objList->next;
		}
		printf("%s %d\n", string, *(int*) total);
		free(total);
	} else if (strcmp(type, "DOUBLE") == 0) {
		void *total = calloc(1, sizeof(double));
		*(double*) total = 0;
		while (objList != NULL) {
			func(objList->object, total);
			objList = objList->next;
		}
		printf("%s %lf\n", string, *(double*) total);
		free(total);
	} else {
		printf("%s is not a valid type", type);
	}
}

void freeScene(Scene *scene) {
	int i;
	while (scene->header != NULL) {
		ObjectList *objList = scene->header;
		free(scene->header->object->vertexes);
		for (i = 0; i < scene->header->object->numberOfFaces; ++i) {
			free(scene->header->object->faces[i].vertex);
		}
		free(scene->header->object->faces);
		scene->header = objList->next;
		free(objList);
	}
	free(scene);
}

