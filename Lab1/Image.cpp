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

void RGBtoYUV(int *arr, int* arr2) {
	int R, G, B, Y, U, V;
	B = arr2[0];
	G = arr2[1];
	R = arr2[2];
	Y = R * 0.299 + G * 0.587 + B * 0.114;
	U = R * 0.596 - G * 0.274 - B * 0.322;
	V = R * 0.211 - G * 0.523 + B * 0.312;
	arr[0] = Y;
	arr[1] = U;
	arr[2] = V;
	

}

void YUVtoRGB(char* arr, int* arr2) {
	int R, G, B, Y, U, V;
	Y = arr2[0];
	U = arr2[1];
	V = arr2[2];
	R = Y * 1.000 + U * 0.956 + V * 0.62;
	
	G = Y * 1.000 - U * 0.272 - V * 0.647;
	B = Y * 1.000 - U * 1.106 + V * 1.703;
	
	if (R > 255) {
		R = 255;
	}
	if (G > 255) {
		G = 255;
	}
	if (B > 255) {
		B = 255;
	}
	if (R <	0 ) {
		R = 0;
	}
	if (G < 0) {
		G = 0;
	}
	if (B < 0) {
		B = 0;
	}
	arr[0] = B;
	arr[1] = G;
	arr[2] = R;

}

char getAvg(int* arr, int x) {
	if (x == 0 || x == 1 || x == 2) {
		return (arr[x] + arr[x+3] + arr[x+1920*3] + arr[x+1921*3]) / 4;
	}
	else if (x == 1920 * 1080 *3 -1 || x == 1920 * 1080 * 3 - 2 || x == 1920 * 1080 * 3 - 3) {
		return (arr[x] + arr[x - 3] + arr[x - 1920*3] + arr[x - 1921*3]) / 4;
	}
	else if (x == 1919*3 || x == 1919 * 3+1 || x == 1919 * 3 +2) {
		return(arr[x] + arr[x - 3] + arr[x + 1920*3] + arr[x + 1919*3]) / 4;
	}
	else if (x == 1920 * 1079 *3 || x == 1920 * 1079 * 3+1 || x == 1920 * 1079 * 3 +2) {
		return(arr[x] + arr[x + 3] + arr[x - 1920*3] + arr[x - 1919*3]) / 4;
	}
	//top
	else if (x - 1920*3 < 0) {
		return(arr[x] + arr[x - 3] + arr[x + 3] + arr[x + 1920*3] + arr[x + 1919*3] + arr[x + 1921*3]) / 6;
	}
	//bot
	else if (x + 1920*3 > 1920 * 1080*3) {
		return(arr[x] + arr[x - 3] + arr[x + 3] + arr[x - 1920*3] + arr[x - 1919*3] + arr[x - 1921*3]) / 6;
	}
	//left
	else if (x % (1920*3) == 0|| x % (1920 * 3) == 1|| x % (1920 * 3) == 2) {
		return(arr[x] + arr[x + 3] + arr[x - 1920*3] + arr[x - 1919*3] + arr[x + 1920*3] + arr[x + 1921*3]) / 6;
	}
	//right
	else if (x % (1920*3) == 1919*3|| x % (1920 * 3) == 1919 * 3+1|| x % (1920 * 3) == 1919 * 3+2) {
		return(arr[x] + arr[x - 3] + arr[x - 1920*3] + arr[x - 1921*3] + arr[x + 1920*3] + arr[x + 1919*3]) / 6;
	}
	else {
		int res = (arr[x]+ arr[x - 3] + arr[x + 3])/3;
		//char res = (arr[x] + arr[x - 3] + arr[x + 3] + arr[x - 1920*3] + arr[x - 1919*3] + arr[x - 1921*3] + arr[x + 1920*3] + arr[x + 1919*3] + arr[x + 1921*3]) / 9;
		
		//printf("'TES%u'\n", (unsigned char)res);
		return res;
	}
	



}

