#ifndef MAIN_H
#define MAIN_H

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Model.h"
#include "libobj.h"

#pragma warning(disable : 4996)

using namespace std;

#define WIDTH 500
#define HEIGHT 500

GLMmodel* model;
Model* front_wheel;
Model* steering_wheel;
Model* back_wheel;
Model* body;
double steering_angle = 0;
double wheels_angle = 0;
double bicycle_speed = 0;
double vehicle_angle = 0;

void reshape(int width, int height);
void display();
void init();

void keyboard(int specialKey, int x, int y);
void keyboardUp(int specialKey, int x, int y);

//char* AddFileToParentPath(char* currentPath, char* fileName);
Model* createModel(GLMgroup* group);
void parseGroups();

#endif