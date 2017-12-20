#include <iostream>	//C++ Includes
#include <list>
#include <math.h>

#include <Point.h>	//Project Specific Includes
#include <rect.cpp>

#include <nanogui/screen.h>	//NanoGUI Includes
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>

using namespace std;
/*
using nanogui::Screen;
using nanogui::Window;
using nanogui::GroupLayout;
using nanogui::Button;
using nanogui::Vector2f;
using nanogui::MatrixXu;
using nanogui::MatrixXf;
using nanogui::Label;
*/
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

//mapWindow is used for drawing wafer map, always has circular wafer drawn in background
// has rectangular die drawn on top.
class mapWindow : public nanogui::Screen {
public:
	mapWindow() : nanogui::Screen(Eigen::Vector2i(500, 500), "Wafer Map"), myRadius(100) {
		using namespace nanogui;
		setBackground(Color((int)255, (int)255, (int)255, (int)0));	// Window Background
	}

	void changeRadius(double inRad) { myRadius = inRad; }

	void setRects(list<rect> newRects) { myRect = newRects; }

	void drawContents() {
		// Get size of drawing area in pixels
		glfwGetWindowSize(glfwWindow(), &screenWindowHeight, &screenWindowWidth);
		
		// windowHeight compensates for pixel scaling on high dpi screens, as
		// nanogui seems to work with the assumption of 96dpi regardless
		// of actual screen resolution.
		windowHeight = screenWindowHeight / pixelRatio();
		windowWidth = screenWindowWidth / pixelRatio();
		
		// Find a scaling factor so that the circle will occupy 90% of available area
		if (windowHeight < windowWidth)
			scaleFactor = (windowHeight*0.9) / (2*myRadius);
		else
			scaleFactor = (windowWidth*0.9) / (2*myRadius);
		
		// Draw the base circle
		NVGcontext* vg = nvgContext();
		nvgTranslate(vg, windowHeight/2, windowWidth/2);
		nvgBeginPath(vg);
		nvgEllipse(vg, 0, 0, myRadius*scaleFactor, myRadius*scaleFactor);
		nvgFillColor(vg, nvgRGBA(128, 128, 128, 255));
		nvgFill(vg);
		for (rects = myRect.begin(); rects != myRect.end(); rects++)
		{
			ptList = rects->getPts();
			nvgBeginPath(vg);
			nvgRect(vg, (ptList[0].getX()*scaleFactor)+2, 
						(ptList[0].getY()*scaleFactor)-2, 
						((ptList[2].getX()-ptList[0].getX())*scaleFactor)-1,
						((ptList[2].getY()-ptList[0].getY())*scaleFactor)+1);
			nvgFillColor(vg, nvgRGBA(210, 0, 0, 180));
			nvgFill(vg);
		}

	}
private:
	GLFWvidmode return_struct;
	double scaleFactor;
	double myRadius;
	int screenWindowHeight, screenWindowWidth;
	double windowHeight, windowWidth;
	std::list<rect>::iterator rects;
	list<rect> myRect;
	point* ptList;
};

int main() {
	double radius = 100;	// Radius around area of interest
	double x_pitch = 25;	// Rectangle spacing in x direction	
	double y_pitch = 4;	// Rectangle spacing in y direction

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
	
	mapWindow* viewScreen = new mapWindow();
	viewScreen->performLayout();
	viewScreen->drawAll();
	viewScreen->setVisible(true);
	viewScreen->changeRadius(radius);
	viewScreen->setRects(rectList);
	nanogui::mainloop();
	nanogui::shutdown();
	
	return 0;
}