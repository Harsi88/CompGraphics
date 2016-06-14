#ifndef TURTLE_H
#define TURTLE_H

#define pi 3.14159265

void Init(float _x,float _y, float _theta);
void Forward(float dist);
void Backward(float dist);
void Right(float _theta);
void Left(float _theta);
void PenDown( bool down );
void LineColor(float r,float g, float b);
void BGColor(float r,float g, float b);
void LineWidth( float width );
void ResetPosition();
void ClearScreen();


#endif
