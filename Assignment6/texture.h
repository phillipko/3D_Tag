/*	
	File:   texture.h
	PROGRAMMER:  Dan Cliburn  (dcliburn@pacific.edu)
	PURPOSE:  The following is the description of the Texture class.  To use, simply pass the filename
	of a 24-bit per pixel Bitmap image to the constructor or call the loadTexBMP method. Bitmaps 
	should be at least 32 pixels in width and height and the width must be a multiple of four.
	If your image does not meet these requirements, you can easily modify it using MS Paint.  Just 
	follow these steps:
	 1) Open the image in MS Paint
	 2) Select the "Resize" option from the "Home" menu.  Click on the "Pixels" option and with 
		"Maintain aspect ratio" selected, specify the closest multiple of 4 to your image
		for the "Horizontal" value.  Click "OK".
	 3) Now, choose to "Save as" then "BMP picture".  In the box that pops up, select "24-bit Bitmap"
	    from the "Save as type:" drop down list.  Click "Save".

	Acknowledgment #1: The idea for this class is based on the c functions written 
	by Nate Miller on 5/5/1999 for handling texture mapping using TGA images.
  	(see http://www.essi.fr/~buffa/cours/synthese_image/DOCS/Tutoriaux/Nate/index-6.htm)

	Acknowledgment #2: Later, this class was modified to support transparent tectures,
	i.e. portions of an image are invisible.  This is often used when billboarding.
	Passing the parameters "addAlpha" and FINDBLACK to the load method (or constructor) adds an alpha 
	bit of 0 to the pixels where the image is black.  Other colors will be given an alpha of 255.  
	With appropriate blending parmeters set, the black portions of the image can be made invisible.
	The idea for this comes from the text "OpenGL Game Programming", by Kevin Hawkins 
	and Dave Astle, pages 434-438.

	Acknowledgement #3:  In 2010 a bug was resolved that kept some 24-bit Bitmaps from loading correctly.
	At this time the class was also modified to support printing of some of the header information to the 
	screen. Header info is printed if true is passed as the 4th parameter to loadTexBMP or the 
	constructor.  Thank you to those who provided suggestions (such as Michael Casolary) that helped 
	to resolve this issue.

	NOTE:  This class was modified on 10/22/13 to be made compatible with the text, OpenGL 4.0 Shading Language
	Cookbook, by David Wolff.  Later, in September of 2014 the fourth parameter "AlphaValue" was modified 
	to support several predefined alpha values that could be added to Bitmap images.  These values are:
	QUARTER, THIRD, HALF, TWOTHIRDS, THREEQUARTERS, and ONE.  The class was also modified to be made 
	compatible with C++ strings.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <fstream>
#include <string>
using namespace std;

enum TexType {addAlpha, rgb};
enum AlphaValue { QUARTER, THIRD, HALF, TWOTHIRDS, THREEQUARTERS, ONE, FINDBLACK };

class Texture
{
	public:

		Texture() {isLOADED = false;}
		Texture(string filename, bool displayTexData = false, TexType t = rgb, AlphaValue a = ONE);

		int loadTexBMP(string filename, bool displayTexData = false, TexType t = rgb, AlphaValue a = ONE);

		int isLoaded() {return isLOADED;}
	
		int width() {return imageWidth;}
		int height() {return imageHeight;}
		char *bits() { if (type == rgb) return imageData; return imageDataAlpha;}

		~Texture();

	private:
		
		bool isLOADED;

		//Image parameter information
		int imageWidth;
		int imageHeight;
		int bitsPerPixel;

		char *imageData;
		char *imageDataAlpha;
		
		int id;  //Texture id number
		TexType type;
		AlphaValue alpha;

		void readHeaderData(ifstream &in, int &offset, int &bitsPerPixel, bool displayTexData);
};


#endif