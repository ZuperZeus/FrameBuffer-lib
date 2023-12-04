#include "fblib.h"
screen::screen(int x, int y, char *fbpath)
{
	screen_x=x;
	screen_y=y;
	fbfile=fopen(fbpath,"w");
	clear();
	commit();
	set_color(rgb(0,0,0));
}
screen::~screen()
{
	fclose(fbfile);
}
void screen::set_color(uint16_t new_color)
{
	color=new_color;
}
void screen::draw_rectangle(int x, int y, int rect_x, int rect_y)
{
	if(rect_x+x>screen_x)
	{
		printf("X is too big\n");
		exit(1);
	}
	if(rect_y+y>screen_y)
	{
		printf("X is too big\n");
		exit(1);
	}
	for(int i=y;i<rect_y+y;i++)
		for(int j=x;j<rect_x+x;j++)
			screenarr[i][j]=color;
}
void screen::draw_circle(int cx, int cy, int r)
{
	for(int i=0;i<screen_y;i++)
		for(int j=0;j<screen_x;j++)
			if((j-x)*(j-x)+(i-y)*(i-y)<r*r)
				screenarr[i][j]=color;

}
void screen::draw_ellipse(int cx, int cy, int rx, int ry)
{
	for(int i=0;i<screen_y;i++)
		for(int j=0;j<screen_x;j++)
			if((double)((j-x)*(j-x))/(double)(rx*rx)+(double)((i-y)*(i-y))/(double)(ry*ry)<1)
				screenarr[i][j]=color;
}
void screen::clear()
{
	memset(screenarr,0,screen_y*screen_x*2);
}
void screen::commit()
{
	fseek(fbfile,0,SEEK_SET);
	fwrite(&screenarr,screen_y*screen_x*2,1,fbfile);
}
uint16_t mapg(int og)
{
	//og is a number from 0b0000_0000 - 0b1111_1111
	//we want to map it to 0b0000_0000 - 0b0011_1111
	//so, we must round the last 2 digits.
	//There are 4 cases for the last 2 digits:
	//0b00, 0b01, 0b10, 0b11
	//since we only want to round 0b10 and 0b11, we can just add 2.
	//Then, we left shift by 2, and we'll get properly rounded values
	if(og>0b11111101) return 0b111111;
	return (uint16_t)((og+2) >> 2);
}
uint16_t maprb(int og)
{
	//og is a number from 0b0000_0000 - 0b1111_1111
	//we want to map it to 0b0000_0000 - 0b0001_1111
	//so, we must round the last 3 digits.
	//There are 8 cases for the last 3 digits:
	//0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111
	//since we only want to round 0b1xx, we can just add 4.
	//Then, we left shift by 3, and we'll get properly rounded values
	if(og>0b11111011) return 0b11111;
	return (uint16_t)((og+4) >> 3);
}
uint16_t nomaprgb(int r,int g,int b)
{
	//the way to show color in 2 bytes is like so:
	//0brrrrr_gggggg_bbbbb
	//so, red and blue are 5 bytes, and g is 6 bytes.
	//therefore, g must be left shifted 5 bytes,
	//and r (5+6) = 11 bytes
	return (r << 11) | (g << 5) | b;
}
uint16_t rgb(int r,int g,int b)
{
	return nomaprgb(maprb(r),mapg(g),maprb(b));
}
