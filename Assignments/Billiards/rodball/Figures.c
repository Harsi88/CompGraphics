#include "Figures.h"
#include <math.h>
#include <stdio.h>

void circle(float r, float c_x, float c_y)
{
	Init( c_x, c_y, 0 );		// Move to the center (facing right)
	PenDown( false );		// Move without drawing
	Backward(r);			// Move forward (right) 50 units
	Left(90);			// Turn left 90 degrees (facing up)
	PenDown ( true );		// Start Drawing
	
	int i = 0;
	while(i < 360)
	{
		Forward(r*pi/180);
		Right(1);
		i++;
	}
}


void arbit_sector(float r, float angle, float angle_x, float c_x, float c_y)
{
	Init( c_x, c_y, 0 );		// Move to the center (facing right)
	PenDown( false );		// Move without drawing
	Left(angle_x);
	Forward(r);
	Left (90);
	PenDown(true);
	int i = 1;
	while(i <= angle)
	{
		Forward(r*pi/180);
		Left(1);
		i++;
	}
}

void sector(float r, float angle, float angle_x)
{
	PenDown( false );		// Move without drawing
	Left(angle_x);
	Forward(r);
	Left (90);
	PenDown(true);
	int i = 1;
	while(i <= angle)
	{
		Forward(r*pi/180);
		Left(1);
		i++;
	}
}

void A(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(3*height/4);
	Right(90);
	PenDown(false);
	Forward(height/4);
	sector(height/4,180,0);
	PenDown(false);
	Left(90);
	Forward(height/2);
	Right(90);
	PenDown(true);
	Forward(3*height/4);
	PenDown(false);
	Backward(height/2);
	Right(90);
	PenDown(true);
	Forward(height/2);
	Left(90);
	PenDown(false);
	Forward(height/2);
	Left(90);
}


void D(float s_x, float s_y, float height)
{
	
	Init(s_x,s_y,0);
	float r = height/2;
	PenDown(false);
	Left(90);
	Forward(r);
	Right(90);
	sector(r,180, -90);
	Left(90);
	Forward(2*r);
	Left(90);
	PenDown(false);
}	

void B(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Forward(height/4);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(90);
	sector(height/4,180,-90);
	PenDown(false);
	Right(90);
	Forward(height/4);
	Right(90);
	sector(height/4,180,-90);
	Forward(height/4);
	Left(90);
	Forward(height/2);
	Left(90);
	Forward(height/4);
	PenDown(false);
	Backward(height/4);
	Left(90);
	PenDown(true);
	Backward(height/2);
	Right(90);
}


void C(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	float r = height/2;
	PenDown(false);
	Forward(r);
	Left(90);
	Forward(r);
	Right(90);
	sector(r,180, 90);
	PenDown(false);
	Backward(r);
}

void E(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	Right(90);
	Forward(height/2);
	PenDown(false);
	Backward(height/2);
	Left(90);
	Backward(height/2);
	Right(90);
	PenDown(true);
	Forward(height/4);
	PenDown(false);
	Backward(height/4);
	Left(90);
	Backward(height/2);
	Right(90);
	PenDown(true);
	Forward(height/2);
	PenDown(false);
	Backward(height/2);
}

void F(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	Right(90);
	Forward(height/2);
	PenDown(false);
	Backward(height/2);
	Left(90);
	Backward(height/2);
	Right(90);
	PenDown(true);
	Forward(height/4);
	PenDown(false);
	Backward(height/4);
	Left(90);
	Backward(height/2);
	Right(90);
	PenDown(false);
}

void G(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	float r = height/2;
	C(s_x,s_y,height);
	PenDown(false);
	Forward(r);
	Left(90);
	PenDown(true);
	Forward(r);
	Left(90);
	Forward(r/2);
	PenDown(false);
	Left(45);
	Forward(r*sqrt(2));
	Right(135);
}



void H(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	PenDown(false);
	Backward(height/2);
	Right(90);
	PenDown(true);
	Forward(height/2);
	Left(90);
	Forward(height/2);
	PenDown(false);
	Backward(height/2);
	PenDown(true);
	Backward(height/2);
	PenDown(false);
	Right(90);
	Backward(height/2);
}
void I(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Forward(height/4);
	Left(90);
	Forward(height);
	Left(90);
	Forward(height/4);
	PenDown(false);
	Backward(height/4);
	PenDown(true);
	Backward(height/4);
	Left(90);
	PenDown(false);
	Forward(height);
	Left(90);
	PenDown(true);
	Backward(height/4);
	PenDown(false);
	Backward(height/4);
}
	
void J(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	float r = height/4;
	PenDown(false);
	Forward(r);
	Left(90);
	Forward(r);
	Right(90);
	sector(r, 180, 180);
	Forward(3*height/4);
	Left(90);
	Forward(r);
	PenDown(false);
	Forward(r);
	Left(90);
	Forward(height);
	Left(90);
}
void K(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	PenDown(false);
	Backward(height/2);
	Right(135);
	PenDown(true);
	Forward(height/sqrt(2));
	PenDown(false);
	Backward(height/sqrt(2));
	Left(90);
	PenDown(true);
	Forward(height/sqrt(2));
	PenDown(false);
	Backward(height/sqrt(2));
	Left(45);
	Backward(height/2);
	Right(45);
}
void L(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	PenDown(false);
	Backward(height);
	Right(90);
	PenDown(true);
	Forward(height/2);
	PenDown(false);
	Backward(height/2);
}

