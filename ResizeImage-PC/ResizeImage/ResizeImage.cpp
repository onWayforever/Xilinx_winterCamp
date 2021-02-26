#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

void ResizeFile(char *filename, char *outputFilename, char *resizeMod);
void CountImageWidthAndHeight();
int CountRowZeros(int width);

using namespace std;

int repeats;
int height = 0;
int width = 0;
int newHeight = 0, newWidth = 0;
char c, b, g, r;
ifstream file1;
ofstream file2;

int main(int __argc,char* __argv[])
{
	if (__argc == 4)
	{
		ResizeFile(__argv[1], __argv[2], __argv[3]);
	}
	else
	{
		cout << "Incorrect input arguments";
	}
	return 0;
}

void ResizeFile(char *filename, char *outputFilename, char *resizeMod)
{
	repeats = atoi(resizeMod);
	file1.open(filename, ios::binary);
	file2.open(outputFilename, ios::binary);
	if (file1.is_open() && file2.is_open())
	{
		// count new width and height
		CountImageWidthAndHeight();

		for (int i = 0; i < height; i++)
		{
			int firstImageZeros, secondImageZeros;
			for (int u = 0; u < repeats; u++)
			{
				for (int j = 0; j < width; j++)
				{
					file1.get(b);
					file1.get(g);
					file1.get(r);
					for (int k = 0; k < repeats; k++)
					{
						file2.put(b);
						file2.put(g);
						file2.put(r);
					}
				}
				firstImageZeros = CountRowZeros(width);
				secondImageZeros = CountRowZeros(newWidth);
				for (int t = 0; t < firstImageZeros; t++)
				{
					file1.get(c);
				}
				for (int t = 0; t < secondImageZeros; t++)
				{
					file2.put(c);
				}
				file1.seekg(54 + i*(width * 3 + firstImageZeros));
			}
			file1.seekg(54 + (i + 1)*(width * 3 + firstImageZeros));
		}
	}
	else
	{
		cout << "smth wrong";
	}
	file1.close();
	file2.close();
	CountRowZeros(65);
	CountRowZeros(3);
}

void CountImageWidthAndHeight()
{
	for (int i = 0; i < 54; i++)
	{
		if (i >= 18 && i <= 25)
		{
			//reading
			for (int j = 0; j < 4; j++)
			{
				int value;
				file1.get(c);
				if (c < 0) value = 256 + (int)c;
				else value = c;
				width += value * pow(256, j);
			}
			for (int j = 0; j < 4; j++)
			{
				int value;
				file1.get(c);
				if (c < 0) value = 256 + (int)c;
				else value = c;
				height += value * pow(256, j);
			}
			//writing
			for (int j = 0; j < 4; j++, i++)
			{
				newWidth = width*repeats;
				int a = newWidth / pow(256, j);
				file2.put(a);
			}
			for (int j = 0; j < 4; j++, i++)
			{
				newHeight = height*repeats;
				int a = newHeight / pow(256, j);
				file2.put(a);
			}
			i--;
		}
		else
		{
			file1.get(c);
			file2.put(c);
		}
	}
}

int CountRowZeros(int width)
{
	int value = 0;
	int pixelWidth = width * 3;
	while (pixelWidth % 4 != 0)
	{
		value++;
		pixelWidth++;
	}
	return value;
}