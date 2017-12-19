#include <Point.h>

class rect {
	public:
		rect(point p1, point p2, point p3, point p4) : vertex{p1, p2, p3, p4} {};
		point* getPts() { return vertex; }
	private:
		point vertex[4];

	
};