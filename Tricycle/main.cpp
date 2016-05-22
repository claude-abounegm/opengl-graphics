/*
* Claude Abounegm
* main.cpp
*/

#include "main.h"

int main(int argc, char** argv)
{
	model = glmReadOBJ("RF_tricycle.obj");

	// init handling
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// set the window size, and position at the center of the screen
	int centerX = ((glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2);
	int centerY = ((glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2);
	glutInitWindowPosition(centerX, centerY);
	glutInitWindowSize(WIDTH, HEIGHT);

	// title of the window
	glutCreateWindow("Tricycle");

	// called once to intialize opengl environment
	init();

	// parses the obj into different parts of the bicycle
	parseGroups();

	// callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutReshapeFunc(reshape);
	glutSpecialUpFunc(keyboardUp);

	// start the main loop
	glutMainLoop();

	glmDelete(model);
	return 0;
}

void init()
{
	glmUnitize(model);
	glmScale(model, 1.2);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-2, 2, -2, 2, -10, 10);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
}

//char* AddFileToParentPath(char* currentPath, char* fileName)
//{
//	char* parentPath = strdup(currentPath);
//	size_t length = strlen(parentPath);
//
//	// get the path of the parent directory
//	int found = 0;
//	for (size_t i = length - 1; i > 0; --i)
//	{
//		--length;
//		if (parentPath[i] == '\\')
//			++found;
//
//		if (found == 2)
//		{
//			parentPath[i + 1] = '\0';
//			++length;
//			break;
//		}
//	}
//
//	char* newString = (char*)malloc((length + strlen(fileName) + 1) * sizeof(char));
//	sprintf(newString, "%s%s", parentPath, fileName);
//	free(parentPath);
//
//	return newString;
//}

Model* createModel(GLMgroup* group)
{
	Model* m = new Model();
	m->group = group;

	double x = 0, y = 0, z = 0;
	for (GLuint i = 0; i < group->numtriangles; ++i)
	{
		GLMtriangle* triangle = &model->triangles[group->triangles[i]];

		for (GLuint j = 0; j < 3; ++j)
		{
			GLfloat* v = &model->vertices[3 * triangle->vindices[j]];

			x += v[0];
			y += v[1];
			z += v[2];
		}
	}

	double total = group->numtriangles * 3;
	m->center[0] = x / total;
	m->center[1] = y / total;
	m->center[2] = z / total;

	return m;
}

void parseGroups()
{
	GLMgroup* group = model->groups;

	while (group)
	{
		if (strstr(group->name, "front_wheel") != NULL)
			front_wheel = createModel(group);
		else if (strstr(group->name, "steering_wheel") != NULL)
			steering_wheel = createModel(group);
		else if (strstr(group->name, "back_wheel") != NULL)
			back_wheel = createModel(group);
		else if (strstr(group->name, "body") != NULL)
			body = createModel(group);

		group = group->next;
	}
}

void drawModel(Model* m)
{
	GLMgroup* group = m->group;

	glBegin(GL_LINES);
	for (GLuint i = 0; i < group->numtriangles; ++i)
	{
		GLMtriangle* triangle = &model->triangles[group->triangles[i]];

		for (GLuint j = 0; j < 3; ++j)
		{
			glNormal3fv(&model->normals[3 * triangle->nindices[j]]);
			glTexCoord2fv(&model->texcoords[2 * triangle->tindices[j]]);
			glVertex3fv(&model->vertices[3 * triangle->vindices[j]]);
		}
	}
	glEnd();
}

void centerModel(Model* model, bool toCenter)
{
	if(toCenter)
		glTranslatef(-model->center[0], -model->center[1], -model->center[2]);
	else
		glTranslatef(model->center[0], model->center[1], model->center[2]);
}

void display()
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// white color
	glColor3f(1, 1, 1);

	glTranslatef(bicycle_speed, 0, bicycle_speed);
	glRotatef(vehicle_angle, 0, 1, 0);

	// draw the body
	drawModel(body);

	// the back wheel, with the appropriate transformations
	// such as rotating the wheel.
	glPushMatrix();
	centerModel(back_wheel, false);
	glRotatef(wheels_angle, 1, 0, 0);
	centerModel(back_wheel, true);
	drawModel(back_wheel);
	glPopMatrix();

	// the front wheel rotation
	glPushMatrix();
	centerModel(front_wheel, false);
	glRotatef(steering_angle, 0, 1, 0);
	if (steering_angle < 0)
		glRotatef(13, 0, 0, 1);
	else if (steering_angle > 0)
		glRotatef(-13, 0, 0, 1);
	glRotatef(wheels_angle, 1, 0, 0);
	centerModel(front_wheel, true);
	drawModel(front_wheel);
	glPopMatrix();

	glPushMatrix();
	centerModel(steering_wheel, false);
	glRotatef(-15, 1, 0, 0);
	glRotatef(steering_angle, 0, 1, 0);
	glRotatef(15, 1, 0, 0);
	centerModel(steering_wheel, true);
	drawModel(steering_wheel);
	glPopMatrix();

	// we're using double buffer, so swap buffers
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	// set the viewport
	glViewport(0, 0, width, height);
}

void keyboard (int specialKey, int x, int y)
{
	if (specialKey == GLUT_KEY_LEFT)
	{
		steering_angle = -40;
		vehicle_angle -= 20;
	}
	else if (specialKey == GLUT_KEY_RIGHT)
	{
		vehicle_angle += 20;
		steering_angle = 40;
	}
	else if (specialKey == GLUT_KEY_UP)
	{
		wheels_angle += 5;
		bicycle_speed += 0.03;
	}
	else if (specialKey == GLUT_KEY_DOWN)
	{
		wheels_angle -= 5;
		bicycle_speed -= 0.03;
	}

	if (vehicle_angle > 360 || vehicle_angle < -360)
		vehicle_angle = 0;

	// request redisplay
	glutPostRedisplay();
}

void keyboardUp(int specialKey, int x, int y)
{
	if (specialKey == GLUT_KEY_LEFT || specialKey == GLUT_KEY_RIGHT)
		steering_angle = 0;

	glutPostRedisplay();
}