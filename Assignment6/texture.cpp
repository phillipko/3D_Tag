//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

/*	
	File:   texture.cpp 
	PROGRAMMER:  Dan Cliburn  (dcliburn@pacific.edu)
	PURPOSE:	Defines the methods for the texture class.
*/

#include "texture.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
using namespace std;

Texture::Texture(string filename, bool displayTexData, TexType t, AlphaValue a)
{
	imageData = 0;
	imageDataAlpha = 0;
	type = t;

	if (!(loadTexBMP(filename, displayTexData, t, a)))
		isLOADED = false;
}

void Texture::readHeaderData(ifstream &in, int &offset, int &bitsPerPixel, bool displayTexData)
{
	char bitData[4];

	in.seekg(10);  //move to the beginning of the header data we want to read
	in.read(bitData, 4);  //read in offset of pixel array
	offset = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;
	if (displayTexData) cout << " offset for pixel data " << offset << endl;
	  
	in.read(bitData, 4);  //read in number of bytes in header from here
	int remaining = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;  
	if (displayTexData) cout << " number of bytes remaining in header " << remaining << endl;
	
	in.read(bitData, 4);  //read in width
	imageWidth = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256 + (unsigned char)bitData[2] * 65536;
	if (displayTexData) cout << " width " << imageWidth << endl;
	
	in.read(bitData, 4);  //read in height
	imageHeight = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256 + (unsigned char)bitData[2] * 65536; 
	if (displayTexData) cout << " height " << imageHeight << endl;
	
	in.read(bitData, 2);  //read in number of color planes
	int colorPlanes = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;
	if (displayTexData) cout << " color planes " << colorPlanes << endl;

	in.read(bitData, 2);  //read in bits per pixel
	bitsPerPixel = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;  
	if (displayTexData) cout << " bitsPerPixel " << bitsPerPixel << endl;

	//There is more header data but we aren't bothering to read it.  We have what we need:
	// offset to the pixel data, bitsPerPixel, image width, and image height
}

int Texture::loadTexBMP(string filename, bool displayTexData, TexType t, AlphaValue a)
{
	unsigned char temp;
	int offset;

	type = t;
	alpha = a;
	ifstream in(filename.c_str(), ios::binary);

	if (!in)
	{
		cout << "Could not load texture -> " << filename << endl;
		return 0;
	}
	
	if (displayTexData) cout << "Data for " << filename << endl;
	readHeaderData(in, offset, bitsPerPixel, displayTexData);

	//Make sure image is of a supported type
	if (bitsPerPixel != 24 || imageHeight < 32 || imageWidth < 32 || imageWidth %4 != 0)
	{																
		in.close();
		cout << filename << " error. Bitmaps must be 24 bits per pixel,\n"
				"at least 32 pixels for row and height,\n"
				"and width must be a multiple of 4." << endl;
		return 0;
	}
	isLOADED = true;
	
	long size = imageWidth * imageHeight;
	imageData = new char[size*3]; //3 bytes per pixel
	in.seekg(offset, ios::beg);  //move past the header data in the file
	in.read(imageData, size*3);
	in.close();

	//Reds and blues are transposed, swap them
	for (int i = 0; i < size; i++)
	{
		temp = imageData[i*3];
		imageData[i*3] = imageData[i*3 + 2];
		imageData[i*3 + 2] = temp;
	}

	if (t == addAlpha)  //Add an alpha value if requested - used with transparency
	{
		imageDataAlpha = new char[size*(3 + 1)];

		// Loop through the image data and add an alpha value
		int newIndex = 0;
		for (int i = 0; i < size*3; i +=3)
		{
			// Copy the pixel data into the new array
			imageDataAlpha[newIndex] = imageData[i];
			imageDataAlpha[newIndex+1] = imageData[i+1];
			imageDataAlpha[newIndex+2] = imageData[i+2];
			if (alpha == QUARTER)
				imageDataAlpha[newIndex+3] = 0x40;  //This is binary 01000000 (64, or 1/4 of 255)
			else if (alpha == THIRD)
				imageDataAlpha[newIndex + 3] = 0x55;  //This is binary 01010101 (85 or 1/3 of 255)
			else if (alpha == HALF)
				imageDataAlpha[newIndex + 3] = 0x80;  //This is binary 10000000 (128, or 1/2 of 255)
			else if (alpha == TWOTHIRDS)
				imageDataAlpha[newIndex + 3] = 0xAA;  //This is binary 10101010 (170, or 2/3 of 255)
			else if (alpha == THREEQUARTERS)
				imageDataAlpha[newIndex + 3] = 0xC0;  //This is binary 11000000 (192, or 3/4 of 255)
			else if (alpha == ONE)
				imageDataAlpha[newIndex + 3] = 0xFF;  //This is binary 11111111 (255)
			else if (imageData[i] == 0 && imageData[i + 1] == 0 && imageData[i + 2] == 0)  //alpha is FINDBLACK
			{	//The color of this pixel is black so make alpha be zero
				imageDataAlpha[newIndex + 3] = 0x00;  //This allows some portions of the image to be 
			}
			else //alpha is FINDBLACK, but the pixel is not the color black.  Make alpha one
			{
				imageDataAlpha[newIndex + 3] = 0xFF;  //This is binary 11111111 (255)
			}

			newIndex += 4;
		}
	}

	return 1;  //read the file successfully
}

Texture::~Texture()
{
	// delete image data array(s)
	if (imageData != 0) delete [] imageData;
	if (imageDataAlpha != 0) delete [] imageDataAlpha;
}