#include <stdlib.h>
#include <iostream>
#include "MyUtils.h"
namespace MyUtils
{
	float ofRandom(float min, float max)
	{
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	float ABS(float number) {
		if (number < 0) {
			return -number;
		} else {
			return number;
		}
	}


}