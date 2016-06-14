#include "Turtle.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

float x=0;
float y=0;
float theta=0;
bool draw=false;

void Init(float _x,float _y, float _theta)
{
	x = _x;
	y = _y;
	theta = _theta;
}

void Forward(float dist)
{
	float x_final;
	float y_final;
	
	x_final = x + dist*cos(theta*pi/180);
	y_final = y + dist*sin(theta*pi/180);

	if(draw == true)
	{
		glBegin(GL_LINES);
			glVertex3f(x,y,0.0);
			glVertex3f(x_final,y_final,0.0);				
		glEnd();	
	}	
	
	x = x_final;
	y = y_final;
	//Implement this function
}

void Backward(float dist)
{
	float x_final;
	float y_final;
	
	
	x_final = x - dist*cos(theta*pi/180);
	y_final = y - dist*sin(theta*pi/180);
	
	if(draw == true)
	{
		glBegin(GL_LINES);
			glVertex3f(x,y,0.0);
			glVertex3f(x_final,y_final,0.0);				
		glEnd();	
	}	
	
	x = x_final;
	y = y_final;
	//Implement this function
}

void Right(float _theta)
{
	theta = theta - _theta;
}

void Left(float _theta)
{
	theta  = theta + _theta;
}

void PenDown( bool down ) 
{
	if(down == true)
		draw = true;
	else 
		draw = false;
}

void LineColor(float r,float g, float b)
{
	glColor3f(r,g,b);
}

void BGColor(float r,float g, float b)
{
	glClearColor(r,g,b,0);
	glClear(GL_COLOR_BUFFER_BIT);
	
}

void LineWidth( float width ) 
{
	glLineWidth(width);
}

void ResetPosition() 
{
	x = 0.0;
	y = 0.0;
	theta = 0.0;
}

void ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

