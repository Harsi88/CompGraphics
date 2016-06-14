#include <math.h>
#include "Transformations.h"
#include <stdio.h>

using namespace std;


void Vector3::Addition(Vector3* v)
{
	for(int i = 0; i<= 2; i++)
		this->vector[i] = this->vector[i]+v->vector[i];

}

void Vector3::Substraction(Vector3* v)
{
	for(int i =0; i<= 2;i++)
			this->vector[i] = this->vector[i]-v->vector[i];
}


	
void Matrix3::Addition(Matrix3* m)
{
	for (int i = 0; i<=2; i++)
	{
		for (int j = 0; j<=2;j++)
			this->matrix[i][j] = this->matrix[i][j] + m->matrix[i][j] ;
	}
}	

void Matrix3::Substraction(Matrix3* m)
{
	for (int i = 0; i<=2; i++)
	{
		for (int j = 0; j<=2;j++)
			this->matrix[i][j] = this->matrix[i][j] - m->matrix[i][j] ;
	}
}	
		
Matrix3* Matrix3::Multiplication(Matrix3* m)		
{
	Matrix3* m1 = new Matrix3();
	for (int i = 0; i<=2; i++)
	{
		for (int j = 0; j<=2; j++)
		{
			int temp = 0;
			for(int k = 0; k<=2; k++)
				temp += this->matrix[i][k]*m->matrix[k][j];
			m1->matrix[i][j] = temp;
		}
	}
	return m1;
}		

Vector3* Matrix3::Multiplication(Vector3* v)		
{
	Vector3* v1 = new Vector3(0.0,0.0,0.0);
	
	int i,j;
	double sum;
	for( i = 0 ; i<=2 ; i++)
	{sum = 0;
		for(j = 0; j<=2 ; j++)
			sum = sum + v->vector[j]*this->matrix[j][i];
		v1->vector[i] = sum;
	}		
	return v1;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3* transform::Translate(Vector3* v , double l, double m)
{	
	Matrix3* transformation = new Matrix3();
	transformation->matrix[0][0] = 1;
	transformation->matrix[1][1] = 1;
	transformation->matrix[2][2] = 1;
	
	transformation->matrix[2][0] = l;
	transformation->matrix[2][1] = m;
	
	Vector3* v1 = transformation->Multiplication(v);
	return v1;
}
	
////////////////////////////////////////////////////////////////////////////////////////////////

Vector3* transform::Rotate(Vector3* v, double theta)
{
	theta = (PI/180)*theta;
	
	Matrix3* transformation = new Matrix3();

		transformation->matrix[0][0] = cos(theta);
		transformation->matrix[0][1] = sin(theta);
		transformation->matrix[1][0] = -1*sin(theta);
		transformation->matrix[1][1] = cos(theta);
		transformation->matrix[2][2] = 1;
	
	return transformation->Multiplication(v);
}
///////////////////////////////////////////////////////////////////////////////////////////
Vector3* transform::Rotate_center(Vector3* v, Vector3* c,double theta)
{
	v = this->Translate(v,-1*c->vector[0],-1*c->vector[1]);
	v = this->Rotate(v,theta);
	v = this->Translate(v,c->vector[0],c->vector[1]);
	return v;
}
////////////////////////////////////////////////////////////////////////////////////////////


