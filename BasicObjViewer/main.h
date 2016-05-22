#ifndef MAIN_H
#define MAIN_H

using namespace std;
typedef unsigned int uint;

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Triangle.h"
#include "Vertex.h"
#include "Model.h"

#define WIDTH 450
#define HEIGHT 450

Model* model = NULL;
float move_x = 0;
float move_y = 0;
float rotate_x = 0;
float rotate_y = 0;

void reshape(int width, int height);
void keyboard(int specialKey, int x, int y);
int usage();
void display();
void init();

#endif