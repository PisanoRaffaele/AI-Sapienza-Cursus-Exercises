#include <cmath>
#include "image.h"

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {

  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)

  // TODO: Your code here

	  //NOT_IMPLEMENTED();

	/*
	int t = 0;
	if (x >= 34.4 && x <= 34.5 && y == 0.0 && c == 0)
	t = 1;


	int a, b;
	int x_int, y_int;
	x_int = x;
	y_int = y;
	x -= x_int;
	y -= y_int;
	if (x > 0.5)
		a = x_int + 1;
	else
		a = x_int;
	if (y > 0.5)
		b = y_int + 1;
	else
		b = y_int;

	if (t)
	{
		printf("[x_int: %d, y_int: %d, x_d: %f, y_d: %f, a: %d, b: %d", x_int, y_int, x, y, a, b);
		printf(" sx: %f,cn: %f, dx: %f]\n\n",clamped_pixel(a-1, b, c), clamped_pixel(a, b, c), clamped_pixel(a+1, b, c));
	}*/

	  int x1 = (int) round(x);
	  int y1 = (int) round(y);
	  return clamped_pixel(x1, y1, c);
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)

  // TODO: Your code here

  //NOT_IMPLEMENTED();
	return 0;
}

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
{
	Image ret(w,h,im.c);
	float x_ratio = (float)im.w / w;
	float y_ratio = (float)im.h / h;
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < im.c; k++)
			{
				ret(i, j, k) = im.pixel_nearest(i*x_ratio, j*y_ratio, k);
			}
		}
	}
	return ret;
}


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {

  // TODO: Your code here

  //NOT_IMPLEMENTED();


  return Image();
  }


