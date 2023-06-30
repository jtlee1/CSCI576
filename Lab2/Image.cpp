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





//modify from tutorial https://www.youtube.com/watch?v=hW4gZ4tGwds&ab_channel=QuantitativeBytes, https://github.com/QuantitativeBytes/qbColor/blob/main/qbColor.cpp 
void RGBtoHSV(double *arr, double* arr2) {
	double m_red, m_green, m_blue, Rd, Gd, Bd, Cmax, Cmin, H, S, V;
	m_blue = arr2[0];
	m_green = arr2[1];
	m_red = arr2[2];

	//printf("This is R : %f\n", m_red);
	//printf("G : %f\n", m_green);
	//printf("B : %f\n", m_blue);
	double minValue, maxValue;
	unsigned char maxIndex;
	if ((m_red == m_green) && (m_red == m_blue))
	{
		maxIndex = 0;
		maxValue = m_red;
		minValue = m_red;
	}
	else if ((m_red >= m_green) && (m_red >= m_blue))
	{
		maxIndex = 1;
		maxValue = m_red;
		minValue = (m_green < m_blue) ? m_green : m_blue;
	}
	else if ((m_green >= m_red) && (m_green >= m_blue))
	{
		maxIndex = 2;
		maxValue = m_green;
		minValue = (m_red < m_blue) ? m_red : m_blue;
	}
	else
	{
		maxIndex = 3;
		maxValue = m_blue;
		minValue = (m_red < m_green) ? m_red : m_green;
	}

	// Now we can compute the value of h.
	double h;
	switch (maxIndex)
	{
	case 0:	h = 0.0;
		break;
	case 1: h = 60.0 * ((m_green - m_blue) / (maxValue - minValue));
		break;
	case 2: h = 60.0 * (2 + ((m_blue - m_red) / (maxValue - minValue)));
		break;
	case 3: h = 60.0 * (4 + ((m_red - m_green) / (maxValue - minValue)));
		break;
	}
	if (h < 0.0)
		h += 360.0;

	// Compute the saturation.
	double s;
	s = (maxIndex == 0) ? 0.0 : ((maxValue - minValue) / maxValue);

	// Compute the value.
	double v = maxValue;

	// And store the result.
	arr[0] = h;
	arr[1] = s;
	arr[2] = v;
	//h > 40 && h < 70 && s>40 && v > 40
	if (h > 88 && h < 175 && v>120) {
		arr[0] = -1;
		arr[1] = -1;
		arr[2] = -1;
		//printf("This is H : %f\n", h);
		//printf("s : %f\n", s);
		//printf("v : %f\n", v);
	}
	//printf("This is H : %f\n", h);
	//printf("s : %f\n", s);
	//printf("v : %f\n", v);
	

}

