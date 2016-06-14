
#define PI 3.141

class Vector3
{
	public:
		double vector[3];
		Vector3(double x, double y, double w)
		{
			vector[0] = x;
			vector[1] = y;
			vector[2] = w;
		}
		void Addition(Vector3* v);
		void Substraction(Vector3* v);
		
};

class Matrix3
{
	public:
		double matrix[3][3];
		Matrix3()
		{
			for (int i = 0; i<=2; i++)
			{
				for (int j = 0; j<=2;j++)
				this->matrix[i][j] = 0.0;
			}
		}
		void Addition(Matrix3* m);
		void Substraction(Matrix3* m);
		Matrix3* Multiplication(Matrix3* m);
		Vector3* Multiplication(Vector3* v);
};

class transform
{
	public :
		Vector3* Translate(Vector3* v , double l, double m);
		Vector3* Rotate(Vector3* v, double theta);
		Vector3*	Rotate_center(Vector3* v, Vector3* c,double theta);
};

void print_vector(Vector3* V);
void print_tranformation(Matrix3* M);
