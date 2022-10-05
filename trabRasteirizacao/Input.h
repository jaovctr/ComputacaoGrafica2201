#pragma once
#include <GL/freeglut.h>
#include <iostream>
#include <utility>
#include <vector>

enum State_Enum {
	PENCIL,
	TRIANGLE,
	LINE,
	RECTANGLE,
	POLYGON,
	FILL,
	CIRCLE,
	TRANSLATE,
	B_CIRCLE
};

enum Direction {
	RIGHT,
	LEFT,
	DOWN,
	UP
};

struct Point {
	int x;
	int y;
};

std::vector<Point> point_vector;

struct State {
	std::pair<State_Enum, int> click_state;
	int num_of_clicks = 0;
};

struct Color {
	GLfloat r, g, b;
};

Direction global_dir;