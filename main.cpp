/*
File: main.cpp
Description: Contains the definition of main and the entry point of the program
Author: Cody Stuck
*/
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cmath>

#include "Image.h"



using namespace std;

/*
	Argument list
	1: scale x
	2: scale y
	3: trans x
	4: trans y
	5: rotat
	6: shear
*/

// coord: allows for easier managing of coordinates
struct coord
{
	double x = 0;
	double y = 0;
};

int main(int argc, char *argv[])
{
	// argument variables
	double xScale, yScale, xTrans, yTrans, rot, shear;
	coord sample, center;
	
	// parse arguments
	sscanf_s(argv[1], "%lf", &xScale);
	sscanf_s(argv[2], "%lf", &yScale);
	sscanf_s(argv[3], "%lf", &xTrans);
	sscanf_s(argv[4], "%lf", &yTrans);
	sscanf_s(argv[5], "%lf", &rot);
	sscanf_s(argv[6], "%lf", &shear);

	// open test image, and create empty output image
	Image inputImg("test.gif");
	Image outputImg(inputImg.getRows(), inputImg.getCols());

	// find center coordinate
	center.x = inputImg.getCols() / 2;
	center.y = inputImg.getRows() / 2;

	// convert rot from degrees to radians
	rot = (rot * M_PI) / 180;

	// loop over every pixel in the output image
	for (int r = 0; r < outputImg.getRows(); r++)
	{
		for (int c = 0; c < outputImg.getCols(); c++)
		{
			// initialize sample pixel coordinates as the current output coordinate
			sample.x = c;
			sample.y = r;

			// apply transformations

			// translation - note that this shifts the center of the image for further transformations
			sample.x = sample.x - xTrans - center.x;
			sample.y = sample.y - yTrans - center.y;

			// scale
			sample.x = sample.x / xScale;
			sample.y = sample.y / yScale;

			// shear
			sample.x = (sample.x - (shear * sample.y)) / (shear + 1);
			sample.y = sample.y / (shear + 1);

			// rotate
			sample.x = (cos(rot) * sample.x) + (sin(rot) * sample.y);
			sample.y = (cos(rot) * sample.y) - (sin(rot) * sample.x);

			// recenter image
			sample.x += center.x;
			sample.y += center.y;

			// pull pixel from sample coordinate
			if (inputImg.getCols() <= sample.x || sample.x < 0 || inputImg.getRows() <= sample.y || sample.y < 0) // check to see if the sample coord is out of bounds
			{
				outputImg.setPixel(r, c, 0, 0, 0); // if so, set pixel to black
			}
			else
			{
				// TODO: do bilinear interpolation on the color of the output to avoid antialiasing
				outputImg.setPixel(r, c, inputImg.getPixel(sample.y, sample.x)); // if not, sample as normal
			}
		}
	}

	// write outputIMG to output.gif
	outputImg.writeImage("output.gif");
}