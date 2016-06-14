#include "Drawings.h"
#include <stdio.h>

void Game_Over()
{

	float turtle_x = 250;
	float turtle_y = 225;

	unsigned char r = 80;
	unsigned char g = 200;
	unsigned char b  = 80;

	LineWidth(4);
	LineColor(((double)r)/256.f, ((double)g)/256.f, ((double)b)/256.f);	


	G(turtle_x+50*1,turtle_y,50);

	A(turtle_x+50*2,turtle_y,50);

	M(turtle_x+50*3,turtle_y,50);

	E(turtle_x+50*4,turtle_y,50);
					
	O(turtle_x+50*5,turtle_y,50);

	V(turtle_x+50*6,turtle_y,50);

	E(turtle_x+50*7,turtle_y,50);
					
	R(turtle_x+50*8,turtle_y,50);
}

void Scene()
{
	float turtle_x = 450;
	float turtle_y = 5;

	unsigned char r = 80;
	unsigned char g = 200;
	unsigned char b  = 80;

	LineWidth(2);
	LineColor(((double)r)/256.f, ((double)g)/256.f, ((double)b)/256.f);	


	B(turtle_x+15*1,turtle_y,15);

	I(turtle_x+15*2,turtle_y,15);

	L(turtle_x+15*3,turtle_y,15);

	L(turtle_x+15*4,turtle_y,15);
					
	I(turtle_x+15*5,turtle_y,15);

	A(turtle_x+15*6,turtle_y,15);

	R(turtle_x+15*7,turtle_y,15);
					
	D(turtle_x+15*8,turtle_y,15);

	S(turtle_x+15*9,turtle_y,15);

	LineWidth(3);
	circle(30, 10, 10);

	circle(30, 10, 490);

	circle(30, 990, 10);

	circle(30, 990, 490);

	arbit_sector(50,180,-90,200, 250);
}

void movind_arc(float c_x,float c_y, float angle_x)
{
	arbit_sector(30, 120, angle_x,c_x,c_y);
}

void draw_thin_circle(float radius, float c_x, float c_y)
{
	circle(radius,c_x,c_y);
}




