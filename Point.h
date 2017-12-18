#ifndef POINT_H
#define POINT_H

class point{
	public:
		point(): x(0),y(0),z(0) {};	
		point(double in_x, double in_y): x(in_x), y(in_y), z(0) {};
		point(double in_x, double in_y, double in_z): x(in_x), y(in_y), z(in_z) {};
		
		const double * xref() {return &x;}
		const double * yref() {return &y;}
		const double * zref() {return &z;}
		
		double getX() {return x;}
		double getY() {return y;}
		double getZ() {return z;}
		
		void setX(double in_x) {x = in_x;}
		void setY(double in_y) {y = in_y;}
		void setZ(double in_z) {z = in_z;}		
	
		// not working in Visual Studio ???
		//friend ostream& operator <<(ostream & output, point thePoint) {output << "(" << thePoint.x << ", " << thePoint.y << ", " << thePoint.z << ")";}
		
	private:		
		double x;
		double y;
		double z;
};

#endif
