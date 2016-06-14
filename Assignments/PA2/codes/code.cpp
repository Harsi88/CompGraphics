#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include<iostream>
#include<cmath>
#include "ifs.h"
using namespace std;

#define pi 3.14159265358979323846

//--- VECTOR CLASS ---//
class vector4
{
public:
    double v1,v2,v3,v4;
    //--- Default Constructor to set vector to Zero 
    vector4()
    {
        v1 = v2 = v3 = v4 = 0;
    }
    //--- Constructor definition for four inputs
    vector4(double a, double b, double c, double d)
    {
        v1 = a; v2 = b; v3 = c; v4 = d;
    }
    //--- Method to print the vector
    void print(string msg = ""){
            cout << msg << " "<< v1 << " " << v2 << " " <<v3 << " " << v4 << endl;
    }
    //--- "+" Operator overloading for addition of two vector4 objects
    vector4 operator + (vector4 v)
    {
        vector4 result;
        result.v1 = v1 + v.v1;
        result.v2 = v2 + v.v2;
        result.v3 = v3 + v.v3;
        result.v4 = 1;
        return result;
    }
    //--- "-" Operator overloading for subtraction of two vector4 objects
    vector4 operator - (vector4 v)
    {
        vector4 result;
        result.v1 = v1 - v.v1;
        result.v2 = v2 - v.v2;
        result.v3 = v3 - v.v3;
        result.v4 = 1;
        return result;
    }
    //--- Method for a vector4 object to Homogenize itself
    vector4 homogenize(){
        vector4 result;
        result.v1 = v1/v4; result.v2 = v2/v4; result.v3 = v3/v4; result.v4 = v4/v4; 
        return result;
    }
    //--- Method to Translate vector 
    vector4 translate(double dx, double dy,double dz){
        vector4 result;
        vector4 temp = homogenize();
        result.v1 = temp.v1 + dx;
        result.v2 = temp.v2 + dy;
        result.v3 = temp.v3 + dz;
        result.v4 = temp.v4;
        return result;
    }
    //--- Method to Scale Vector about Origin 
    vector4 scale(double sx, double sy, double sz){
        vector4 result;
        vector4 temp = homogenize();
        result.v1 = temp.v1*sx;
        result.v2 = temp.v2*sy;
        result.v3 = temp.v3*sz;
        result.v4 = temp.v4;
        return result;
    }
    //--- Method to Rotate vector around x-axis
    vector4 rotateX(double theta){    //--- Theta is in degrees
        vector4 result,temp = homogenize();
        theta *= pi/180;
        result.v1 = temp.v1;
        result.v2 = temp.v2*cos(theta)-temp.v3*sin(theta);
        result.v3 = temp.v2*sin(theta)+temp.v3*cos(theta);
        result.v4 = temp.v4;
        return result;
    }
    //--- Method to Rotate vector around y-axis
    vector4 rotateY(double theta){    //--- Theta is in degrees
        vector4 result,temp = homogenize();
        theta *= pi/180;
        result.v1 = temp.v1*cos(theta)+temp.v3*sin(theta);
        result.v2 = temp.v2;
        result.v3 = 0-temp.v1*sin(theta)+temp.v3*cos(theta);
        result.v4 = temp.v4;
        return result;
    }
    //--- Method to Rotate vector around z-axis
    vector4 rotateZ(double theta){    //--- Theta is in degrees
        vector4 result,temp = homogenize();
        theta *= pi/180;
        result.v1 = temp.v1*cos(theta)-temp.v2*sin(theta);
        result.v2 = temp.v1*sin(theta)+temp.v2*cos(theta);
        result.v3 = temp.v3;
        result.v4 = temp.v4;
        return result;
    }
    //--- Rotation about arbitrary axis
    vector4 rotate(vector4 axis, double a, double b, double c, double theta){ //--- Theta is in degrees
        vector4 result,temp = homogenize();
        axis = axis.homogenize();
        theta *= pi/180;
        double l = sqrt(pow(axis.v1,2) + pow(axis.v2,2) + pow(axis.v3,2));
        double u = axis.v1/l, v = axis.v2/l, w = axis.v3/l;
        double u2 = pow(u,2), v22 = pow(v,2), w2 = pow(w,2);
        double cc = cos(theta), s = sin(theta), c1 = 1-cc;
        
        result.v1 = v1*(u2+(v22+w2)*cc) + v2*(u*v*c1-w*s) + v3*(u*w*c1+v*s) + v4*((a*(v22+w2)-u*(b*v+c*w))*c1+(b*w-c*v)*s);
        result.v2 = v1*(u*v*c1+w*s) + v2*(v22+(u2+w2)*cc) + v3*(v*w*c1-u*s) + v4*((b*(u2+w2)-v*(a*u+c*w))*c1+(c*u-a*w)*s);
        result.v3 = v1*(u*w*c1-v*s) + v2*(v*w*c1+u*s) + v3*(w2+(u2+v22)*cc) + v4*((c*(u2+v22)-w*(a*u+b*v))*c1+(a*v-b*u)*s);
        result.v4 = v1*(0) + v2*(0) + v3*(0) + v4*(1);
        
        return result;            
    }
};

