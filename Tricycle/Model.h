#ifndef MODEL_H
#define MODEL_H

#include "libobj.h"

typedef struct _Model {
	GLMgroup* group;
	GLfloat center[3];
} Model;

#endif