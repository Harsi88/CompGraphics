#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/glut.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static FILE *fp;
//--- Global file pointer for reading the input file
int flag = 0;
//--- Flag to prevent glutDisplayFunc() from running twice\
	function runs when flag is equal to zero

//--- Implementation of Bresenham's Line Drawing Algorithm
void bresenham()
{
    //--- Setting flag to one to prevent display func from running twice
    flag = 1;
    //--- x0,y0,r0,g0,b0,a0 are the specifications for starting point of line
    //--- x1,y1,r1,g1,b1,a1 are the specifications for terminating point of line
    float x0,x1,y0,y1,r1,r0,g1,g0,b1,b0,a1,a0;
    float num_lines,i;				//--- num_lines represent number of lines to be drawn------and i is iterator
    fscanf(fp,"%f",&num_lines);
    for(i=0; i<num_lines; i++)			//--- Iterating over number of lines
    {     
    	    fscanf(fp, "%f", &x0);
    	    fscanf(fp, "%f", &y0);
     	    fscanf(fp, "%f", &r0);
    	    fscanf(fp, "%f", &g0);
    	    fscanf(fp, "%f", &b0);
    	    fscanf(fp, "%f", &a0);
    	    
    	    fscanf(fp, "%f", &x1);
    	    fscanf(fp, "%f", &y1);
    	    fscanf(fp, "%f", &r1);
    	    fscanf(fp, "%f", &g1);
    	    fscanf(fp, "%f", &b1);
    	    fscanf(fp, "%f", &a1);
    	    
    	    //--- Input taken for a line
    glColor4f(r0,g0,b0,a0);
    glVertex2f(x0,y0);
    	    
    float dx = x1 - x0;		
    float dy = y1 - y0;
    //---dy and dx are used to give required slope to the line 
    float incrementx = 1;
    float incrementy = 1;
    //---Increment x and y by required increment based on glOrtho() co-ordinate system
    float x = x0;
    float y = y0;
    //---x and y are the pixels to be illuminated to draw given line
    float error = 0;
    //--- error will determine how much our pixel has shifted from original line
    int slope_greater_than_one;
    //--- slope_greater_than_one helps in determining which co-ordinate to increment on each step (x or y)
	    	
    if(fabs(dx) > fabs(dy)) slope_greater_than_one = 0;		//--- slope is less than one 
    else slope_greater_than_one = 1;				//--- slope is greater than one
    	
    if(dx < 0) incrementx = -incrementx;
    if(dy < 0) incrementy = -incrementy;
    //--- Determining whether to increase or decrease our co-ordinate on each step
    
    while(1)
    {
    	if(dy==0 && dx==0) break;
        glVertex2f(x,y);		//--- Specifying the pixel to be included in the line drawing
        if(slope_greater_than_one==0)   //--- Case where we have to increment x after every step
        {   
            //---specifying color of next pixel which is based on how far we have travelled on the line
            glColor4f(r0 + (x-x0)/(x1-x0)*(r1-r0),g0 + (x-x0)/(x1-x0)*(g1-g0),b0 + (x-x0)/(x1-x0)*(b1-b0),a0 + (x-x0)/(x1-x0)*(a1-a0));
            //---Increasing error by dy---- because we have moved in x-direction only
            error += fabs(dy);
            //---If error becomes more than the tolerated value than we increment y and hence reduce the error 
            if((error*2) >= fabs(dx)){
                y += incrementy;
	        error -= fabs(dx);
	    }
	    x += incrementx;		//--- incrementing x on every step
	    //--- Break condition for finishing line drawing by checking if point has surpassed the final point
	    if(dx > 0 && x>=x1) break;  
	    if(dx < 0 && x<=x1) break;
	}
	if(slope_greater_than_one==1)   //--- Case where we will increment y after every step
        {   
            //---specifying color of next pixel which is based on how far we have travelled on the line 
            glColor4f(r0 + (y-y0)/(y1-y0)*(r1-r0),g0 + (y-y0)/(y1-y0)*(g1-g0),b0 + (y-y0)/(y1-y0)*(b1-b0),a0 + (y-y0)/(y1-y0)*(a1-a0)); 
            //---Increasing error by dx---- because we have moved in y-direction only
            error += fabs(dx);
            //---If error becomes more than the tolerated value than we increment y and hence reduce the error 
            if((error*2) >= fabs(dy)){
                x += incrementx;
	        error -= fabs(dy);
	    }
	    y += incrementy;
	    //--- incrementing y on every step
	    //--- Break condition for finishing line drawing by checking if point has surpassed the final point
	    if(dy > 0 && y>=y1) break;
	    if(dy < 0 && y<=y1) break;
	}
    }
        }

    return;    
}

//--- Determine area of triangle formed by given points using determinant method
float area_determinant(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float area = x1*(y2-y3) - x2*(y1-y3) + x3*(y1-y2);
    area = area/2;
    return fabs(area);
} 