//--- MATRIX CLASS ---//
class matrix4
{
    public:
    double mat[4][4];
    //--- Constructor definition to initialise matrix to ZERO matrix by default
    matrix4()
    {
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                mat[i][j] = 0;
            }
        }
    }
    //--- Method to print matrix4 object
    void print(){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                cout << mat[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
        return;
    }
    //--- "+" Operator overloading for the addition of two matrix4 objects
    matrix4 operator + (matrix4 m){
        matrix4 result;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                result.mat[i][j] = mat[i][j] - m.mat[i][j];
            }
        }
        return result;
    }
    //--- "-" Operator overloading for the subtraction of two matrix4 objects
    matrix4 operator - (matrix4 m){
        matrix4 result;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                result.mat[i][j] = mat[i][j] + m.mat[i][j];
            }
        }
        return result;
    }
    //--- "*" Operator overloading to implement right multiplication of a vector with a scalar
    matrix4 operator * (double scalar){
        matrix4 result;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                result.mat[i][j] = scalar*mat[i][j];
            }
        }
        return result;        
    }
    //--- "*" Operator overloading to implement multiplication of matrix with vector4
    vector4 operator * (vector4 v){
        vector4 result;
        result.v1 = mat[0][0]*v.v1 + mat[0][1]*v.v2 + mat[0][2]*v.v3 + mat[0][3]*v.v4;
        result.v2 = mat[1][0]*v.v1 + mat[1][1]*v.v2 + mat[1][2]*v.v3 + mat[1][3]*v.v4;
        result.v3 = mat[2][0]*v.v1 + mat[2][1]*v.v2 + mat[2][2]*v.v3 + mat[2][3]*v.v4;
        result.v4 = mat[3][0]*v.v1 + mat[3][1]*v.v2 + mat[3][2]*v.v3 + mat[3][3]*v.v4;
        return result;
    }
    //--- "*" Operator overloading to implement matrix multiplication
    matrix4 operator * (matrix4 m){
        matrix4 result;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                for(int k=0;k<4;k++){
                    result.mat[i][k] += mat[i][j] * m.mat[j][k];
                }
            }
        }
        return result;
    }
    //--- Method to implement transpose of a matrix
    matrix4 transpose(){
        matrix4 result;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                result.mat[i][j] = mat[j][i];
            }
        }
        return result;
    }
};

//--- Global Definition of Variables

IFS_DATA* data;     //--- data read from "ifs" file is stored here
vector4 look(0,0,-1,1); //--- Look vector of Camera
vector4 up(0,1,0,1);    //--- Up vector of Camera
vector4 lu(-1,0,0,1);   //--- Third vector representing camera which is perpendicular to both look and up vector
float position[4] = {0,0,1,0};  //--- initial position of camera
float thetaX=pi/2,thetaY=pi/2,thetaZ=0; //--- 

//--- Methods implemented below "scale", "translate", "rotate" will be used as replacement of "gluLookat" function
//--- These methods are used collectively in various combinations in order to give same functionality as "gluLookat" function

