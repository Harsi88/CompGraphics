#include<stdio.h>
#include<iostream>
#include<math.h>

using namespace std;
#include<SOIL/SOIL.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<string.h>

#define WIDTH 1000
#define HEIGHT 496
#define RADIUS 15

#define PI 3.14159265358979323846
#define d2r PI/180
#define r2d 180/PI

class ball{
public:
    double x,y; //--- Position
    double radius;
    double momentum;
    double direction;   //--- Angle in deegrees
    bool isVisible;
    ball(){
        x=y=momentum=0;
        direction = 0;
        radius = RADIUS;
        isVisible = true;
    }
    ball(double x,double y){
        this->x = x;
        this->y = y;
        radius = RADIUS;
        momentum = 0;
        isVisible = true;
    }
};

class stick{
public:
    double x1,y1,x2,y2; 
    double force;
    stick(){
        x1 = y1 = x2 = y2 = force = 0;
    }
    stick(double x1,double y1,double x2,double y2){
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
        force = 0;
    }
    void print(){
        printf("x1:%lf -- y1:%lf -- x2:%lf -- y2:%lf -- force:%lf\n",x1,y1,x2,y2,force);
    }
};

stick cue;
double cue_theta,dist,theta,thetai,thetaj,mix,mjx,miy,mjy,temp,lastColl[16][16],px,py;
int pocketx,pockety,pocketr,timer,ttime,i,j;
bool momentum,start,visible;
ball Ball[16];
char scoreArr[3000];
string tmp;
const char* c;

void bprint(){
    for(i=0;i<16;i++)
        printf("x: %lf-- y:%lf-- r:%lf-- m:%lf-- v:%d\n",Ball[i].x,Ball[i].y,Ball[i].radius,Ball[i].momentum,Ball[i].isVisible);
}

void boundaryCollision(){
    for(i=0;i<16;i++){
        if(Ball[i].x <= (-455+RADIUS)){
            if(Ball[i].y > -195 && Ball[i].y < 195){
                Ball[i].direction = 180 - Ball[i].direction;
                if(Ball[i].direction < 0) Ball[i].direction+=360;
                Ball[i].x = -450+RADIUS;
            }
        }
        if(Ball[i].x >= (455-RADIUS)){
            if(Ball[i].y > -190 && Ball[i].y < 190){
                Ball[i].direction = 180 - Ball[i].direction;
                Ball[i].x = 450-RADIUS;
            }    
        }
        if(Ball[i].y >= (200-RADIUS)){
            if((Ball[i].x > 10 && Ball[i].x < 430) || ((Ball[i].x > -430 && Ball[i].x < -10))){
                Ball[i].direction = 360 - Ball[i].direction;
                Ball[i].y = 200-RADIUS;
            }
        }
        if(Ball[i].y <= (-200+RADIUS)){
            if((Ball[i].x > 10 && Ball[i].x < 430) || ((Ball[i].x > -430 && Ball[i].x < -10))){
                Ball[i].direction = 360 - Ball[i].direction;
                Ball[i].y = -200+RADIUS;
            }
        }
    }    
}

