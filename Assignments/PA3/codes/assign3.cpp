/*
Author: Frank Pfenning
*/

#include<stdio.h>
#include<iostream>
#include<math.h>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>

#define MAX_TRIANGLES 10000
#define MAX_SPHERES 10000
#define MAX_LIGHTS 10000

char *filename=0;

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2
int mode=MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
#define WIDTH 640
#define HEIGHT 480

//the field of view of the camera
#define fov 60.0
#define pi 3.1415926535897932384626433

unsigned char buffer[HEIGHT][WIDTH][3];

struct Vertex
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double normal[3];
  double shininess;
};

typedef struct _Triangle
{
  struct Vertex v[3];
} Triangle;

typedef struct _Sphere
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
} Sphere;

typedef struct _Light
{
  double position[3];
  double color[3];
} Light;

Triangle triangles[MAX_TRIANGLES];
Sphere spheres[MAX_SPHERES];
Light lights[MAX_LIGHTS];
double ambient_light[3];

int num_triangles=0;
int num_spheres=0;
int num_lights=0;

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

int flag = 0;


//--- method to calulate area of triangles from its co-ordinates
double Area(double A[],double B[], double C[]){
  double v1[] = { A[0]-B[0], A[1]-B[1], A[2]-B[2]};
  double v2[] = { C[0]-B[0], C[1]-B[1], C[2]-B[2]};
  double v[] = {v1[1]*v2[2]-v1[2]*v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0] };
  return fabs(0.5*sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] ));
}

//--- Checking intersection of a ray with all the objects
bool checkIntersection(double R0[], double Rd[]){
  
  for(int i=0;i<num_triangles;i++){
    double N[3];
    //--- A,B,C are the vertices of triangle
    double A[] = { triangles[i].v[0].position[0], triangles[i].v[0].position[1], triangles[i].v[0].position[2]},
           B[] = { triangles[i].v[1].position[0], triangles[i].v[1].position[1], triangles[i].v[1].position[2]},
           C[] = { triangles[i].v[2].position[0], triangles[i].v[2].position[1], triangles[i].v[2].position[2]};
    //--- N is the normal vector of triangle
    N[0] = (B[1]-A[1])*(C[2]-A[2]) - (C[1]-A[1])*(B[2]-C[2]);
    N[1] = (B[2]-C[2])*(C[0]-A[0]) - (B[0]-A[0])*(C[2]-A[2]);
    N[2] = (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]);
    double mod = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
    N[0]/=mod; N[1]/=mod; N[2]/=mod;  //--- N normalized
    double d = -1.0*( N[0]*A[0] + N[1]*A[1] + N[2]*A[2] );
    double t1 = -1.0*( (N[0]*R0[0] + N[1]*R0[1] + N[2]*R0[2]+d)/( N[0]*Rd[0] + N[1]*Rd[1] + N[2]*Rd[2] ) );
    if (t1>1e-10){     
      double P[] = { R0[0]+t1*Rd[0], R0[1]+t1*Rd[1], R0[2]+t1*Rd[2] }; //--- Intersection Point with the plane
      double ABP = Area(A,B,P), ACP = Area(A,C,P), BCP = Area(B,C,P), ABC = Area(A,B,C);
      //--- Computing areas of triangles formed by point P in order to 
      if(fabs(ABP + ACP + BCP - ABC) < 1e-10 ) return true;
    }
  }
  for(int i=0;i<num_spheres;i++){
    double C[3];
    C[0] = spheres[i].position[0]; C[1] = spheres[i].position[1]; C[2] = spheres[i].position[2];
    //--- a,b,c are the coefficients of quadratic equations we require to solve in order to get intersection point with sphere
    double a = Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2];
    double b = 2.0*((Rd[0]*(R0[0]-C[0])) + (Rd[1]*(R0[1]-C[1])) + (Rd[2]*(R0[2]-C[2])));
    double radius = spheres[i].radius;
    double c = (R0[0]-C[0])*(R0[0]-C[0]) + (R0[1]-C[1])*(R0[1]-C[1]) + (R0[2]-C[2])*(R0[2]-C[2]) - radius*radius;
    double d = b*b - 4*a*c;
    if(d>=0){
      double t1 = (-b+sqrt(d))/(2*a), t2 = (-b-sqrt(d))/(2*a);
      if(t1>t2) t1 = t2;
      if(t1>1e-04) return true;
    }
  }
  return false;
}