void M(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	Right(135);
	Forward(height*sqrt(2)/4);
	Left(90);
	Forward(height*sqrt(2)/4);
	Right(135);
	Forward(height);
	Left(90);
	PenDown(false);
	Backward(height/2);
}

void N(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	Right(90+63.434);
	Forward(height*sqrt(5/4));
	Left(90+63.434);
	Forward(height);
	PenDown(false);
	Right(90);
	Backward(height/2);
	Left(90);
	Backward(height);
	Right(90);
}

void O(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(90);
	PenDown(false);
	Forward(height/4);
	sector(height/4,180,180);
	Forward(height/2);
	PenDown(false);
	Right(90);
	Backward(height/4);
	sector(height/4,180,0);
	Forward(height/2);
	PenDown(false);
	Forward(height/4);
	Left(90);	
}

void P(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height/2);
	Right(90);
	Forward(height/4);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(90);
	sector(height/4,180,-90);
	Forward(height/4);
	Left(90);
	Forward(height/2);
	PenDown(false);
	Forward(height/2);
	Left(90);
}
	
void Q(float s_x, float s_y, float height)	
{
	Init(s_x,s_y,0);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(90);
	PenDown(false);
	Forward(height/4);
	sector(height/4,180,180);
	Forward(height/2);
	PenDown(false);
	Right(90);
	Backward(height/4);
	sector(height/4,180,0);
	Forward(height/2);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(45);
	PenDown(true);
	Forward(height*sqrt(2)/4);
	Left(45);	
	PenDown(false);
	Backward(height/2);
	

}	

void R(float s_x, float s_y, float height)		
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height/2);
	Right(90);
	Forward(height/4);
	PenDown(false);
	Left(90);
	Forward(height/4);
	Right(90);
	sector(height/4,180,-90);
	Forward(height/4);
	Left(90);
	Forward(height/2);
	Left(45);
	Forward(height/sqrt(2));
	PenDown(false);
	Left(45);
	Backward(height/2);
}
	
void S(float s_x, float s_y, float height)			
{
	Init(s_x,s_y,0);
	Left(90);
	Forward(height/4);
	Right(90);
	Forward(height/4);
	sector(height/4,180,180);
	PenDown(false);
	Forward(height/2);
	Left(90);
	Forward(height/4);
	Right(180);
	sector(height/4,180,0);
	Left(45);
	Forward(height/sqrt(2));
	PenDown(false);
	Left(45);
	Backward(height/2);
	Left(90);
	Backward(height/4);
	Right(90);	
}	
void T(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	Forward(height/4);
	Left(90);
	PenDown(true);
	Forward(height);
	Left(90);
	Forward(height/4);
	PenDown(false);
	Backward(height/4);
	PenDown(true);
	Backward(height/4);
	PenDown(false);
	Forward(height/2);
	Right(90);
	Backward(height);
	Right(90);
}	
void U(float s_x, float s_y, float height)				
{
	Init(s_x,s_y,0);
	PenDown(false);
	Forward(height/4);
	Left(90);
	Forward(height/4);
	Right(90);
	sector(height/4,180,180);
	Forward(3*height/4);
	Left(90);
	PenDown(false);
	Forward(height/2);
	Left(90);
	PenDown(true);
	Forward(3*height/4);
	PenDown(false);
	Forward(height/4);
	Left(90);
}

void V(float s_x, float s_y, float height)				
{
	Init(s_x,s_y,0);
	PenDown(false);
	Left(90);
	Forward(height);
	Right(90+76);
	PenDown(true);
	Forward(height*sqrt(17/16));
	Left(2*76);
	Forward(height*sqrt(17/16));
	PenDown(false);
	Backward(height*sqrt(17/16));
	Right(76);
	Backward(height/4);	
}

void W(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(90);
	Forward(height);
	PenDown(false);
	Backward(height);
	PenDown(true);
	Right(45);
	Forward(height*sqrt(2)/4);
	Right(90);
	Forward(height*sqrt(2)/4);
	Left(135);
	Forward(height);
	PenDown(false);
	Left(90);
	Forward(height/2);
	Left(90);
	Forward(height);
	Left(90);
}

void X(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(true);
	Left(63.434);
	Forward(height*sqrt(5/4));
	Right(90+63.434);
	PenDown(false);
	Forward(height);
	Right(63.434+90);
	PenDown(true);
	Forward(height*sqrt(5/4));
	Left(63.434+90);
	PenDown(false);
	Forward(height);
	Left(90);
}
void Y(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(false);
	Left(90);
	Forward(height);
	Right(135);
	PenDown(true);
	Forward(height*sqrt(2)/4);
	Left(90);
	Forward(height*sqrt(2)/4);
	PenDown(false);
	Backward(height*sqrt(2)/4);
	Left(45);
	PenDown(true);
	Backward(height/2);
	Right(90);
	PenDown(false);
	Backward(height/4);
}
void Z(float s_x, float s_y, float height)
{
	Init(s_x,s_y,0);
	PenDown(false);
	Forward(height/2);
	PenDown(true);
	Backward(height/2);
	Left(63.434);
	Forward(height*sqrt(5/4));
	Left(180-63.434);
	Forward(height/2);
	Left(90);
	PenDown(false);
	Forward(height);
	Left(90);
}