//--- Algorithm to draw triangle given three vertices
void drawTriangle()
{ 
    //--- Setting flag to one to prevent display func from running twice
    flag = 1;
    //--- xi,yi,ri,gi,bi,ai for i=1,2,3 are the specifications of i'th vertex of triangle
    float x1,x2,x3,y1,y2,y3,r1,r2,r3,g1,g2,g3,b1,b2,b3,a1,a2,a3;
    //--- Number of triangles to be drawn
    float num_of_triangles;
    	
    fscanf(fp,"%f",&num_of_triangles);
    
    while(num_of_triangles >= 1)	//--- Iterating for number of triangles
    {
   	fscanf(fp,"%f",&x1);
    	fscanf(fp,"%f",&y1);
    	fscanf(fp,"%f",&r1);
    	fscanf(fp,"%f",&g1);
    	fscanf(fp,"%f",&b1);
    	fscanf(fp,"%f",&a1);
    	
    	fscanf(fp,"%f",&x2);
    	fscanf(fp,"%f",&y2);
    	fscanf(fp,"%f",&r2);
    	fscanf(fp,"%f",&g2);
    	fscanf(fp,"%f",&b2);
    	fscanf(fp,"%f",&a2);
    	
    	fscanf(fp,"%f",&x3);
    	fscanf(fp,"%f",&y3);
    	fscanf(fp,"%f",&r3);
    	fscanf(fp,"%f",&g3);
    	fscanf(fp,"%f",&b3);
    	fscanf(fp,"%f",&a3);
    	//--- Input Taken
    	
    	//--- Determining area of triangle
    	float area_of_triangle = area_determinant(x1,y1,x2,y2,x3,y3), area_1, area_2, area_3;
    	
    	//--- Calculating minimum and maximum value of x and y \
    		so that we will have a rectangle which will include\
    		every pixel of triangle in it, alongwith some\
    		additional ones which we will reject by iterating\
    		through them
    	float xmin, xmax, ymin, ymax;
    	xmin = MIN(x1,MIN(x2,x3));
    	xmax = MAX(x1,MAX(x2,x3));
    	ymin = MIN(y1,MIN(y2,y3));
    	ymax = MAX(y1,MAX(y2,y3));
    	
    	float i = xmin;
    	float j = ymin;
    	
    	//--- Value of the color of every pixel in the triangle based\
    		on Barycentric Interpolation
    	float r,g,b,a;
    	
    	//---Iterating over every pixel in the rectangle made by xmin,ymin,xmax,ymax
    	for (i=xmin; i<=xmax; i++)
    	{
    	    for(j=ymin;j<=ymax; j++)
    	    {
    	    	//--- Every point inside the triangle will divide it\
    	    		into three triangles, sum of area of those triangles\
    	    		will be equal to area of original triangle\
    	    		using above condition, reject or accept the pixel\
    	    		to be included
    	        area_1 = area_determinant(i,j,x2,y2,x3,y3);
    	        area_2 = area_determinant(x1,y1,i,j,x3,y3);
    	        area_3 = area_determinant(x1,y1,x2,y2,i,j);
    	        if((area_1+area_2+area_3) == area_of_triangle)
    	        {
    	            r = (area_1*r1+area_2*r2+area_3*r3)/area_of_triangle;
    	            g = (area_1*g1+area_2*g2+area_3*g3)/area_of_triangle;
    	            b = (area_1*b1+area_2*b2+area_3*b3)/area_of_triangle;
    	            a = (area_1*a1+area_2*a2+area_3*a3)/area_of_triangle;
    	            glColor4f(r,g,b,a);
		    glVertex3f(i,j,0.0f);  
		}
    	    }
    	}
    	
    	num_of_triangles--;
    } 	  
    return;
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);	//---Clear Color Buffer to Preset Values by glClearColor
    if(flag==0)
    {
     glBegin(GL_POINTS);
        bresenham();			
     glEnd();
     glFlush();
     glBegin(GL_POINTS);
       drawTriangle();
     glEnd();
     glFlush();   
    }  
    return;
}

//--- Code to handle the window resizing interrupt
void reshape(int w, int h)
{
    glClearColor(0,0,0,1);	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    //--- Get Current size of viewport
    glGetIntegerv(GL_VIEWPORT,viewport);
    //--- Setting orthographic matrix based on current viewport settings
    glOrtho(-viewport[2]/2,viewport[2]/2,-viewport[3]/2,viewport[3]/2,-1,1);
    //--- Redrawing everything
    fseek(fp,SEEK_SET,0);
    flag = 0;
    glutPostRedisplay();
}

//--- Initialising settings for window
void init(void)
{
    glClearColor(0,0,0,1);	//---Set Clear Values for Color Buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glOrtho(-viewport[2]/2,viewport[2]/2,-viewport[3]/2,viewport[3]/2,-1,1);
    return;
}

int main(int argc, char** argv)
{
	//--- Initialising Glut library
	glutInit(&argc, argv);
	//--- Initialising Window variables
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,800);
	glutCreateWindow("Scan Conversion");
	init();

	fp = fopen(argv[1],"r");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	fclose(fp);
	
	return 0;
}