//--- Function implemented to scale the current matrix (MODELVIEW MATRIX)
void scale(double s1,double s2,double s3){
        GLfloat matrix[16];
        matrix4 m;
        glGetFloatv(GL_MODELVIEW_MATRIX,matrix);    //--- Get Current Model View Matrix
        //--- Converting openGL's 1-D coloumn major matrix to 2-D matrix
        for(int i=0;i<4;i++){
            m.mat[0][i] = matrix[4*i];
            m.mat[1][i] = matrix[4*i+1];
            m.mat[2][i] = matrix[4*i+2];
            m.mat[3][i] = matrix[4*i+3];               
        }
        
        //--- Scale Matrix
        matrix4 arb;
        arb.mat[0][0] = s1; 
        arb.mat[0][1] = 0; 
        arb.mat[0][2] = 0; 
        arb.mat[0][3] = 0;
        arb.mat[1][0] = 0; 
        arb.mat[1][1] = s2; 
        arb.mat[1][2] = 0; 
        arb.mat[1][3] = 0;
        arb.mat[2][0] = 0; 
        arb.mat[2][1] = 0; 
        arb.mat[2][2] = s3; 
        arb.mat[2][3] = 0;
        arb.mat[3][0] = 0; 
        arb.mat[3][1] = 0; 
        arb.mat[3][2] = 0; 
        arb.mat[3][3] = 1;
        
        //--- Scaling being performed                  
        matrix4 result;
        result = m*arb;
        
        //--- Converting 2-D matrix to openGL's 1-D coloumn major matrix
        for(int i=0;i<4;i++){
            matrix[4*i] = result.mat[0][i];
            matrix[4*i+1] = result.mat[1][i];
            matrix[4*i+2] = result.mat[2][i];
            matrix[4*i+3] = result.mat[3][i];                   
        }
        glLoadMatrixf(matrix);  //--- Loading changes to current MODELVIEW MATRIX
        return;           
}

//--- Function implemented to translate the current matrix (MODELVIEW MATRIX)
void translate(double x, double y, double z){   
        GLfloat matrix[16];
        matrix4 m;
        glGetFloatv(GL_MODELVIEW_MATRIX,matrix);    //--- Get Current Model View Matrix
        //--- Converting openGL's 1-D coloumn major matrix to 2-D matrix
        for(int i=0;i<4;i++){
            m.mat[0][i] = matrix[4*i];
            m.mat[1][i] = matrix[4*i+1];
            m.mat[2][i] = matrix[4*i+2];
            m.mat[3][i] = matrix[4*i+3];               
        }
        
        //--- Translate matrix
        matrix4 arb;
        arb.mat[0][0] = 1; 
        arb.mat[0][1] = 0; 
        arb.mat[0][2] = 0; 
        arb.mat[0][3] = x;
        arb.mat[1][0] = 0; 
        arb.mat[1][1] = 1; 
        arb.mat[1][2] = 0; 
        arb.mat[1][3] = y;
        arb.mat[2][0] = 0; 
        arb.mat[2][1] = 0; 
        arb.mat[2][2] = 1; 
        arb.mat[2][3] = z;
        arb.mat[3][0] = 0; 
        arb.mat[3][1] = 0; 
        arb.mat[3][2] = 0; 
        arb.mat[3][3] = 1;
               
        //--- translation being performed                  
        matrix4 result;
        result = m*arb;
        
        //--- Converting 2-D matrix to openGL's 1-D coloumn major matrix
        for(int i=0;i<4;i++){
            matrix[4*i] = result.mat[0][i];
            matrix[4*i+1] = result.mat[1][i];
            matrix[4*i+2] = result.mat[2][i];
            matrix[4*i+3] = result.mat[3][i];                   
        }
        glLoadMatrixf(matrix);  //--- Loading changes to current MODELVIEW MATRIX
        return;       
}

