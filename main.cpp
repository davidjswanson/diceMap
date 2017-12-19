#include <iostream>	//C++ Includes
#include <list>
#include <math.h>

#include <Point.h>	//Project Specific Includes
#include <rect.cpp>

//#include <glad/glad.h>


 //NanoGUI Includes

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>

using namespace std;


using nanogui::Screen;
using nanogui::Window;
using nanogui::GroupLayout;
using nanogui::Button;
using nanogui::Vector2f;
using nanogui::MatrixXu;
using nanogui::MatrixXf;
using nanogui::Label;

using std::list;
//using std::list<int>::iterator;

// Check if a point is with a certain radius of the origin.
bool ptInRadius(point* thePt, double rad) {
	if (sqrt(pow(thePt->getX(), 2) + pow(thePt->getY(), 2)) < rad)
		return true;
	else
		return false;
}

// Function to find all the rectangles in the circle
list<rect> findRectangles(double radius, double x_pitch, double y_pitch){
	point* startPoint = new point(((int)(radius / x_pitch) + 1.0) * -x_pitch,
		((int)(radius / y_pitch) + 1.0) * y_pitch);	// Furthest Upper Left Point to search
	list<rect> rectList;
	//	iterator currentRect;

	double current_x = 0;
	double current_y = 0;

	//Loop through all points, assume each point is the top left vertex of a rectangle
	//and check if the rectangle is within the desired radius.
	for (int i = 0; i < ((radius * 2 / x_pitch) + 1); i++) {
		current_x = (startPoint->getX() + (x_pitch*i));
		for (int j = 0; j < ((radius * 2 / y_pitch) + 1); j++) {
			current_y = (startPoint->getY() - (y_pitch*j));

			// If all four points of a rectangle are in 
			if (ptInRadius(&point(current_x, current_y), radius) &&
				ptInRadius(&point(current_x, current_y - y_pitch), radius) &&
				ptInRadius(&point(current_x + x_pitch, current_y - y_pitch), radius) &&
				ptInRadius(&point(current_x + x_pitch, current_y), radius))
			{
				rectList.push_back(rect(			// Add it to the list
					point(current_x, current_y),
					point(current_x, current_y - y_pitch),
					point(current_x + x_pitch, current_y - y_pitch),
					point(current_x + x_pitch, current_y)));
			}
		}
	}
	return rectList;
}

class drawWindow : public nanogui::Screen {
public:
	drawWindow() : nanogui::Screen(Eigen::Vector2i(500, 500), "Wafer Map") {
		using namespace nanogui;
		Window* window = new Window(this);
		window->setTitle("Window");
	}

private:


};

int main() {
	double radius = 50;		// Radius around area of interest
	double x_pitch = 30;	// Rectangle spacing in x direction	
	double y_pitch = 11;	// Rectangle spacing in y direction

	std::list<rect>::iterator rects;

	list<rect> rectList = findRectangles(radius, x_pitch, y_pitch);
	
	cout << rectList.size() << " Rectangles\n";
	point* ptList;
	for (rects = rectList.begin(); rects != rectList.end(); rects++)
	{
		ptList = rects->getPts();
		cout << "(" << ptList[0].getX() << ", " << ptList[0].getY() << "), ";
		cout << "(" << ptList[1].getX() << ", " << ptList[1].getY() << "), ";
		cout << "(" << ptList[2].getX() << ", " << ptList[2].getY() << "), ";
		cout << "(" << ptList[3].getX() << ", " << ptList[3].getY() << ")\n";

	}
	
	nanogui::init();
	
	nanogui::Screen* screen = new drawWindow();
	screen->performLayout();
	screen->drawAll();
	screen->setVisible(true);
	
	nanogui::mainloop();
	nanogui::shutdown();
	
	int junk;
	cin >> junk;
	return 0;
}