//MODIFY THIS FUNCTION
void draw_scene()
{
if(flag==0){
flag=1;
  int x,y;
  for(x=(-1.0*WIDTH)/2;x < WIDTH/2;x++)
  {
    glBegin(GL_POINTS);
    for(y=(-1.0*HEIGHT)/2;y < HEIGHT/2;y++)
    {
      double t=999999;
      double IP[] = {0,0,0};
      for(int l=0;l<num_lights;l++){
        double I[] = {0,0,0};  
        
        //--- Intersection with triangles 
        for(int i=0;i < num_triangles;i++){
          double Rd[3];
          Rd[0] = (double)x; 
          Rd[1] = (double)y;
          Rd[2] = -415.7;
          //Rd[2] = -tan((fov*pi)/360)*WIDTH/2;
          double N[3];
          //--- A,B,C are Vertices of triangle
          double A[] = { triangles[i].v[0].position[0], triangles[i].v[0].position[1], triangles[i].v[0].position[2]},
                  B[] = { triangles[i].v[1].position[0], triangles[i].v[1].position[1], triangles[i].v[1].position[2]},
                  C[] = { triangles[i].v[2].position[0], triangles[i].v[2].position[1], triangles[i].v[2].position[2]};
          //--- N is the normal to triangle
          N[0] = (B[1]-A[1])*(C[2]-A[2]) - (C[1]-A[1])*(B[2]-A[2]);
          N[1] = (B[2]-A[2])*(C[0]-A[0]) - (B[0]-A[0])*(C[2]-A[2]);
          N[2] = (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]);
          double mod = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
          N[0]/=mod; N[1]/=mod; N[2]/=mod;  //--- normalising N
          double d = -1.0*( N[0]*A[0] + N[1]*A[1] + N[2]*A[2] );
          double t1 = -1.0*( d/( N[0]*Rd[0] + N[1]*Rd[1] + N[2]*Rd[2] ) );
          if(t1>=0 && t1 < t){
            double P[] = { t1*Rd[0], t1*Rd[1], t1*Rd[2] };

            double ABP = Area(A,B,P), ACP = Area(A,C,P), BCP = Area(B,C,P), ABC = Area(A,B,C);
            if(fabs(ABP + ACP + BCP - ABC) < 1e-6 ){
              t = t1;
              
              //--- Computing intensity of light at the intersection point
              double kd[3],ks[3],shi;
              
              //--- Computing diffuse-coefficient using barycentric co-ordinates
              kd[0] = (triangles[i].v[0].color_diffuse[0]*BCP + triangles[i].v[1].color_diffuse[0]*ABP + triangles[i].v[2].color_diffuse[0]*ABP)/ABC;
              kd[1] = (triangles[i].v[0].color_diffuse[1]*BCP + triangles[i].v[1].color_diffuse[1]*ABP + triangles[i].v[2].color_diffuse[1]*ABP)/ABC;
              kd[2] = (triangles[i].v[0].color_diffuse[2]*BCP + triangles[i].v[1].color_diffuse[2]*ABP + triangles[i].v[2].color_diffuse[2]*ABP)/ABC;
              
              //--- Computing specular coefficient using barycentric co-ordinates
              ks[0] = (triangles[i].v[0].color_specular[0]*BCP + triangles[i].v[1].color_specular[0]*ABP + triangles[i].v[2].color_specular[0]*ABP)/ABC;
              ks[1] = (triangles[i].v[0].color_specular[1]*BCP + triangles[i].v[1].color_specular[1]*ABP + triangles[i].v[2].color_specular[1]*ABP)/ABC;
              ks[2] = (triangles[i].v[0].color_specular[2]*BCP + triangles[i].v[1].color_specular[2]*ABP + triangles[i].v[2].color_specular[2]*ABP)/ABC;
              
              //--- Computing Shininess using barycentric co-ordinates
              shi  = (triangles[i].v[0].shininess*BCP + triangles[i].v[1].shininess*ABP + triangles[i].v[2].shininess*ABP)/ABC;
              
              //--- View Vector
              double V[3] = { -1.0*Rd[0], -1.0*Rd[1], -1.0*Rd[2] };
              mod = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
              V[0]/=mod;  V[1]/=mod;  V[2]/=mod;
              
              double L[3],R[3];
              //--- intensity of light at x,y Pixel
              I[0] = I[1] = I[2] = 0;
              //--- Light Vector
              L[0] = lights[l].position[0] - P[0];
              L[1] = lights[l].position[1] - P[1];
              L[2] = lights[l].position[2] - P[2];
              mod = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
              L[0]/=mod;  L[1]/=mod;  L[2]/=mod;  //--- Normalising L

              if(checkIntersection(P,L)){ //--- Checking If Point is in Shadow
                I[0] = I[1] = I[2] = 0;
              }
              else{
              
              //--- Cosine of angle between Light vector and Normal vector
              double cos0 = L[0]*N[0] + L[1]*N[1] + L[2]*N[2];
              
              if(cos0 > 0.0f){  //--- To check if point is in front of light source or not
                  //--- Calculating Reflection Vector
                  R[0] = 2*N[0]*cos0-L[0];
                  R[1] = 2*N[1]*cos0-L[1];
                  R[2] = 2*N[2]*cos0-L[2];
                  mod = sqrt(R[0]*R[0] + R[1]*R[1] + R[2]*R[2]);
                  R[0]/=mod;  R[1]/=mod;  R[2]/=mod;  //--- Normalising R
                  
                  //--- Computing Intensity of light using phong model
                  double ln = cos0, rv = R[0]*V[0]+R[1]*V[1]+R[2]*V[2];
                  I[0] += lights[l].color[0]*(ln*kd[0] + ks[0]*pow(rv,spheres[i].shininess));
                  I[1] += lights[l].color[1]*(ln*kd[1] + ks[1]*pow(rv,spheres[i].shininess));
                  I[2] += lights[l].color[2]*(ln*kd[2] + ks[2]*pow(rv,spheres[i].shininess));
              }
              }
            }
          }
        }
        
        //--- Intersection with spheres
        for(int i=0;i < num_spheres;i++){
          double Rd[3]; //--- Vector in the direction of ray
          Rd[0] = (double)x; 
          Rd[1] = (double)y;
          Rd[2] = -415.7;
          //Rd[2] = -tan((fov*pi)/360)*WIDTH/2;
          double C[3];  //--- center of sphere
          C[0] = spheres[i].position[0]; C[1] = spheres[i].position[1]; C[2] = spheres[i].position[2];
          //--- a,b,c are coefficients of quadratic equation which is used to compute intersection point of ray with sphere
          double a = Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2];
          double b = -2.0*((Rd[0]*C[0]) + (Rd[1]*C[1]) + (Rd[2]*C[2]));
          double radius = spheres[i].radius;
          double c = C[0]*C[0] + C[1]*C[1] + C[2]*C[2] - radius*radius;
          //--- Discriminant of quadratic equations
          double d = b*b - 4*a*c;
                    
          if(d>=0){ //--- To check that there is a intersection of ray with sphere
            double t1 = (-b+sqrt(d))/(2*a), t2 = (-b-sqrt(d))/(2*a);  
            if(t2>0 && t1>t2) t1 = t2;  //--- parameter closer to the camera
            if(t1>0 && t1<t){ //--- If this pixel is already colored,Compare parameter t1 with the older parameter t, if it is closer overwrite it
              t = t1;
              //--- Specular, Ambient amd diffused coefficients
              double kd[3];  kd[0] = spheres[i].color_diffuse[0]; kd[1] = spheres[i].color_diffuse[1]; kd[2] = spheres[i].color_diffuse[2];
              double ks[3];  ks[0] = spheres[i].color_specular[0]; ks[1] = spheres[i].color_specular[1]; ks[2] = spheres[i].color_specular[2];
              
              //--- intersection point
              double IntP[3] = { t1*Rd[0], t1*Rd[1], t1*Rd[2]};
              
              //--- normal to the surface
              double N[3] = { IntP[0]-C[0], IntP[1]-C[1], IntP[2]-C[2] };
              double mod = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
              N[0]/=mod;  N[1]/=mod;  N[2]/=mod;
              //--- View Vector
              double V[3] = { -1.0*Rd[0], -1.0*Rd[1], -1.0*Rd[2] };
              mod = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
              V[0]/=mod;  V[1]/=mod;  V[2]/=mod;
              double L[3],R[3];
              
              I[0] = I[1] = I[2] = 0;
              //--- Light vector
              L[0] = lights[l].position[0] - IntP[0];
              L[1] = lights[l].position[1] - IntP[1];
              L[2] = lights[l].position[2] - IntP[2];
              mod = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
              L[0]/=mod;  L[1]/=mod;  L[2]/=mod;  //--- normalising L
  
              if(checkIntersection(IntP,L)){  //--- Checking if point is in shadow
                I[0] = I[1] = I[2] = 0;
              }
              else{
              double cos0 = L[0]*N[0] + L[1]*N[1] + L[2]*N[2];
              if(cos0 > 0.0f){  //--- If point is in front of light source
                  //--- Reflection Vector
                  R[0] = 2*N[0]*cos0-L[0];
                  R[1] = 2*N[1]*cos0-L[1];
                  R[2] = 2*N[2]*cos0-L[2];
                  mod = sqrt(R[0]*R[0] + R[1]*R[1] + R[2]*R[2]);
                  R[0]/=mod;  R[1]/=mod;  R[2]/=mod;
                  
                  //--- Computing intensity of light using phong model
                  double ln = cos0, rv = R[0]*V[0]+R[1]*V[1]+R[2]*V[2];
                  I[0] += lights[l].color[0]*(ln*kd[0] + ks[0]*pow(rv,spheres[i].shininess));
                  I[1] += lights[l].color[1]*(ln*kd[1] + ks[1]*pow(rv,spheres[i].shininess));
                  I[2] += lights[l].color[2]*(ln*kd[2] + ks[2]*pow(rv,spheres[i].shininess));
              }
              }
            }
          }
        }
        IP[0] += I[0];  IP[1] += I[1];  IP[2] += I[2];
      }
      glColor3f(IP[0]+ambient_light[0],IP[1]+ambient_light[1],IP[2]+ambient_light[2]);
      glVertex2i(x, y);
    }
    glEnd();
    glFlush();
  }
  printf("Done!\n"); fflush(stdout);
}
}

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  glColor3f(((double)r)/256.f,((double)g)/256.f,((double)b)/256.f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  buffer[HEIGHT-y-1][x][0]=r;
  buffer[HEIGHT-y-1][x][1]=g;
  buffer[HEIGHT-y-1][x][2]=b;
}

