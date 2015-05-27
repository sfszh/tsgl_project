/*
 * testForestFire.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

const int IPF = 1000;  //For those functions that need it

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Pseudo-simulates a forest fire using a lot of probability and randomness.
 * \details
 * - Store the Canvas's dimensions for ease of use.
 * - Set the fire's life, strength, and maximum spread distance to some predetermined numbers.
 * - Seed the random number generator.
 * - Allocate arrays for storing each pixel's onFire status and flammability.
 * - For each pixel:
 *   - Get its distance from the center of the Canvas.
 *   - Set its flammability and color based upon a semi-arbitrary single-line function.
 *   .
 * - Draw 32 random square "lakes" with very low flammabilities on the Canvas
 * - Declare a mini-firePoint struct with coordinates, life, and strength.
 * - Make a 3x3 square of fire in the middle of the Canvas, and color the pixels accordingly
 * - A timer is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep until the Canvas is ready to draw.
 *   - For each fire point:
 *     - Pop it from the queue, pushing it back on only if its life > 0.
 *     - For each cell adjacent to the fire, if it is not on the edge of the screen, not already
 *     on fire, and the random number generator rolls a number nower than the cell's flammability,
 *     set that cell on fire.
 *     .
 *   .
 * - Deallocate the onFire and flammability arrays.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void forestFireFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    const float LIFE = 10,
                STRENGTH = 0.03,
                MAXDIST = sqrt(WINDOW_W * WINDOW_W + WINDOW_H * WINDOW_H) / 2;
    srand(time(NULL));  // Seed the random number generator
    bool* onFire = new bool[WINDOW_W * WINDOW_H]();
    float* flammability = new float[WINDOW_W * WINDOW_H]();
    for (int i = 0; i < WINDOW_W; i++) {  // For each individual point
        for (int j = 0; j < WINDOW_H; j++) {
            float xi = std::abs(WINDOW_W / 2 - i);
            float yi = std::abs(WINDOW_H / 2 - j);
            float tdist = (MAXDIST - sqrt(xi * xi + yi * yi)) / MAXDIST;
            float f = 0.01 + (i * j % 100) / 100.0 * randfloat(100) / 2 * tdist;
            flammability[i * WINDOW_H + j] = f;
            can.drawPoint(i, j, ColorFloat(0.0f, f, 0.0f, 1.0f));
        }
    }
    for (int reps = 0; reps < 32; reps++) {
        int x = rand() % WINDOW_W;
        int y = rand() % WINDOW_H;
        int w = rand() % (WINDOW_W - x);
        int h = rand() % (WINDOW_H - y);
        if (w > 32) w = 32;
        if (h > 32) h = 32;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                flammability[(x + i) * WINDOW_H + (y + j)] = 0.01;
                can.drawPoint(x + i, y + j, ColorFloat(0.0f, 0.0f, 1.0f, 0.25f));
            }
        }
    }
    struct firePoint {
        int x;
        int y;
        float life;
        float strength;
    };
    std::queue<firePoint> fires;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            firePoint fire = { WINDOW_W / 2 - 1 + i, WINDOW_H / 2 - 1 + j, LIFE, STRENGTH };
            fires.push(fire);
            can.drawPoint(WINDOW_W / 2 - 1 + i, WINDOW_H / 2 - 1 + j, ColorFloat(1.0f, 0.0f, 0.0f, STRENGTH));
        }
    }
    Timer t(FRAME);
    while (can.getIsOpen()) {
        t.sleep();
        int l = fires.size();
        for (int i = 0; i < l; i++) {
            firePoint f = fires.front();
            fires.pop();
            if (--f.life > 0) fires.push(f);
            int myCell = f.x * WINDOW_H + f.y;
            if (f.x > 0 && !onFire[myCell - WINDOW_H] && randfloat() < flammability[myCell - WINDOW_H]) {
                firePoint fire = { f.x - 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - WINDOW_H] = true;
                can.drawPoint(f.x - 1, f.y, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.x < WINDOW_W - 1 && !onFire[myCell + WINDOW_H]
                && randfloat() < flammability[myCell + WINDOW_H]) {
                firePoint fire = { f.x + 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + WINDOW_H] = true;
                can.drawPoint(f.x + 1, f.y, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y > 0 && !onFire[myCell - 1] && randfloat() < flammability[myCell - 1]) {
                firePoint fire = { f.x, f.y - 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - 1] = true;
                can.drawPoint(f.x, f.y - 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y < WINDOW_H && !onFire[myCell + 1] && randfloat() < flammability[myCell + 1]) {
                firePoint fire = { f.x, f.y + 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + 1] = true;
                can.drawPoint(f.x, f.y + 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
        }
    }
    delete onFire;
    delete flammability;
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c20(0, 0, WINDOW_W, WINDOW_H, BUFFER*2, "");
    c20.setBackgroundColor(GREY);
    c20.start();
    forestFireFunction(c20);
    c20.close();
    glfwTerminate();  // Release GLFW
}