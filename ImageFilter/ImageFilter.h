#pragma once
#include <gdiplus.h>
 
class ImageFilter {
	
public:

	ImageFilter();
	~ImageFilter();

	enum FilterType
	{
		Media, MediaPonderada, Mediana, Gaussiano
	};

	bool SetImage(WCHAR *fileUri);
	bool SetKernel(int** matrix, int cols, int rows);
	bool Filter(FilterType filterType);
	bool Save(std::string filename);
	Bitmap GetFilteredImage();

private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Bitmap* origin;
	Bitmap* result;
	BitmapData* bitmapData;
	BitmapData* buffer;
	Gdiplus::Rect* rect;

	int kernelRows;
	int kernelCols;
	std::vector<int> kernel;
	int kernelDivisor;

	int width;
	int height;

	void GetImageDimensions();

	void Media();
	void MediaPonderada();
	void Mediana();
	void Gaussiano();
};