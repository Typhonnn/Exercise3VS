#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

errno_t err; //ptr for checking NULL

//This is a variadic function that takes a string of file names and creates a scene with objects in it.
Scene* createScene(char* fileName, ...) {
	Scene* scene = malloc(sizeof(Scene));
	if (scene == NULL) {
		printf("Failed To Allocate Memory For New Scene! ABORTING!\n");
		return NULL;
	}
	scene->header = malloc(sizeof(struct ObjectList));
	if (scene->header == NULL) {
		printf("Failed To Allocate Memory For New Object List! ABORTING!\n");
		return NULL;
	}
	scene->numOfObjects = 0;
	scene->header->next = NULL;
	ObjectList* objList = scene->header;
	va_list allFile;
	va_start(allFile, fileName);
	char* currentFile = fileName;
	if (currentFile != NULL) { //Due to ObjectList->next logic in while. We have this condition for the first file.
		objList->object = createObject(currentFile);
		currentFile = va_arg(allFile, char*);
		scene->numOfObjects++;
	}
	while (currentFile != NULL) {
		objList->next = malloc(sizeof(struct ObjectList));
		if (objList->next == NULL) {
			printf("Failed To Allocate Memory For New Object List! ABORTING!\n");
			return NULL;
		}
		objList = objList->next;
		objList->next = NULL;
		objList->object = createObject(currentFile);
		scene->numOfObjects++;
		currentFile = va_arg(allFile, char*);
	}
	return scene;
}

//This function takes a pointer to a function, a string the represent type to print, and performs the pointed function.
void perform(Scene* scene, void (*func)(Object*, void*), char* type,
	char* string) {
	if (scene == NULL || scene->header == NULL) {
		printf("No Object Found! ABORTING!\n");
		return;
	}
	ObjectList* objList = scene->header;
	if (strcmp(type, "INT") == 0) {
		int total = 0;
		while (objList != NULL) {
			if (objList->object == NULL) {
				objList = objList->next;
			}
			else {
				func(objList->object, &total);
				objList = objList->next;
			}
		}
		printf("%s %d\n", string, total);
	}
	else if (strcmp(type, "DOUBLE") == 0) {
		double total = 0;
		while (objList != NULL) {
			if (objList->object == NULL) {
				objList = objList->next;
			}
			else {
				func(objList->object, &total);
				objList = objList->next;
			}
		}
		printf("%s %lf\n", string, total);
	}
	else {
		printf("%s is not a valid type\n", type);
	}
}

//This function takes an existing scene with objects, file name, and file type and saves it to a file. 
void saveScene(Scene* scene, char* fileName, enum FileType type) {
	FILE* file = NULL;
	if (scene->header->object == NULL) {
		printf("No Object Found! ABORTING!\n");
		return;
	}
	if (type == TextFormat) {
		err = fopen_s(&file,fileName, "w");
	}
	else if (type == BinaryFormat) {
		err = fopen_s(&file,fileName, "wb");
	}
	if (err != 0) {
		printf("File Open Failed! ABORTING!\n");
		return;
	}
	if (type == BinaryFormat) {
		fwrite(&(scene->numOfObjects), sizeof(int), 1, file);
	}
	ObjectList* objList = scene->header;
	while (objList != NULL) {
		if (type == TextFormat) {
			saveObjectTxt(objList->object, file);
		}
		else if (type == BinaryFormat) {
			saveObjectBinary(objList->object, file);
		}
		objList = objList->next;
	}
	if (file) {
		err = fclose(file);
	}
	
}

//This function takes a file name and defines a file type (TXT / Binary) and loads a scene from it.
Scene* loadScene(char* fileName, enum FileType type) {
	int counter = 0;
	FILE* file = NULL;
	if (type == TextFormat) {
		err = fopen_s(&file,fileName, "r");
	}
	else if (type == BinaryFormat) {
		err = fopen_s(&file,fileName, "rb");
	}
	if (err != 0) {
		printf("File Open Failed! ABORTING!\n");
		return NULL;
	}
	Scene* scene = malloc(sizeof(Scene));
	if (scene == NULL) {
		printf("Failed To Allocate Memory For New Scene! ABORTING!\n");
		return NULL;
	}
	scene->header = malloc(sizeof(struct ObjectList));
	if (scene->header == NULL) {
		printf(
			"Failed To Allocate Memory For New Scene Object List! ABORTING!\n");
		return NULL;
	}
	scene->numOfObjects = 1; //This is to allow txt file to run the loop.
	if (!(feof(file)) && type == BinaryFormat) { //Due to the nature of loadObject, only the binary file needed the number of objects. 
		fread(&(scene->numOfObjects), sizeof(int), 1, file);
	}
	scene->header->next = NULL;
	ObjectList* objList = scene->header;
	ObjectList* preObjList = objList;
	while (!feof(file) && counter < scene->numOfObjects) {
		objList->object = malloc(sizeof(Object));
		if (objList->object == NULL) {
			printf("Failed To Allocate Memory For New Object! ABORTING!\n");
			return NULL;
		}
		if (type == TextFormat) {
			loadObjectTxt(file, objList->object);
		}
		else if (type == BinaryFormat) {
			loadObjectBinary(file, objList->object);
			counter++;
		}
		objList->next = malloc(sizeof(struct ObjectList));
		if (objList->next == NULL) {
			printf("Failed To Allocate Memory For New Object List! ABORTING!\n");
			return NULL;
		}
		preObjList = objList;
		objList = objList->next;
		objList->next = NULL;
	}
	if (file) {
		err = fclose(file);
	}
	preObjList->next = NULL;
	return scene;
}

//Frees the allocated memory a scene holds.
void freeScene(Scene* scene) {
	if (scene == NULL) {
		printf("Scene is NULL! Nothing To Free!\n");
		return;
	}
	int i;
	while (scene->header != NULL) {
		ObjectList* objList = scene->header;
		if (objList->object == NULL) {
			printf("Object is NULL! No Object To Free!\n");
			scene->header = objList->next;
		}
		else {
			free(scene->header->object->vertexes);
			for (i = 0; i < scene->header->object->numberOfFaces; ++i) {
				free(scene->header->object->faces[i].vertex);
			}
			free(scene->header->object->faces);
			scene->header = objList->next;
			free(objList);
		}
	}
	free(scene);
}
