
#include "Object.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define END_OBJECT "endObject"
#define VERTEX_POS "vertex positions\n"
#define FACE_POS "face positions\n"
#define MAX_LINE 300

errno_t err;

Object* createObject(char* filename) {
	FILE* file;
	err = fopen_s(&file, filename, "r");
	if (err != 0) {
		printf("Failed Opening File %s! Aborting!", filename);
		return NULL;
	}
	Object* object = malloc(sizeof(Object));
	if (object == NULL) {
		printf("Failed Allocating Memory For Object! ABORTING!");
		return NULL;
	}
	loadObjectTxt(file, object);
	if (file) {
		err = fclose(file);
	}
	return object;
}

void saveObjectBinary(Object* object, FILE* file) {
	int i;
	fwrite(&object->numberOfVertexes, sizeof(int), 1, file);
	fwrite(object->vertexes, sizeof(Vertex), object->numberOfVertexes, file);
	fwrite(&object->numberOfFaces, sizeof(int), 1, file);
	for (i = 0; i < object->numberOfFaces; ++i) {
		fwrite(&object->faces[i].size, sizeof(int), 1, file);
		fwrite(object->faces[i].vertex, sizeof(int), object->faces[i].size,
			file);
	}
}

void loadObjectBinary(FILE* file, Object* object) {
	int i;
	fread(&object->numberOfVertexes, sizeof(int), 1, file);
	object->vertexes = malloc(object->numberOfVertexes * sizeof(Vertex));
	if (object->vertexes == NULL) {
		printf("Failed To Allocate Memory For Vertexes! ABORTING!");
		return;
	}
	int numOfVertexes = object->numberOfVertexes;
	fread(object->vertexes, sizeof(Vertex), numOfVertexes, file);
	fread(&object->numberOfFaces, sizeof(int), 1, file);
	object->faces = malloc(object->numberOfFaces * sizeof(Face));

	if (object->faces == NULL) {
		printf("Failed To Allocate Memory For Faces! ABORTING!");
		return;
	}
	for (i = 0; i < object->numberOfFaces; ++i) {
		fread(&object->faces[i].size, sizeof(int), 1, file);
		object->faces[i].vertex = malloc(object->faces[i].size * sizeof(int));
		if (object->faces[i].vertex == NULL) {
			printf("Failed To Allocate Memory For Faces->Vertexes! ABORTING!");
			return;
		}
		fread(object->faces[i].vertex, sizeof(int), object->faces[i].size,
			file);
	}
}

void saveObjectTxt(Object* object, FILE* file) {
	int i;
	fprintf(file, "\n# %d %s", object->numberOfVertexes, VERTEX_POS);
	for (i = 0; i < object->numberOfVertexes; ++i) {
		saveVertexTxt(&object->vertexes[i], file);
	}
	fprintf(file, "\n# %d %s", object->numberOfFaces, FACE_POS);
	for (i = 0; i < object->numberOfFaces; ++i) {
		saveFaceTxt(&object->faces[i], file);
	}
	fprintf(file, "%s", END_OBJECT);
}

void loadObjectTxt(FILE* file, Object* object) {
	object->vertexes = malloc(sizeof(Vertex));
	if (object->vertexes == NULL) {
		printf("Failed To Allocate Memory For Vertexes! ABORTING!");
		return;
	}
	object->faces = malloc(sizeof(Face));
	if (object->faces == NULL) {
		printf("Failed To Allocate Memory For Faces! ABORTING!");
		return;
	}
	object->faces->vertex = calloc(3, sizeof(int));
	if (object->faces->vertex == NULL) {
		printf("Failed To Allocate Memory For Faces -> Vertexes! ABORTING!");
		return;
	}
	char line[MAX_LINE];
	Vertex* vertexes = object->vertexes;
	Face* faces = object->faces;
	object->numberOfVertexes = 0;
	object->numberOfFaces = 0;
	char* bytesRead = NULL;
	bytesRead = fgets(line, MAX_LINE, file);
	while (bytesRead != NULL && strstr(line, END_OBJECT) == NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			object->vertexes = realloc(vertexes,
				(object->numberOfVertexes + 1) * sizeof(Vertex));
			if (vertexes == NULL) {
				printf(
					"Failed To Reallocate Memory For New Vertexes! ABORTING!");
				return;
			}
			vertexes = object->vertexes;
			object->vertexes[object->numberOfVertexes++] = *createVertex(line);
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			object->faces = realloc(faces,
				(object->numberOfFaces + 1) * sizeof(Face));
			if (faces == NULL) {
				printf("Failed To Reallocate Memory For New Faces! ABORTING!");
				return;
			}
			faces = object->faces;
			createFace(line, &object->faces[object->numberOfFaces++]);
		}
		bytesRead = fgets(line, MAX_LINE, file);
	}
}