void ballCollision(){
    for(i=0;i<16;i++){
        for(j=i+1;j<16;j++){
            //--- Collisions between balls
            if(i!=j && Ball[i].isVisible && Ball[j].isVisible){
                dist = pow((Ball[i].x - Ball[j].x),2) + pow((Ball[i].y - Ball[j].y),2);                
                dist = sqrt(dist);
                if(dist <= (2*RADIUS)){
                    //--- There is a collision
                    if(ttime - lastColl[i][j] < 3*timer) continue;
                    lastColl[i][j] = ttime;
                    if( fabs(Ball[j].x - Ball[i].x) < 1e-10 ){   //--- aTan(inf) case
                        if(Ball[j].y >= Ball[i].y) theta = 90;
                        else theta = 270;
                    }
                    else if( fabs(Ball[j].y - Ball[i].y) < 1e-10){
                        if(Ball[j].x >= Ball[i].x) theta = 0;
                        else theta = 180;
                    }
                    else if(Ball[j].x > Ball[i].x  && Ball[j].y > Ball[i].y){
                        theta = ( atan( (Ball[i].y-Ball[j].y)/(Ball[i].x-Ball[j].x) ) )*r2d;
                    }
                    else if(Ball[j].x < Ball[i].x  && Ball[j].y > Ball[i].y){
                        theta = ( atan( (Ball[i].y-Ball[j].y)/(Ball[i].x-Ball[j].x) ) )*r2d + 180;
                    }
                    else if(Ball[j].x < Ball[i].x  && Ball[j].y <= Ball[i].y){
                        theta = ( atan( (Ball[i].y-Ball[j].y)/(Ball[i].x-Ball[j].x) ) )*r2d + 180;
                    }
                    else if(Ball[j].x > Ball[i].x  && Ball[j].y <= Ball[i].y){
                        theta = ( atan( (Ball[i].y-Ball[j].y)/(Ball[i].x-Ball[j].x) ) )*r2d + 360;
                    }
                    
//                    while(dist < 2*RADIUS){
                            if(Ball[i].momentum>0){
                            Ball[i].x -= (Ball[i].momentum+0.025)*cos(Ball[i].direction*d2r);
                            Ball[i].y -= (Ball[i].momentum+0.025)*sin(Ball[i].direction*d2r);
                            }
                            if(Ball[j].momentum>0){
                            Ball[j].x -= (Ball[j].momentum+0.025)*cos(Ball[j].direction*d2r);
                            Ball[j].y -= (Ball[j].momentum+0.025)*sin(Ball[j].direction*d2r);
                            }
                            dist = pow((Ball[i].x - Ball[j].x),2) + pow((Ball[i].y - Ball[j].y),2);                
                            dist = sqrt(dist);
//                    }
                                        
                    thetai = Ball[i].direction - theta;
                    thetaj = Ball[j].direction - theta;
                    
                    mix = Ball[i].momentum*cos(thetai*d2r);
                    miy = Ball[i].momentum*sin(thetai*d2r);
                    
                    mjx = Ball[j].momentum*cos(thetaj*d2r);
                    mjy = Ball[j].momentum*sin(thetaj*d2r);
                    
                    temp = mix;
                    mix = mjx;
                    mjx = temp;
                    
                    if(fabs(mix)<1e-9) mix=0;
                    if(fabs(miy)<1e-9) miy=0;
                    if(fabs(mjx)<1e-9) mjx=0;
                    if(fabs(mjy)<1e-9) mjy=0;                  
                    
                    Ball[i].momentum = sqrt( pow(mix,2)+pow(miy,2) );
                    Ball[j].momentum = sqrt( pow(mjx,2)+pow(mjy,2) );
                                       
                    if(mix==0){//--- atan(inf) case
                        if(miy > 0) thetai = 90;
                        else if(miy<0) thetai = 270;
                    }
                    else if(mix >= 0 && miy >= 0)
                        thetai = ( atan( miy/mix ) )*r2d;
                    else if(mix < 0 && miy >= 0)
                        thetai = ( atan( miy/mix ) )*r2d + 180;
                    else if(mix < 0 && miy <= 0)
                        thetai = ( atan( miy/mix ) )*r2d + 180;
                    else if(mix > 0 && miy <=0 )
                        thetai = ( atan( miy/mix ) )*r2d + 360;
                    
                    if(thetai > 360)  thetai -= 360;
                    
                    if(mjx==0){
                        if(mjy>0) thetaj = 90;
                        else if(mjy<0) thetaj = 270;
                    }
                    else if(mjx >= 0 && mjy >= 0) thetaj = ( atan( mjy/mjx ) )*r2d;
                    else if(mjx < 0 && mjy >= 0) thetaj = ( atan( mjy/mjx ) )*r2d + 180;
                    else if(mjx < 0 && mjy <= 0) thetaj = ( atan( mjy/mjx ) )*r2d + 180;
                    else if(mjx > 0 && mjy <=0 ) thetaj = ( atan( mjy/mjx ) )*r2d + 360;

                    if(thetaj > 360)  thetaj -= 360;

                    Ball[i].direction = thetai + theta;
                    Ball[j].direction = thetaj + theta;
                }
            }
        }
        //--- Collisions with pockets
        if(Ball[i].isVisible){
            pocketr=10;
            dist = pow(Ball[i].x - pocketx,2) + pow((Ball[i].y - pockety),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;
            
            dist = pow(Ball[i].x + pocketx,2) + pow((Ball[i].y - pockety),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;

            dist = pow(Ball[i].x + pocketx,2) + pow((Ball[i].y + pockety),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;

            dist = pow(Ball[i].x - pocketx,2) + pow((Ball[i].y + pockety),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;

            dist = pow(Ball[i].x,2) + pow((Ball[i].y - 210),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;

            dist = pow(Ball[i].x,2) + pow((Ball[i].y + 210),2);
            dist = sqrt(dist);
            if(dist < RADIUS+pocketr)
                Ball[i].isVisible = false;
        }
    }
}

void drawSphere(double x,double y,double rad,int number){
    double currentcolor[3];
    glGetDoublev(GL_CURRENT_COLOR,currentcolor);
    if(number==0)
        glColor3f(1,1,1);
    else if(number==16)
        glColor3f(0,0,0);
    else if(number==1 || number==9)
        glColor3f(1,1,0);
    else if(number==2 || number==10)
        glColor3f(0,0,1);
    else if(number==3 || number==11)
        glColor3f(1,0,0.2);
    else if(number==4 || number==12)
        glColor3f(0.2,0,0.56);
    else if(number==5 || number==13)
        glColor3f(0.8,0.2,0);
    else if(number==6 || number==14)
        glColor3f(0,0.2,0);
    else if(number==7 || number==15)
        glColor3f(0.4,0,0);
    else if(number==8)
        glColor3f(0,0,0);
    else
        glColor3f(0.5,0.4,0.4);
    glTranslatef(x,y,0);
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluSphere(quad,rad,20,20);
    glTranslatef(-x,-y,0);
    glFlush();
    glColor3f(currentcolor[0],currentcolor[1],currentcolor[2]);
}

static GLuint LoadPNG(char* filename)
{
    GLuint texture = SOIL_load_OGL_texture
    (
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (texture == 0)
        cout<<"Texture Load Error: " << string(filename)<<endl;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void pauseWindow()
{ 
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-100, 210, 0);
    tmp = "BILLIARDS";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

    glColor3f(1.0, 1.0, 1.0);    
    glRasterPos3f(-490, 170, 0);
    tmp = "CONTROLS:";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

    glRasterPos3f(-490, 130, 0);
    tmp = "U -> Rotate Cue Anti-clockwise. --- I and O for fine Rotation";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

    glRasterPos3f(-490, 90, 0);
    tmp = "J -> Rotate Cue Clockwise. --- K and L for fine Rotation";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

    glRasterPos3f(-490, 50, 0);
    tmp = "Z, X, C, Space -> Strike the white ball with cue. --- Z=Least Force and Space=Max Force";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position
    
    glRasterPos3f(-490, 10, 0);
    tmp = "E -> Pause the Game";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position
    
    glRasterPos3f(-490, -30, 0);
    tmp = "R -> Reset the Game";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

    glRasterPos3f(-490, -70, 0);
    tmp = "Q -> Quit the Game";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position
    
    glRasterPos3f(-150, -125, 0);
    tmp = "S -> START THE GAME";
    strcpy(scoreArr, tmp.c_str());
    for (c=scoreArr; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position

}

void drawStick(double x1,double y1,double x2,double y2){
    glColor3f(1,1,1);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(x1,y1,2);
        glVertex3f(x2,y2,2);
    glEnd();
    glFlush();
    cue.x1 = x1;
    cue.y1 = y1;
    cue.x2 = x2;
    cue.y2 = y2;
}

void drawThinStick(double x1,double y1,double x2,double y2){
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex3f(x1,y1,1);
        glVertex3f(x2,y2,1);
    glEnd();
    glFlush();
    cue.x1 = x1;
    cue.y1 = y1;
    cue.x2 = x2;
    cue.y2 = y2;
}

void drawScene(GLuint tex){

    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(-500,250);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(500,250);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(500,-250);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(-500,-250);
    glEnd();
    //--- Draw Pockets
    pocketx=450,pockety=200;
    drawSphere(-pocketx,pockety-5,20,16);
    drawSphere(pocketx,pockety-5,20,16);
    drawSphere(pocketx,-pockety,20,16);
    drawSphere(-pocketx,-pockety,20,16);
    //Middle Pockets
    drawSphere(0,210,20,16);
    drawSphere(0,-210,20,16);
    
    //--- Draw Balls        
    momentum = false;
    for(i=0;i<16;i++){
        if(Ball[i].isVisible){
            drawSphere(Ball[i].x,Ball[i].y,RADIUS,i);
            if(Ball[i].momentum>0) momentum = true;
        }
    }

    //--- Draw Cue
    if(!momentum){
        if(!Ball[0].isVisible){
            Ball[0].isVisible = true;
            Ball[0].x = -250;
            Ball[0].y = 0;
            Ball[0].momentum = 0;
            Ball[0].direction = 0;
        }
        drawStick(Ball[0].x+20*cos(cue_theta*d2r),Ball[0].y + 20*sin(cue_theta*d2r),Ball[0].x+300*cos(cue_theta*d2r),Ball[0].y+300*sin(cue_theta*d2r));
        drawThinStick(Ball[0].x-20*cos(cue_theta*d2r),Ball[0].y - 20*sin(cue_theta*d2r),Ball[0].x-400*cos(cue_theta*d2r),Ball[0].y-400*sin(cue_theta*d2r));
    }
    return;
}

void reset(){
    start=false;
    px = 180,py = 0;
    //--- White ball
    Ball[0].x = -250;  Ball[0].y = 0;
    //--- First Layer
    Ball[1].x = px;  Ball[1].y = py;
    //--- Second Layer
    Ball[2].x = px+25;  Ball[2].y = py-18;
    Ball[3].x = px+25;  Ball[3].y = py+18;
    //--- Third Layer
    Ball[4].x = px+50;  Ball[4].y = py;
    Ball[5].x = px+50;  Ball[5].y = py-36;
    Ball[6].x = px+50;  Ball[6].y = py+36;
    //--- Fourth Layer
    Ball[7].x = px+75;  Ball[7].y = py-18;
    Ball[8].x = px+75;  Ball[8].y = py+18;
    Ball[9].x = px+75;  Ball[9].y = py-54;
    Ball[10].x = px+75;  Ball[10].y = py+54;
    //--- Fifth Layer
    Ball[11].x = px+100;  Ball[11].y = py;
    Ball[12].x = px+100;  Ball[12].y = py-36;
    Ball[13].x = px+100;  Ball[13].y = py+36;
    Ball[14].x = px+100;  Ball[14].y = py-72;
    Ball[15].x = px+100;  Ball[15].y = py+72;
    
    dist = theta = temp = pocketr = pocketx = pockety = 0;
    momentum = false;
    cue_theta = 180;
    timer = 1;
    ttime = -3*timer;
    for(i=0;i<16;i++){
        Ball[i].momentum = Ball[i].direction = 0;
        Ball[i].isVisible = true;
        for(j=0;j<16;j++){
            lastColl[i][j] = 0;
        }
    }
    
}

void timerfunc(int value){
    ttime += timer;
    for(i=0;i<16;i++){
        if(Ball[i].isVisible && Ball[i].momentum > 0){
            Ball[i].x += Ball[i].momentum*cos(Ball[i].direction*d2r);
            Ball[i].y += Ball[i].momentum*sin(Ball[i].direction*d2r);
            Ball[i].momentum -= 0.025;
            if(Ball[i].momentum < 0) Ball[i].momentum = 0;
        }
    }
    ballCollision();
    boundaryCollision();
    glutTimerFunc (timer, timerfunc, 2);
    glutPostRedisplay();
}

void keyboardlistener(unsigned char key,int x,int y){
    if( (key == 'u' || key == 'U') && start){
        //--- Rotating cue
        cue_theta += 5;
        if(cue_theta >= 360) cue_theta-=360;
    }
    else if((key == 'j' || key == 'J') && start){
        //--- Rotating cue
        cue_theta -= 5;
        if(cue_theta < 0) cue_theta+=360;
    }
    if(start && (key == 'i' || key == 'I')){
        //--- Rotating cue
        cue_theta += 1;
        if(cue_theta >= 360) cue_theta-=360;
    }
    else if(start && (key == 'k' || key == 'K')){
        //--- Rotating cue
        cue_theta -= 1;
        if(cue_theta < 0) cue_theta+=360;
    }
    if(start && (key == 'o' || key == 'O')){
        //--- Rotating cue
        cue_theta += 0.2;
        if(cue_theta >= 360) cue_theta-=360;
    }
    else if(start && (key == 'l' || key == 'L')){
        //--- Rotating cue
        cue_theta -= 0.2;
        if(cue_theta < 0) cue_theta+=360;
    }
    else if(start && key == ' '){
        momentum = false;
        for(i=0;i<16;i++){
            if(Ball[i].isVisible){
                if(Ball[i].momentum>0) momentum = true;
            }
        }
        if(!momentum){
            Ball[0].momentum = 10;
            Ball[0].direction = 180+cue_theta;
            if(Ball[0].direction >= 360) Ball[0].direction -= 360;
        }
    }
    else if(start && (key == 'z' || key == 'Z')){
        momentum = false;
        for(i=0;i<16;i++){
            if(Ball[i].isVisible){
                if(Ball[i].momentum>0) momentum = true;
            }
        }
        if(!momentum){
            Ball[0].momentum = 5;
            Ball[0].direction = 180+cue_theta;
            if(Ball[0].direction >= 360) Ball[0].direction -= 360;
        }
    }    
    else if(start && (key == 'x' || key == 'X')){
        //bool momentum = false;
        momentum = false;
        for(i=0;i<16;i++){
            if(Ball[i].isVisible){
                if(Ball[i].momentum>0) momentum = true;
            }
        }
        if(!momentum){
            Ball[0].momentum = 6;
            Ball[0].direction = 180+cue_theta;
            if(Ball[0].direction >= 360) Ball[0].direction -= 360;
        }
    }
    else if(start && (key == 'c' || key=='C')){
        //bool momentum = false;
        momentum = false;
        for(i=0;i<16;i++){
            if(Ball[i].isVisible){
                if(Ball[i].momentum>0) momentum = true;
            }
        }
        if(!momentum){
            Ball[0].momentum = 8;
            Ball[0].direction = 180+cue_theta;
            if(Ball[0].direction >= 360) Ball[0].direction -= 360;
        }
    }
    else if(key == 's' || key == 'S'){
        start = true;
    }
    else if(start && (key == 'e' || key == 'E')){
        start = false;
    }
    else if(start && (key == 'r' || key=='R')){
        reset();        
    }
    else if(start && (key == 'q' || key=='Q')){
        exit(0);        
    }
}

static GLuint Texture;

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(start)
        drawScene(Texture);
    else
        pauseWindow();
    visible = false;
    for(i=1;i<16;i++){
        if(Ball[i].isVisible) visible = true;
    }
    if(!visible){ 
        start = false;
        reset();
    }
    glutSwapBuffers();
    glFlush();
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(WIDTH)/2,WIDTH/2,-(HEIGHT)/2,HEIGHT/2,-1000.0,1000.0);
}   

int main(int argc,char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Billiards");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardlistener);
    
    px = 180,py = 0;
    //--- White ball
    Ball[0].x = -250;  Ball[0].y = 0;
    //--- First Layer
    Ball[1].x = px;  Ball[1].y = py;
    //--- Second Layer
    Ball[2].x = px+25;  Ball[2].y = py-18;
    Ball[3].x = px+25;  Ball[3].y = py+18;
    //--- Third Layer
    Ball[4].x = px+50;  Ball[4].y = py;
    Ball[5].x = px+50;  Ball[5].y = py-36;
    Ball[6].x = px+50;  Ball[6].y = py+36;
    //--- Fourth Layer
    Ball[7].x = px+75;  Ball[7].y = py-18;
    Ball[8].x = px+75;  Ball[8].y = py+18;
    Ball[9].x = px+75;  Ball[9].y = py-54;
    Ball[10].x = px+75;  Ball[10].y = py+54;
    //--- Fifth Layer
    Ball[11].x = px+100;  Ball[11].y = py;
    Ball[12].x = px+100;  Ball[12].y = py-36;
    Ball[13].x = px+100;  Ball[13].y = py+36;
    Ball[14].x = px+100;  Ball[14].y = py-72;
    Ball[15].x = px+100;  Ball[15].y = py+72;
    
    dist = theta = temp = pocketr = pocketx = pockety = 0;
    momentum = false;
    visible = true;
    cue_theta = 180;
    timer = 1;
    ttime = -3*timer;
    for(i=0;i<16;i++)
        for(j=0;j<16;j++)
            lastColl[i][j] = 0;
    start=false;
    glutTimerFunc(timer,timerfunc,timer);
    Texture = LoadPNG((char*)"image");
    glutMainLoop();
    return 0;
}
