/*
 * testAlphaRectangle.cpp
 *
 * Usage: ./testAlphaRectangle <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;
#include "Flock2d.h"

Flock2d flock;

void alphaRectangleFunction(Canvas& can) {
   /*
	const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;
    while (can.isOpen()) {
        can.sleep();
        a = rand() % WW; b = rand() % WH;
        c = rand() % WW; d = rand() % WH;
        can.drawRectangle(a, b, c, d, ColorInt(rand()%MAX_COLOR, rand()%MAX_COLOR, rand()%MAX_COLOR, 16));
    }
	*/
	while (can.isOpen())
	{
		can.sleep();
		can.clear();
		flock.update();
		std::cout << flock.size() << std::endl;
		for (int i =0; i < flock.size(); i++) {
			Boid2d *b = flock.get(i);
			//can.drawRectangle(b->x, b->y, 0.01, 0.01,  ColorInt(MAX_COLOR, MAX_COLOR, MAX_COLOR, 16));
			can.drawCircle(b->x, b->y, 10, 32, ColorInt(rand()%MAX_COLOR, rand()%MAX_COLOR, rand()%MAX_COLOR, 16), true);
			
		}
		can.resumeDrawing();
	}
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Fancy Rectangles");
    c.setBackgroundColor(BLACK);
	flock.setup(50,w/2,h/2,100);
	flock.setBounds(0,0,w,h);
	flock.setBoundmode(1);
    c.run(alphaRectangleFunction);
}