//modify from tutorial https://www.youtube.com/watch?v=hW4gZ4tGwds&ab_channel=QuantitativeBytes, https://github.com/QuantitativeBytes/qbColor/blob/main/qbColor.cpp 
void HSVtoRGB(char* arr, double* arr2, char* bg) {
	double m_red, m_green, m_blue, m_value, m_saturation, m_hue,R,G,B;
	m_value = arr2[2];
	m_saturation = arr2[1];
	m_hue = arr2[0];
	if (m_hue == -1) {
		arr[0] = bg[0];
		arr[1] = bg[1];
		arr[2] = bg[2];
		//arr[0] = 255;
		//arr[1] = 0;
		//arr[2] = 0;
	}
	else {

		double rgbRange = m_value * m_saturation;
		double maxRGB = m_value;
		double minRGB = m_value - rgbRange;
		double hPrime = m_hue / 60.0;
		double x1 = fmod(hPrime, 1.0);
		double x2 = 1.0 - fmod(hPrime, 1.0);

		//printf("This is minRGB : %f\n", minRGB);
		//printf("maxRGB : %f\n", maxRGB);
		//printf("hPrime : %f\n", hPrime);
		//printf("m_hue : %f\n", m_hue);

		if ((hPrime >= 0) && (hPrime < 1))
		{
			m_red = maxRGB;
			m_green = (x1 * rgbRange) + minRGB;
			m_blue = minRGB;
		}
		else if ((hPrime >= 1) && (hPrime < 2))
		{
			m_red = (x2 * rgbRange) + minRGB;
			m_green = maxRGB;
			m_blue = minRGB;
		}
		else if ((hPrime >= 2) && (hPrime < 3))
		{
			m_red = minRGB;
			m_green = maxRGB;
			m_blue = (x1 * rgbRange) + minRGB;
		}
		else if ((hPrime >= 3) && (hPrime < 4))
		{
			m_red = minRGB;
			m_green = (x2 * rgbRange) + minRGB;
			m_blue = maxRGB;
		}
		else if ((hPrime >= 4) && (hPrime < 5))
		{
			m_red = (x1 * rgbRange) + minRGB;
			m_green = minRGB;
			m_blue = maxRGB;
		}
		else if ((hPrime >= 5) && (hPrime < 6))
		{
			m_red = maxRGB;
			m_green = minRGB;
			m_blue = (x2 * rgbRange) + minRGB;
		}
		else
		{
			m_red = 0.0;
			m_green = 0.0;
			m_blue = 0.0;
		}
		R = m_red;
		G = m_green;
		B = m_blue;
		if (R > 255) {
			R = 255;
		}
		if (G > 255) {
			G = 255;
		}
		if (B > 255) {
			B = 255;
		}
		if (R < 0) {
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
	//printf("This is R : %f\n", R);
	//printf("G : %f\n", G);
	//printf("B : %f\n", B);
}





double getAvg(double* arr, int x) {
	if (x == 0 || x == 1 || x == 2) {
		return (arr[x] + arr[x + 3] + arr[x + 640 * 3] + arr[x + 641 * 3]) / 4;
	}
	else if (x == 640 * 480 * 3 - 1 || x == 640 * 480 * 3 - 2 || x == 640 * 480 * 3 - 3) {
		return (arr[x] + arr[x - 3] + arr[x - 640 * 3] + arr[x - 641 * 3]) / 4;
	}
	else if (x == 639 * 3 || x == 639 * 3 + 1 || x == 639 * 3 + 2) {
		return(arr[x] + arr[x - 3] + arr[x + 640 * 3] + arr[x + 639 * 3]) / 4;
	}
	else if (x == 640 * 479 * 3 || x == 640 * 479 * 3 + 1 || x == 640 * 479 * 3 + 2) {
		return(arr[x] + arr[x + 3] + arr[x - 640 * 3] + arr[x - 639 * 3]) / 4;
	}
	//top
	else if (x - 640 * 3 < 0) {
		return(arr[x] + arr[x - 3] + arr[x + 3] + arr[x + 640 * 3] + arr[x + 640 * 3] + arr[x + 641 * 3]) / 6;
	}
	//bot
	else if (x + 640 * 3 > 640 * 480 * 3) {
		return(arr[x] + arr[x - 3] + arr[x + 3] + arr[x - 640 * 3] + arr[x - 639 * 3] + arr[x - 641 * 3]) / 6;
	}
	//left
	else if (x % (640 * 3) == 0 || x % (640 * 3) == 1 || x % (640 * 3) == 2) {
		return(arr[x] + arr[x + 3] + arr[x - 640 * 3] + arr[x - 639 * 3] + arr[x + 640 * 3] + arr[x + 641 * 3]) / 6;
	}
	//right
	else if (x % (640 * 3) == 639 * 3 || x % (640 * 3) == 639 * 3 + 1 || x % (640 * 3) == 639 * 3 + 2) {
		return(arr[x] + arr[x - 3] + arr[x - 640 * 3] + arr[x - 641 * 3] + arr[x + 640 * 3] + arr[x + 639 * 3]) / 6;
	}
	else {
		int res = (arr[x] + arr[x - 3] + arr[x + 3]) / 3;
		
		return res;
	}




}

bool isDifferent(double* arr, double* arr2) {
	bool flag = true;
	double a, b, c, d, e, f, g, h, i, j, k, l;
	double thresh = 200;
	a = arr[0] - arr2[0];
	a = a * a;
	b = arr[1] - arr2[1];
	b = b * b;
	c = arr[2] - arr2[2];
	c = c * c;

	if (a + b + c <= thresh) {
		flag = false;
	}

	return flag;
}



char* getnum(int a) {
	char ret[64];
	char str[32];
	itoa(a, str, 10);
	if (a < 10) {
		strcpy(ret,"000");
	}
	else if (a < 100) {
		strcpy(ret, "00");
	}
	else if (a < 1000) {
		strcpy(ret, "0");
	}
	strcat(ret, str);
	//printf("This is ret : %s\n", ret);
	return ret;
}





// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(const char* pram1, const char* pram2, int count, const char* del)
{

	// TO DO by student
	MyImage bImage;
	MyImage pImage;
	MyImage dImage;
	MyImage eImage;
	MyImage fImage;

	// sample operation
	char* Y, * U, * V, * A;
	//double W = 0, H = 0;
	char p[_MAX_PATH];
	
//C:\Users\user\Downloads\input\foreground_1\foreground_1.0000.rgb C : \Users\user\Downloads\input\background_static_1\background_static_1.0000.rgb 1
	const char *pa1 = pram1;
	const char* pa2 = pram2;
	char path0[_MAX_PATH];
	char path1[_MAX_PATH];
	char path2[_MAX_PATH];
	char path3[_MAX_PATH];
	char path4[_MAX_PATH];
	char path5[_MAX_PATH];
	char test[1000];
	strcpy_s(p, pram1);

	char* delim = "\\";
	char* next;
	char* ptr;
	ptr = strtok_s(p, delim, &next);

	while (ptr != NULL)
	{
		A = ptr;
		ptr = strtok_s(NULL, delim, &next);
	}

	strcpy(path1, pa1);
	strcat(path1, "\\");
	strcat(path1, A);
	strcat(path1, ".");
	strcat(path1, getnum(count));
	strcat(path1, ".rgb");

	if (count > 3) {
		strcpy(path0, pa1);
		strcat(path0, "\\");
		strcat(path0, A);
		strcat(path0, ".");
		strcat(path0, getnum(count-1));
		strcat(path0, ".rgb");
		pImage.setWidth(Width);
		pImage.setHeight(Height);
		pImage.setImagePath(path0);
		pImage.ReadImage();

		strcpy(path3, pa1);
		strcat(path3, "\\");
		strcat(path3, A);
		strcat(path3, ".");
		strcat(path3, getnum(count - 2));
		strcat(path3, ".rgb");
		dImage.setWidth(Width);
		dImage.setHeight(Height);
		dImage.setImagePath(path3);
		dImage.ReadImage();

		strcpy(path4, pa1);
		strcat(path4, "\\");
		strcat(path4, A);
		strcat(path4, ".");
		strcat(path4, getnum(count - 3));
		strcat(path4, ".rgb");
		eImage.setWidth(Width);
		eImage.setHeight(Height);
		eImage.setImagePath(path4);
		eImage.ReadImage();

		strcpy(path5, pa1);
		strcat(path5, "\\");
		strcat(path5, A);
		strcat(path5, ".");
		strcat(path5, getnum(count - 4));
		strcat(path5, ".rgb");
		fImage.setWidth(Width);
		fImage.setHeight(Height);
		fImage.setImagePath(path5);
		fImage.ReadImage();
	}

	strcpy_s(p, pram2);

	ptr = strtok_s(p, delim, &next);

	while (ptr != NULL)
	{
		V = ptr;
		ptr = strtok_s(NULL, delim, &next);
	}


	
	strcpy(path2, pa2);
	strcat(path2, "\\");
	strcat(path2, V);
	strcat(path2, ".");
	strcat(path2, getnum(count));
	strcat(path2, ".rgb");
	setImagePath(path1);
	//setImagePath(test);
	ReadImage();
	bImage.setWidth(Width);
	bImage.setHeight(Height);
	bImage.setImagePath(path2);
	//bImage.setImagePath(test);
	bImage.ReadImage();
	setBk(&bImage);
	//printf("This is A : %s\n", path1);
	
	if (*del == '1') {
		double* Data3;
		Data3 = new double[Width * Height * 3];
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = (unsigned char)Data[i];
			
		}
		double* temparr;
		temparr = new double[Width * Height * 3];
		for (int i = 0; i < Width * Height * 3; i++)
		{
			temparr[i] = (unsigned char)Data[i];
		}
		for (int i = 0; i < Width * Height; i++)
		{
			RGBtoHSV(&temparr[3 * i], &Data3[3 * i]);
		}


		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data[i] = temparr[i];
		}

		for (int i = 0; i < Width * Height; i++)
		{
			HSVtoRGB(&Data[3 * i], &temparr[3 * i], &bImage.Data[3 * i]);
		}

		delete temparr;
		delete Data3;

	}
	if (*del == '0') {

		double* Data3;
		Data3 = new double[Width * Height * 3];
		double* Data2;
		Data2 = new double[Width * Height * 3];
		double* Data1;
		Data1 = new double[Width * Height * 3];
		double* Data4;
		Data4 = new double[Width * Height * 3];
		double* Data5;
		Data5 = new double[Width * Height * 3];
		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data3[i] = (unsigned char)Data[i];
		}
		

		for (int i = 0; i < Width * Height * 3; i++)
		{
			Data[i] = bImage.Data[i];
		}
		if (count > 3) {
			double* temparr;
			temparr = new double[Width * Height * 3];
			double* temparr2;
			temparr2 = new double[Width * Height * 3];
			/*for (int i = 0; i < Width * Height; i++)
			{
				RGBtoHSV(&temparr[3 * i], &Data3[3 * i]);
			}*/
			for (int i = 0; i < Width * Height * 3; i++)
			{
				Data2[i] = (unsigned char)pImage.Data[i];
			}
			for (int i = 0; i < Width * Height * 3; i++)
			{
				Data1[i] = (unsigned char)dImage.Data[i];
			}
			for (int i = 0; i < Width * Height * 3; i++)
			{
				Data4[i] = (unsigned char)eImage.Data[i];
			}
			for (int i = 0; i < Width * Height * 3; i++)
			{
				Data5[i] = (unsigned char)fImage.Data[i];
			}
			/*for (int i = 0; i < Width * Height; i++)
			{
				RGBtoHSV(&temparr2[3 * i], &Data2[3 * i]);
			}*/
			for (int i = 0; i < Width * Height *3-3; i=i+3)
			{
				//if (isDifferent(&temparr[i], &temparr2[i])) {
				
				//if (isDifferent(&Data3[i], &Data2[i]) && isDifferent(&Data3[i], &Data4[i]) && isDifferent(&Data3[i], &Data5[i])&& isDifferent(&Data4[i], &Data2[i]) && isDifferent(&Data2[i], &Data5[i]) && isDifferent(&Data4[i], &Data5[i])) {
				double d1[3], d2[3], d3[3], d4[3], d5[3];
				
				d1[0] = getAvg(Data3, i);
				d1[1] = getAvg(Data3, i+1);
				d1[2] = getAvg(Data3, i+2);
				d2[0] = getAvg(Data2, i);
				d2[1] = getAvg(Data2, i+1);
				d2[2] = getAvg(Data2, i+2);
				d3[0] = getAvg(Data1, i);
				d3[1] = getAvg(Data1, i + 1);
				d3[2] = getAvg(Data1, i + 2);
				d4[0] = getAvg(Data4, i);
				d4[1] = getAvg(Data4, i + 1);
				d4[2] = getAvg(Data4, i + 2);
				d5[0] = getAvg(Data5, i);
				d5[1] = getAvg(Data5, i + 1);
				d5[2] = getAvg(Data5, i + 2);
				


				//if (isDifferent(d1, d2)&& isDifferent(d1, d3)&&isDifferent(d1, d4)&& isDifferent(d3, d2)&& isDifferent(d4, d2)&& isDifferent(d3, d4)){
				if (isDifferent(d1, d2)|| isDifferent(d1, d3) || isDifferent(d1, d4) || isDifferent(d3, d2) || isDifferent(d4, d2) || isDifferent(d3, d4)|| isDifferent(d1, d5) || isDifferent(d2, d5) || isDifferent(d3, d5) || isDifferent(d4, d5)) {
					Data[i] = Data3[i];
					Data[i+1] = Data3[i+1];
					Data[i+2] = Data3[i+2];
					//Data5[i] = -1;
				}
			}
			
			
			delete temparr;
			delete temparr2;
		}
		
		delete Data3;
		delete Data2;
		delete Data1;
		delete Data5;
		delete Data4;
		
	}

	
	

	return true;
}