//--- Function implemented to rotate the current matrix (MODELVIEW MATRIX)
void rotate(vector4 axis,double a,double b,double c,double theta){
        axis = axis.homogenize();
        theta *= pi/180;
        double l = sqrt(pow(axis.v1,2) + pow(axis.v2,2) + pow(axis.v3,2));
        double u = axis.v1/l, v = axis.v2/l, w = axis.v3/l;
        double u2 = pow(u,2), v22 = pow(v,2), w2 = pow(w,2);
        double cc = cos(theta), s = sin(theta), c1 = 1-cc;
        GLfloat matrix[16];
        matrix4 m;
        glGetFloatv(GL_MODELVIEW_MATRIX,matrix);    //--- Get Current Model View Matrix
        //--- Converting openGL's 1-D coloumn major matrix to 2-D matrix
        for(int i=0;i<4;i++){
            m.mat[0][i] = matrix[4*i];
            m.mat[1][i] = matrix[4*i+1];
            m.mat[2][i] = matrix[4*i+2];
            m.mat[3][i] = matrix[4*i+3];               
        }
        matrix4 arb;
        
        //--- ROTATION MATRIX
        arb.mat[0][0] = u2+(v22+w2)*cc; 
        arb.mat[0][1] = u*v*c1-w*s; 
        arb.mat[0][2] = u*w*c1+v*s; 
        arb.mat[0][3] = (a*(v22+w2)-u*(b*v+c*w))*c1+(b*w-c*v)*s;
        arb.mat[1][0] = u*v*c1+w*s; 
        arb.mat[1][1] = v22+(u2+w2)*cc; 
        arb.mat[1][2] = v*w*c1-u*s; 
        arb.mat[1][3] = (b*(u2+w2)-v*(a*u+c*w))*c1+(c*u-a*w)*s;
        arb.mat[2][0] = u*w*c1-v*s; 
        arb.mat[2][1] = v*w*c1+u*s; 
        arb.mat[2][2] = w2+(u2+v22)*cc; 
        arb.mat[2][3] = (c*(u2+v22)-w*(a*u+b*v))*c1+(a*v-b*u)*s;
        arb.mat[3][0] = 0; 
        arb.mat[3][1] = 0; 
        arb.mat[3][2] = 0; 
        arb.mat[3][3] = 1;
         
        //--- Rotation being performed                  
        matrix4 result;
        result = arb*m;
        
        //--- Converting 2-D matrix to openGL's 1-D coloumn major matrix
        for(int i=0;i<4;i++){
            matrix[4*i] = result.mat[0][i];
            matrix[4*i+1] = result.mat[1][i];
            matrix[4*i+2] = result.mat[2][i];
            matrix[4*i+3] = result.mat[3][i];                   
        }
        glLoadMatrixf(matrix);  //--- Loading changes to current MODELVIEW MATRIX
        return;       
}


//--- Method to get normal vector to two given vectors which is being used in implementation of "lighting"
vector4 getNormal(vector4 a, vector4 b){
    vector4 result;
    result.v1 = 1*(a.v2*b.v3-a.v3*b.v2);
    result.v2 = 1*(a.v3*b.v1-a.v1*b.v3);
    result.v3 = 1*(a.v1*b.v2-a.v2*b.v1);
    double v = pow(result.v1,2)+pow(result.v2,2)+pow(result.v3,2);
    v = sqrt(v);
    result.v1/=v;
    result.v2/=v;
    result.v3/=v;
    result.v4 = 1;
    return result;
}

//--- DISPLAY FUNCTION
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TRIANGLE* ptr = data->triangles;
    VERTEX* pt = data->vertices;
    
    //--- Displaying World Co-ordinates axis
    glBegin(GL_POINTS);
        for(float i =0;i<=1;i+=0.005){
            glColor3f(1,0,0);   //--- X-axis is red-coloured(*** ONLY WHEN LIGHTING IS OFF***)
            glVertex3f(i,0,0);
            glColor3f(0,1,0);   //--- Y-axis is green-coloured(*** ONLY WHEN LIGHTING IS OFF***)
            glVertex3f(0,i,0);
            glColor3f(0,0,1);   //--- Z-axis is blue-coloured(*** ONLY WHEN LIGHTING IS OFF***)
            glVertex3f(0,0,i);
        }
    glEnd();
    
    //--- Displaying Object from data of "ifs" file
    glColor3f(1,1,1);   //--- White in color
    glBegin(GL_TRIANGLES);
        for(int i=0;i<data->numTriangles;i++){
            vector4 a(ptr->a->x - ptr->b->x, ptr->a->y - ptr->b->y, ptr->a->z - ptr->b->z, 1),b(ptr->a->x - ptr->c->x, ptr->a->y - ptr->c->y, ptr->a->z - ptr->c->z, 1);        //--- a,b are two vectors in the surface of triangle drawn
            a = getNormal(a,b); //--- calculating outward normal to triangle
            glVertex3f(ptr->a->x,ptr->a->y,ptr->a->z);
            glNormal3f(a.v1,a.v2,a.v3);     //--- Normal Supplied to help in lighting
            glVertex3f(ptr->b->x,ptr->b->y,ptr->b->z);
            glNormal3f(a.v1,a.v2,a.v3);     //--- Normal Supplied to help in lighting
            glVertex3f(ptr->c->x,ptr->c->y,ptr->c->z);
            glNormal3f(a.v1,a.v2,a.v3);     //--- Normal Supplied to help in lighting
            ptr++;
        }
    glEnd();
    glFlush();
    return;
}


