/*
* Claude Abounegm
* main.cpp
*/

#include "main.h"

int main(int argc, char** argv)
{
	// check if the path was given
	if (argc < 2)
		return usage();

	// check for extension
	char* extension = (argv[1] + strlen(argv[1]) - 3);
	if (tolower(extension[0]) != 'o' || towlower(extension[1]) != 'b' || tolower(extension[2]) != 'j')
		return usage();

	// we set the model, and if it's null, we return
	model = Model::fromObjFile(argv[1]);
	if (!model)
		return usage();

	// init handling
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// set the window size, and position at the center of the screen
	int centerX = ((glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2);
	int centerY = ((glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2);
	glutInitWindowPosition(centerX, centerY);
	glutInitWindowSize(WIDTH, HEIGHT);

	// title of the window
	glutCreateWindow("Q3: .obj reader");

	// called once to intialize opengl environment
	init();

	// callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutReshapeFunc(reshape);

	// start the main loop
	glutMainLoop();

	return 0;
}

int usage()
{
	cout << "Usage: BasicObjViewer.exe \"path\"" << endl;
	return 1;
}

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// show front, and hide back
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_POINT);

	// change matrices
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// rotation about the x axis
	if (rotate_x > 360)
		rotate_x = 360;
	else if (rotate_x < 0)
		rotate_x = 0;
	// rotation about the y axis
	if (rotate_y > 360)
		rotate_y = 360;
	else if (rotate_y < 0)
		rotate_y = 0;

	// handle color, rotation, and translation
	glColor3d(0.0, 0.0, 0.0);
	glTranslatef(move_x, move_y, 0.0);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	// draw the model
	model->draw();

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
	// if ctr is pressed, we rotate
	int ctrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);

	if (specialKey == GLUT_KEY_LEFT)
	{
		if (ctrl) rotate_y -= 1.0;
		else move_x -= 0.1;
	}
	else if (specialKey == GLUT_KEY_RIGHT)
	{
		if (ctrl) rotate_y += 1.0;
		else move_x += 0.1;
	}
	else if (specialKey == GLUT_KEY_UP)
	{
		if (ctrl) rotate_x -= 1.0;
		else move_y += 0.1;
	}
	else if (specialKey == GLUT_KEY_DOWN)
	{
		if (ctrl) rotate_x += 1.0;
		else move_y -= 0.1;
	}

	// request redisplay
	glutPostRedisplay();
}