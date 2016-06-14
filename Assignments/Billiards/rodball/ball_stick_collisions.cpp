#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include<math.h>
#include<stdio.h>
#include "Drawings.h"
#include "collisions.h"
#include <list>

using namespace std;

disk* main_ball = new disk(10,200,250,1,1);

rod* central_rod = new rod(500,250, 700,350,0,0,10);

boundary* out_boundary = new boundary();

void draw_circle(double r, double c_x, double c_y)
{
	glBegin(GL_POLYGON);
		for(int i = 0; i <= 360 ; i++)
			glVertex2f(c_x+r*cos(i),c_y+r*sin(i));
	glEnd();	
}

void display(void);
void keyboard(unsigned char key, int x, int y);
void timer_func(int v);

void init(void)
{
	unsigned char r = 40;
	unsigned char g = 94;
	unsigned char b  = 45;

	glClearColor(((double)r)/256.f, ((double)g)/256.f, ((double)b)/256.f, 0.5);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1000, 0, 500,0,1);
}


int  main (int argc, char **argv)
{
  	glutInit( &argc, argv );
  	glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE );
  	glutInitWindowSize (1000, 500);
  	glutInitWindowPosition (50, 100);
  	glutCreateWindow ("Billiards By Hardeep");

  	//Register the GLUT callbacks
  	glutTimerFunc(1,timer_func, 2);
  	glutDisplayFunc( display );
 
  	init();
  	glutMainLoop();


return 0;
}

void change_pos_vel()
{	
	transform* t = new transform();
	central_rod->v2 = t->Rotate_center(central_rod->v2,central_rod->v1,2);
	
	main_ball->c_x = main_ball->c_x + main_ball->v_x;
	main_ball->c_y = main_ball->c_y + main_ball->v_y;
	
}

void timer_func(int v)
{	
	change_pos_vel();
	collision(main_ball,out_boundary);
//	collision(main_ball,central_rod);
	glutTimerFunc (2, timer_func, 2);
	glutPostRedisplay();
}

//GLUT display callback
void display( void )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);		
	
		Scene();

		unsigned char r = 64;
		unsigned char g = 177;
		unsigned char b  = 76;

		glColor3f(((double)r)/256.f, ((double)g)/256.f, ((double)b)/256.f);
		
		draw_circle(main_ball->radius,main_ball->c_x,main_ball->c_y);
		
		glLineWidth(2);
		glBegin(GL_LINES);

			glVertex2f(50,30);
			glVertex2f(950,30);

			glVertex2f(30,50);
			glVertex2f(30,450);

			glVertex2f(50,470);
			glVertex2f(950,470);

			glVertex2f(970,450);
			glVertex2f(970,50);
		
			glVertex2f(200,80);
			glVertex2f(200,420);
		
		glEnd();

		draw_circle(15,15,485);
		draw_circle(15,985,485);
		draw_circle(15,985,15);
		draw_circle(15,15,15);	
		
		glBegin(GL_LINES);
			glVertex2f(central_rod->v1->vector[0],central_rod->v1->vector[1]);
			glVertex2f(central_rod->v2->vector[0],central_rod->v2->vector[1]);
		glEnd();	
	glFlush();
}