//--- Function to handle reshaping window
void reshape(int w, int h){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);    //--- Setting view size to newly obtained width and height
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-1, 1, -1*(GLfloat)h/(GLfloat)w,1*(GLfloat)h/(GLfloat)w, -1, 1);
    else
        glOrtho (-1*(GLfloat)w/(GLfloat)h,1*(GLfloat)w/(GLfloat)h, -1, 1, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return;
}

//--- FUNCTION to HANDLE KEYPRESS SIGNAL OF glut
void keyPressed(unsigned char key, int x, int y){
    double angle = 4;
    if(key=='r'){
        data = load_ifs_file("ifs/SOLDER.ifs");
        glutPostRedisplay();
    }
    
    //--- Object Rotation
    if(key=='U' || key=='u'){
        TRIANGLE *ptr = data->triangles;
        VERTEX *pt = data->vertices;
        vector4 v;
        for(int i=0;i<data->numVertices;i++){
            v.v1 = pt->x;
            v.v2 = pt->y;
            v.v3 = pt->z;
            v.v4 = 1;
            if(key=='U') v = v.rotateX(angle);
            if(key=='u') v = v.rotateX(-1*angle);
            pt->x = v.v1;
            pt->y = v.v2;
            pt->z = v.v3;
            pt++;
        }
        glutPostRedisplay();
    }
    if(key=='V' || key=='v'){
        TRIANGLE *ptr = data->triangles;
        VERTEX *pt = data->vertices;
        vector4 v;
        for(int i=0;i<data->numVertices;i++){
            v.v1 = pt->x;
            v.v2 = pt->y;
            v.v3 = pt->z;
            v.v4 = 1;
            if(key=='V') v = v.rotateY(angle);
            if(key=='v') v = v.rotateY(-1*angle);
            pt->x = v.v1;
            pt->y = v.v2;
            pt->z = v.v3;
            pt++;
        }
        glutPostRedisplay();
    }
    if(key=='W' || key=='w'){
        TRIANGLE *ptr = data->triangles;
        VERTEX *pt = data->vertices;
        vector4 v;
        for(int i=0;i<data->numVertices;i++){
            v.v1 = pt->x;
            v.v2 = pt->y;
            v.v3 = pt->z;
            v.v4 = 1;
            if(key=='W') v = v.rotateZ(angle);
            if(key=='w') v = v.rotateZ(-1*angle);
            pt->x = v.v1;
            pt->y = v.v2;
            pt->z = v.v3;
            pt++;
        }
        glutPostRedisplay();
    }
    //---
    
    //--- Camera rotation
    if(key=='k' || key=='K'){   //--- rotating about look vector
        if(key=='k'){           //--- ANTI-Clockwise rotation(looking from -z direction)
            rotate(look,position[0],position[1],position[2],angle);
            thetaZ += angle;
        }
        if(key=='K'){           //--- Clockwise rotation(looking from -z direction)
            rotate(look,position[0],position[1],position[2],-1*angle);
            thetaZ -= angle;
        }
        glutPostRedisplay();    
    }
    if(key=='j' || key=='J'){   //--- Rotating about up vector
        if(key=='j'){           //--- ANTI-Clockwise rotation(looking from -z direction)
            rotate(up,position[0],position[1],position[2],angle);
            
        }
        if(key=='J'){           //--- Clockwise rotation(looking from -z direction)
            rotate(up,position[0],position[1],position[2],-1*angle);
        }
        glutPostRedisplay();        
    }
    if(key=='i' || key=='I'){   //--- rotating about camera vector which is perpendicular to both look and up vector
        if(key=='i') rotate(lu,position[0],position[1],position[2],angle);  //--- ANTI-Clockwise rotation(looking from -z direction)
        if(key=='I') rotate(lu,position[0],position[1],position[2],-1*angle);//--- Clockwise rotation(looking from -z direction)
        glutPostRedisplay();    
    }
    //---
    
    //--- Translate Camera
    if(key=='x' || key=='X'){   
        if(key=='x'){   //--- Translating camera in +x direction ( Effectively moving world co-ordinate in -x direction)
            translate(-0.05,0,0);
            position[0]+=0.05;
        }
        if(key=='X'){   //--- Translating camera in -x direction ( Effectively moving world co-ordinate in +x direction)
            translate(0.05,0,0);
            position[0]-=0.05;
        }
        glutPostRedisplay();
    }
    if(key=='y' || key=='Y'){   //--- Translating camera in +y direction ( Effectively moving world co-ordinate in -y direction)
        if(key=='y'){
            translate(0,-0.05,0);
            position[1]+=0.05;
        }
        if(key=='Y'){           //--- Translating camera in -y direction ( Effectively moving world co-ordinate in +y direction)
            translate(0,0.05,0);
            position[1]-=0.05;
        }
        glutPostRedisplay();
    }
    if(key=='z' || key=='Z'){   //--- Translating camera in +z direction ( Effectively moving world co-ordinate in -z direction)
        if(key=='z'){
            translate(0,0,-0.05);
            position[2]+=0.05;
        } 
        if(key=='Z'){           //--- Translating camera in -z direction ( Effectively moving world co-ordinate in +z direction)
            translate(0,0,0.05);
            position[2]-=0.05;
        }
        glutPostRedisplay();
    }

    //--- Moving Camera front and back
    if(key=='a' || key=='A'){
        if(key=='a') scale(0.9,0.9,0.9);
        if(key=='A') scale(1.2,1.2,1.2);
        glutPostRedisplay();
    }
    
    //--- Object Scaling
    if(key=='+'){
        VERTEX *pt = data->vertices;
        vector4 v;
        for(int i=0;i<data->numVertices;i++){
            v.v1 = pt->x;
            v.v2 = pt->y;
            v.v3 = pt->z;
            v.v4 = 1;
            v = v.scale(1.2,1.2,1.2);
            pt->x = v.v1;
            pt->y = v.v2;
            pt->z = v.v3;
            pt++;
        }
        glutPostRedisplay();    
    }
    if(key=='-'){
        VERTEX *pt = data->vertices;
        vector4 v;
        for(int i=0;i<data->numVertices;i++){
            v.v1 = pt->x;
            v.v2 = pt->y;
            v.v3 = pt->z;
            v.v4 = 1;
            v = v.scale(0.9,0.9,0.9);
            pt->x = v.v1;
            pt->y = v.v2;
            pt->z = v.v3;
            pt++;
        }
        glutPostRedisplay();
    }
    
    //--- FILL OR LINE
    if(key=='f' || key=='F'){
         glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );   
         glutPostRedisplay();
    }
    if(key=='e' || key=='E'){
         glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
         glutPostRedisplay();
    }
    //--- Light on or off
    if(key=='l'){
        glEnable(GL_LIGHTING);
        glutPostRedisplay();
    }
    if(key=='L'){
        glDisable(GL_LIGHTING);
        glutPostRedisplay();
    }
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(800,600);
    glutCreateWindow(argv[0]);
    
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
//    gluPerspective(60,1,-1,1);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glClearColor (0.0, 0.0, 0.0, 0.0);
       
    GLfloat material_ambient[] = {0.25f, 0.25f, 0.25f};
    GLfloat material_diffuse[] = {0.90f, 0.90f, 0.90f};
    GLfloat material_specular[] = {0.90f, 0.90f, 0.90f};
    GLfloat material_shininess = 25.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);

    GLfloat light0_diffuse[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat light0_position[] = {-10.0, 10.0, 10.0, 1.0};
    GLfloat light1_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat light1_position[] = {10.0, 10.0, 10.0, 1.0};

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glEnable(GL_LIGHTING);
    
    glShadeModel(GL_SMOOTH);      
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    data = load_ifs_file(argv[1]);
        
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
