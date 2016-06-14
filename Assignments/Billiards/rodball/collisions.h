#include <stdio.h>
#include <math.h>
#include "Transformations.h"


class wall
{
	public:
		Vector3* v1;
		Vector3* v2;
		char name;
		wall(double x1, double y1, double x2, double y2,char name)
		{
			Vector3* v1 = new Vector3(x1,y1,1);
			Vector3* v2 = new Vector3(x2,y2,1);
			this->v1 = v1;
			this->v2 = v2;
			this->name = name;
		}	
};

class rod
{
	public:
		double length;
		double speed_x;
		double speed_y;
		double angular_speed;
		
		Vector3* v1;
		Vector3* v2;	
		
 		rod(double x1, double y1, double x2, double y2, double speed_x, double speed_y, double angular_speed)
		{
			Vector3* v1 = new Vector3(x1,y1,1);
			Vector3* v2 = new Vector3(x2,y2,1);
			this->v1 = v1;
			this->v2 = v2;
			this->length = sqrt(pow((this->v1->vector[0]-this->v2->vector[0]),2) + pow((this->v1->vector[1]-this->v2->vector[1]),2)); 
			this->speed_x = speed_x;
			this->speed_y = speed_y;
			this->angular_speed = angular_speed;
		}
};

class disk
{
	public:
		double radius;
		double c_x;
		double c_y;
		double v_x;
		double v_y;
		disk(double r, double c_x, double c_y, double v_x, double v_y)
		{
			this->c_x = c_x;
			this->c_y = c_y;
			this->radius = r;
			this->v_x = v_x;
			this->v_y = v_y;
		}
};


class boundary
{
	public:
		wall* right;
		wall* left;
		wall* below;
		wall* upper;
		boundary()
		{
			this->left = new wall(0,0,0,500,'l');
			this->below = new wall(0,0,1000,0,'b');
			this->right = new wall(1000,0,1000,500,'r');
			this->upper = new wall(0,500,1000,500,'u');
		}
};


void collision(disk* d, wall* w);
void collision(disk* d1, disk* d2);

int collision(rod* r, wall* w);
int collision(rod* r1, rod* r2);


void collision(disk* d,boundary* b);