void plot_pixel(int x,int y,unsigned char r,unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
      plot_pixel_jpeg(x,y,r,g,b);
}

void parse_check(char *expected,char *found)
{
  if(strcasecmp(expected,found))
    {
      char error[100];
      printf("Expected '%s ' found '%s '\n",expected,found);
      printf("Parse error, abnormal abortion\n");
      exit(0);
    }

}

void parse_doubles(FILE*file, char *check, double p[3])
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check(check,str);
  fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
//  printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

void parse_rad(FILE*file,double *r)
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check((char*)"rad:",str);
  fscanf(file,"%lf",r);
//  printf("rad: %f\n",*r);
}

void parse_shi(FILE*file,double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check((char*)"shi:",s);
  fscanf(file,"%lf",shi);
//  printf("shi: %f\n",*shi);
}

int loadScene(char *argv)
{
  FILE *file = fopen(argv,"r");
  int number_of_objects;
  char type[50];
  int i;
  Triangle t;
  Sphere s;
  Light l;
  fscanf(file,"%i",&number_of_objects);

  printf("number of objects: %i\n",number_of_objects);
  char str[200];

  parse_doubles(file,(char*)"amb:",ambient_light);

  for(i=0;i < number_of_objects;i++)
    {
      fscanf(file,"%s\n",type);
//      printf("%s\n",type);
      if(strcasecmp(type,"triangle")==0)
	{

//	  printf("found triangle\n");
	  int j;

	  for(j=0;j < 3;j++)
	    {
	      parse_doubles(file,(char*)"pos:",t.v[j].position);
	      parse_doubles(file,(char*)"nor:",t.v[j].normal);
	      parse_doubles(file,(char*)"dif:",t.v[j].color_diffuse);
	      parse_doubles(file,(char*)"spe:",t.v[j].color_specular);
	      parse_shi(file,&t.v[j].shininess);
	    }

	  if(num_triangles == MAX_TRIANGLES)
	    {
	      printf("too many triangles, you should increase MAX_TRIANGLES!\n");
	      exit(0);
	    }
	  triangles[num_triangles++] = t;
	}
      else if(strcasecmp(type,"sphere")==0)
	{
//	  printf("found sphere\n");

	  parse_doubles(file,(char*)"pos:",s.position);
	  parse_rad(file,&s.radius);
	  parse_doubles(file,(char*)"dif:",s.color_diffuse);
	  parse_doubles(file,(char*)"spe:",s.color_specular);
	  parse_shi(file,&s.shininess);

	  if(num_spheres == MAX_SPHERES)
	    {
	      printf("too many spheres, you should increase MAX_SPHERES!\n");
	      exit(0);
	    }
	  spheres[num_spheres++] = s;
	}
      else if(strcasecmp(type,"light")==0)
	{
//	  printf("found light\n");
	  parse_doubles(file,(char*)"pos:",l.position);
	  parse_doubles(file,(char*)"col:",l.color);

	  if(num_lights == MAX_LIGHTS)
	    {
	      printf("too many lights, you should increase MAX_LIGHTS!\n");
	      exit(0);
	    }
	  lights[num_lights++] = l;
	}
      else
	{
	  printf("unknown type in scene description:\n%s\n",type);
	  exit(0);
	}
    }
  return 0;
}

void display()
{

}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(-WIDTH/2,WIDTH/2,-HEIGHT/2,HEIGHT/2,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{
  //hack to make it only draw once
  static int once=0;
  if(!once)
  {
      draw_scene();
//      if(mode == MODE_JPEG)
//	save_jpg();
    }
  once=1;
}

int main (int argc, char ** argv)
{
  if (argc<2 || argc > 3)
  {  
    printf ("usage: %s <scenefile> [jpegname]\n", argv[0]);
    exit(0);
  }
  if(argc == 3)
    {
      mode = MODE_JPEG;
      filename = argv[2];
    }
  else if(argc == 2)
    mode = MODE_DISPLAY;

  glutInit(&argc,argv);
  loadScene(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}
