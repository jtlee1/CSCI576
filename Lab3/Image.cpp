//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}



void widthcomp(double* arr, int w, double* arr2,int off) {
	int h = 512;
	for (int i = 0; i < h; i = i++) {
		for (int j = 0; j < w; j = j++)
		{
			if (j % 2 == 0) {
				double temp = (arr2[(i * 3 * h + j * 3) + off] + arr2[i * 3 * h + j * 3 + 3 + off]) / 2;
				arr[i * 3 * h + j * 3/2 + off] = temp;
				
				
			}
			else {
				double temp = (arr2[i * 3 * h + j * 3 - 3 + off] - arr2[i * 3 * h + j * 3 + off])/2;
				arr[i * 3 * h + (j * 3 -3) / 2 + w*3 / 2 + off] = temp;
				
				
			}
		}
	}
}

void heightcomp(double* arr, int h, double* arr2, int off) {
	int w = 512;
	for (int i = 0; i < h/2; i = i++) {
		for (int j = 0; j < w; j = j++)
		{
			
			double temp = (arr2[i * 3 * 2 * w + j * 3 + off] + arr2[(i * 2 + 1) * 3 * w + j * 3 + off])/2;

			arr[i * 3 * w + j * 3 + off] = temp;
			
			arr[i * 3 * w + j * 3 + h * w * 3 / 2 + off] = arr2[i * 3 * 2 * w + j * 3 + off] - temp;
			

			
		}
	}
}

void heightde(double* arr, int h, double* arr2, int off) {
	int w = 512;
	for (int i = 0; i < h ; i = i++) {
		for (int j = 0; j < w; j = j++)
		{

			if (i % 2 == 0) {
				arr[i * 3 * w + j * 3 + off] = arr2[i * 3 * w / 2 + j * 3 + off] + arr2[(i) * 3 * w / 2 + j * 3 + h * w * 3 / 2 + off];
				
			}
			else {
				
				arr[i * 3 * w + j * 3 + off] = arr2[(i-1) * 3 * w / 2 + j * 3 + off] - arr2[(i - 1) * 3 * w / 2 + j * 3 + h * w * 3 / 2 + off];
				
			}

		}
	}
}


void widthde(double* arr, int w, double* arr2, int off) {
	int h = 512;
	for (int i = 0; i < h; i = i++) {
		for (int j = 0; j < w; j = j++)
		{
			if (j % 2 == 0) {
				arr[i * 3 * h + j * 3 + off] = arr2[i * 3 * h + (j * 3)/2 + off] + arr2[i * 3 * h + (j * 3 ) / 2 + w * 3 / 2 + off];
				
				
			}
			else {
				arr[i * 3 * h + j * 3 + off] = arr2[i * 3 * h + (j * 3 -3) / 2 + off] - arr2[i * 3 * h + (j * 3 - 3) / 2 + w * 3 / 2 + off];
				
				
			}
		}
	}
}
// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(const char* pram, int c)
{

	// TO DO by student

	// sample operation
	int Y = 0, U = 0, V = 0, A = 0;
	double W = 0, H = 0;
	char p[_MAX_PATH];
	strcpy_s(p, pram);
	
	char* delim = " ";
	char* next;
	char* ptr;
	ptr = strtok_s(p, delim, &next);



	if (ptr != NULL)
	{
		Y = atoi(ptr);
	}

	if (Y == -1) {
		//printf("'%d'\n", Y);
		Y = c;
		//printf("'%d'\n", Y);
	}
	

	double* Data3;
	Data3 = new double[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data3[i] = (unsigned char)Data[i];
	}
	double* Data4;
	Data4 = new double[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data4[i] = (unsigned char)Data[i];
	}

	
	
	int count = 512;
	for (int i = 0; i < 9-Y; i++) {

		widthcomp(Data4, count, Data3, 0);
		widthcomp(Data4, count, Data3, 1);
		widthcomp(Data4, count, Data3, 2);
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = Data4[i];
		}
		heightcomp(Data4, count, Data3, 0);
		heightcomp(Data4, count, Data3, 1);
		heightcomp(Data4, count, Data3, 2);
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = Data4[i];
		}
		count = count / 2;
	}
	
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			if (i >= count || j>=count) {
				Data4[(i * 512 + j) * 3] = 0;
				Data4[(i * 512 + j) * 3+1] = 0;
				Data4[(i * 512 + j) * 3+2] = 0;
			}
		}
	}
	
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data3[i] = Data4[i];
	}

	for (int i = 0; i < 9 - Y; i++) {
		count = count * 2;
		heightde(Data4, count, Data3, 0);
		heightde(Data4, count, Data3, 1);
		heightde(Data4, count, Data3, 2);
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = Data4[i];
		}
		widthde(Data4, count, Data3, 0);
		widthde(Data4, count, Data3, 1);
		widthde(Data4, count, Data3, 2);
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = Data4[i];
		}
		

	}



	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data[i] = Data4[i];
	}

	delete Data3;
	delete Data4;

	return true;
}