void getTotalArea(Object* ptr, void* totalAreaOfTriangularFaces) {
	int i;
	double s, a, b, c;
	for (i = 0; i < ptr->numberOfFaces; ++i) {
		if (ptr->faces[i].size == 3) {
			a =
				pow(
					pow(
					(ptr->vertexes[ptr->faces[i].vertex[0] - 1].x
						- ptr->vertexes[ptr->faces[i].vertex[1]	- 1].x), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[0]
						- 1].y
						- ptr->vertexes[ptr->faces[i].vertex[1]
						- 1].y), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[0]
						- 1].z
						- ptr->vertexes[ptr->faces[i].vertex[1]
						- 1].z), 2), 0.5);
			b =
				pow(
					pow(
					(ptr->vertexes[ptr->faces[i].vertex[0] - 1].x
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].x), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[0]
						- 1].y
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].y), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[0]
						- 1].z
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].z), 2), 0.5);
			c =
				pow(
					pow(
					(ptr->vertexes[ptr->faces[i].vertex[1] - 1].x
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].x), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[1]
						- 1].y
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].y), 2)
					+ pow(
					(ptr->vertexes[ptr->faces[i].vertex[1]
						- 1].z
						- ptr->vertexes[ptr->faces[i].vertex[2]
						- 1].z), 2), 0.5);
			s = (a + b + c) / 2;
			*(double*)totalAreaOfTriangularFaces += pow(
				s * (s - a) * (s - b) * (s - c), 0.5);
		}
	}
}

void printVertexes(Object* ptr, void* numberOfVertexes) {
	if (ptr->numberOfVertexes == NULL) {
		return;
	}
	int total = ptr->numberOfVertexes;
	*(int*)numberOfVertexes += total;
}

void printFaces(Object* ptr, void* numberOfTriangularFaces) {
	int i, counter = 0;
	for (i = 0; i < ptr->numberOfFaces; i++) {
		if (ptr->faces[i].size == 3) {
			counter++;
		}
	}
	*(int*)numberOfTriangularFaces += counter;
}

void transformObject(char* originalObjectFileName, char* deformedObjectFileName) {
	float x, y, z;
	FILE * orgFile, *defoFile;
	err	= fopen_s(&orgFile,originalObjectFileName, "r");
	if (err != 0) {
		printf("Failed Opening File %s! Aborting!", originalObjectFileName);
		return;
	}
	err = fopen_s(&defoFile,deformedObjectFileName, "w");
	if (err != 0) {
		printf("Failed Opening File %s! Aborting!", deformedObjectFileName);
		return;
	}
	char line[MAX_LINE];
	char* bytesRead = NULL;
	bytesRead = fgets(line, MAX_LINE, orgFile);
	while (bytesRead != NULL) {
		if (line[0] == 'v' && line[1] == ' ') {
			sscanf_s(line, "%*c %f %f %f", &x, &y, &z);
			x = 0.3 * x;
			fprintf(defoFile, "v %f %f %f\n", x, y, z);
		}
		else {
			fprintf(defoFile, "%s", line);
		}
		bytesRead = fgets(line, MAX_LINE, orgFile);
	}
	fclose(orgFile);
	fclose(defoFile);
	free(line);
}