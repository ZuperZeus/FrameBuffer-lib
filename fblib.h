#ifndef FBLIB_H
#define FBLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
class screen
{
	private:
		FILE *fbfile;
		uint16_t screenarr[YS][XS];
		uint16_t color;
		int screen_x, screen_y;
	public:
		screen(int screen_x, int screen_y, char *fbpath);
		~screen();
		void set_color(uint16_t new_color);
		void draw_rectangle(int x, int y, int rect_x, int rect_y);
		void draw_circle(int cx, int cy, int r);
		void draw_ellipse(int cx, int cy, int rx, int ry);
		void clear();
		void commit();
};
uint16_t mapg(int og);
uint16_t maprb(int og);
uint16_t nomaprgb(int r,int g,int b);
uint16_t rgb(int r,int g,int b);
#endif