void resample(int* arr, int c, char det ,int w, int h, int* arr2) {
	
	int offset = 0;
	if (det == 'Y') {
		offset = 0;
	}
	if (det == 'U') {
		offset = 1;
	}
	if (det == 'V') {
		offset = 2;
	}
	for (int i = 0; i < h; i = i++) {
		for (int j = 0; j < w; j = j + c)
		{
			if (j + c < w) {
				double s = arr2[i * 3 * w + j * 3 + offset];
				double e = arr2[i * 3 * w + j * 3 + c * 3 + offset];
				double avg = (s + e) / 2;
				for (int k = 1; k < c; k++) {
					arr[i * 3 * w + j * 3 + k * 3 + offset] = avg;
				}
			}
		
			
		}
	}
	
	
}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(const char* pram)
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
	ptr = strtok_s(NULL, delim, &next);
	if (ptr != NULL)
	{
		U = atoi(ptr);
	}
	ptr = strtok_s(NULL, delim, &next);
	if (ptr != NULL)
	{
		V = atoi(ptr);
	}
	ptr = strtok_s(NULL, delim, &next);
	if (ptr != NULL)
	{
		W = atof(ptr);
	}
	ptr = strtok_s(NULL, delim, &next);
	if (ptr != NULL)
	{
		//printf("'%s'\n", ptr);
		H = atof(ptr);
	}
	ptr = strtok_s(NULL, delim, &next);
	if (ptr != NULL)
	{
		A = atoi(ptr);
	}
	ptr = strtok_s(NULL, delim, &next);


	

	int* Data3;
	Data3 = new int[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data3[i] = (unsigned char)Data[i];
	}
	int* temparr;
	temparr = new int[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		temparr[i] = (unsigned char)Data[i];
	}
	for (int i = 0; i < Width * Height; i++)
	{
		RGBtoYUV(&temparr[3 * i], &Data3[3 * i]);
	}

	int* Data4;
	Data4 = new int[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data4[i] = temparr[i];
	}

	resample(temparr, Y, 'Y', Width, Height,Data4);
	resample(temparr, U, 'U', Width, Height,Data4);
	resample(temparr, V, 'V', Width, Height,Data4);


	for (int i = 0; i < Width * Height; i++)
	{
		YUVtoRGB(&Data[3 * i], &temparr[3 * i]);
	}
	
	
	double wp = W;
	double hp = H;

	int wc = round(1920 * wp);
	int hc = round(1080 * hp);
	int off = 0;
	int tem = 0;
	

	int* Data2;
	Data2 = new int[1920 * 1080 * 3];
	for (int i = 0; i < Width * Height * 3; i++)
	{
		Data2[i] = (unsigned char)Data[i];
	}
	

	if (A == 1) {
		for (int j = 0; j < hc; j++) {
			int t2 = round(j / hp);
			for (int i = 0; i < wc; i++)
			{
				
				Data[i * 3 + j * wc * 3] = getAvg(Data2, (round(i / wp) + round(j / hp) * 1920) * 3);
				Data[i * 3 + j * wc * 3 + 1] = getAvg(Data2, round((i / wp) + round(j / hp) * 1920) * 3 + 1);
				Data[i * 3 + j * wc * 3 + 2] = getAvg(Data2, (round(i / wp) + round(j / hp) * 1920) * 3 + 2);
				
			}
		}
	}
	else if(A==0) {
		for (int j = 0; j < hc; j++) {
			int t2 = round(j / hp);
			for (int i = 0; i < wc; i++)
			{
				int t1 = round(i / wp) * 3;
				Data[i * 3 + j * wc * 3] = Data2[t1 + t2 * Width * 3];
				Data[i * 3 + 1 + j * wc * 3] = Data2[t1 + t2 * Width * 3 + 1];
				Data[i * 3 + 2 + j * wc * 3] = Data2[t1 + t2 * Width * 3 + 2];
				
			}
		}
	}
	setWidth(wc);
	setHeight(hc);
	
	
	
	

	return true;
}