#include "Histogram.h"


Cv::Histogram::Histogram() 
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//La inicializaci�n provoca un error en el heap al cargar la image...
	/*std::fill(this->redFrequency, this->redFrequency + BINS, 0);
	std::fill(this->greenFrequency, this->greenFrequency + BINS, 0);
	std::fill(this->blueFrequency, this->blueFrequency + BINS, 0);
	std::fill(this->luminanceFrequency, this->luminanceFrequency + BINS, 0);
	std::fill(this->frequencySize, this->frequencySize + 4, 0);
	std::fill(this->min, this->min + 4, 0);
	std::fill(this->max, this->max + 4, 0);
	this->maxLuminance = this->minLuminance = 0;
	std::fill(&roundCdf[0][0], &roundCdf[4][BINS], 0);
	std::fill(&cdfValues[0][0], &cdfValues[4][BINS], 0);
	std::fill(minCdf, minCdf + 4, 0);*/
}

Cv::Histogram::~Histogram() 
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	delete this->originalImage;
	delete this->equalizedImage;
	delete this->rect;
	delete this->histogramCanvas;
}

bool Cv::Histogram::SetImage(WCHAR * fileUri) 
{
	this->originalImage = Gdiplus::Bitmap::FromFile(fileUri);

	if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) 
	{
		return false;
	}

	this->GetImageDimensions();

	int luminanceHolder = 0;

	for (int y = 0; y < this->imageHeight; y++)
	{
		for (int x = 0; x < this->imageWidth; x++)
		{
			this->originalImage->GetPixel(x, y, &this->color);

			//Al asignar a int hace el corte, como byte no jala
			luminanceHolder = (this->color.GetRed() * 0.30F) + (this->color.GetBlue() * 0.59F) + (this->color.GetGreen() * 0.11F);

			this->red.push_back(this->color.GetRed());
			this->green.push_back(this->color.GetGreen());
			this->blue.push_back(this->color.GetBlue());
			this->luminance.push_back(luminanceHolder);

			//obtener m�ximos y m�nimos.
			if (color.GetRed() > max[0]) max[0] = color.GetRed();
			if (color.GetGreen() > max[1]) max[1] = color.GetGreen();
			if (color.GetBlue() > max[2]) max[2] = color.GetBlue();
			if (luminanceHolder > max[3]) max[3] = luminanceHolder;

			if (color.GetRed() < min[0]) min[0] = color.GetRed();
			if (color.GetGreen() < min[1]) min[1] = color.GetGreen();
			if (color.GetBlue() < min[2]) min[2] = color.GetBlue();
			if (luminanceHolder < min[3]) min[3] = luminanceHolder;
		}
	}

	frequencySize[0] = max[0] - min[0] + 1;
	frequencySize[1] = max[1] - min[1] + 1;
	frequencySize[2] = max[2] - min[2] + 1;
	frequencySize[3] = max[3] - min[3] + 1;

	return true;
}

void Cv::Histogram::FillBins() 
{
	std::fill(this->redFrequency, this->redFrequency + BINS, 0);
	std::fill(this->greenFrequency, this->greenFrequency + BINS, 0);
	std::fill(this->blueFrequency, this->blueFrequency + BINS, 0);
	std::fill(this->luminanceFrequency, this->luminanceFrequency + BINS, 0);

	//iterar sobre los data set, obtener frecuencia
	for (int i = 0; i < this->red.size(); i++) 
	{
		this->redFrequency[this->red[i]]++;
	}
	for (int j = 0; j < this->green.size(); j++) 
	{
		this->greenFrequency[this->green[j]]++;
	}
	for (int k = 0; k < this->blue.size(); k++) 
	{
		this->blueFrequency[blue[k]]++;
	}
	for (int l = 0; l < this->luminance.size(); l++) 
	{
		this->luminanceFrequency[this->luminance[l]]++;
	}
}

void Cv::Histogram::CDF() {
	//TODO: Debo resetearlo?
	this->cdfValues[0][0] = this->redFrequency[0];
	this->cdfValues[1][0] = this->greenFrequency[0];
	this->cdfValues[2][0] = this->blueFrequency[0];
	this->cdfValues[3][0] = this->luminanceFrequency[0];

	for (int i = 1; i < BINS; i++)
	{
		this->cdfValues[0][i] = this->cdfValues[0][i - 1] + this->redFrequency[i];
		this->cdfValues[1][i] = this->cdfValues[1][i - 1] + this->greenFrequency[i];
		this->cdfValues[2][i] = this->cdfValues[2][i - 1] + this->blueFrequency[i];
		this->cdfValues[3][i] = this->cdfValues[3][i - 1] + this->luminanceFrequency[i];

		if (this->cdfValues[0][i] < this->minCdf[0]) this->minCdf[0] = this->cdfValues[0][i];
		if (this->cdfValues[1][i] < this->minCdf[1]) this->minCdf[1] = this->cdfValues[1][i];
		if (this->cdfValues[2][i] < this->minCdf[2]) this->minCdf[2] = this->cdfValues[2][i];
		if (this->cdfValues[3][i] < this->minCdf[3]) this->minCdf[3] = this->cdfValues[3][i];
	}
}

void Cv::Histogram::DrawHistogram() 
{
	Gdiplus::Pen redPen(Gdiplus::Color::Red, 1.0F);
	this->histogramCanvas[0] = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* redDrawingBuffer = Gdiplus::Graphics::FromImage(this->histogramCanvas[0]);

	Gdiplus::Pen greenPen(Gdiplus::Color::Green, 1.0F);
	this->histogramCanvas[1] = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* greenDrawingBuffer = Gdiplus::Graphics::FromImage(this->histogramCanvas[1]);

	Gdiplus::Pen bluePen(Gdiplus::Color::Blue, 1.0F);
	this->histogramCanvas[2] = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* blueDrawingBuffer = Gdiplus::Graphics::FromImage(this->histogramCanvas[2]);

	Gdiplus::Pen luminancePen(Gdiplus::Color::White, 1.0F);
	this->histogramCanvas[3] = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* luminanceDrawingBuffer = Gdiplus::Graphics::FromImage(this->histogramCanvas[3]);

	int h = 0;
	double division = 0.0;
	double f = 0.0, m = 0.0;

	//Obtener el tope de la gr�fica
	int height = 0;
	for (int i = 0; i < 4; i++)
	{
		if (this->max[i] > height)
		{
			height = this->max[i];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		f = this->redFrequency[i]; m = this->max[0];
		division = f / m;
		h = (256 - (division * 256));
		redDrawingBuffer->DrawLine(&redPen, i, 256, i, h);

		f = this->greenFrequency[i]; m = this->max[0];
		division = f / m;
		h = (256 - (division * 256));
		greenDrawingBuffer->DrawLine(&greenPen, i, 256, i, h);
		
		f = this->blueFrequency[i]; m = this->max[0];
		division = f / m;
		h = (256 - (division * 256));
		blueDrawingBuffer->DrawLine(&bluePen, i, 256, i, h);
		
		f = this->luminanceFrequency[i]; m = this->max[0];
		division = f / m;
		h = (256 - (division * 256));
		luminanceDrawingBuffer->DrawLine(&luminancePen, i, 256, i, h);
	}
}

void Cv::Histogram::Equalize(EqualizationType equalizationType, double alpha = 1.0) 
{
	switch (equalizationType)
	{
	case Cv::Simple:
		//255*CDF/CDFmax
		
		break;
	case Cv::Uniform:
		//(pixelMayor - pixelMenor) * CDF/CDFmax + pixelMin
		break;
	case Cv::Exponential:
		//(pixelMin) - (1/alpha) * ln(1 - (CDF / maxCDF))
		break;
	case Cv::General:
		//round( ((CDF-CDFmin)/(totalPixels - CDFmin)) * 255 )
		break;
	case Cv::DynamicRange:
		// (pixel - PixelMenor) * ((CDFmax - CDFMin) / (pixelMayor - pixelMenor)) + CDFMin
		break;
	case Cv::Stretching:
		// ( (pixel - pixelMin) / (pixelMax - pixelMin) ) * 255 
	default:
		break;
	}
}

bool Cv::Histogram::Save(std::wstring filename) 
{
	std::wstring prepend = L"";

	try
	{
		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		//this->equalizedImage->Save((prepend + filename).c_str(), &pngClsid, NULL);
		prepend = L"RedHistogram";
		this->GetRedHistogram()->Save((prepend + filename).c_str(), &pngClsid, NULL);
		prepend = L"GreenHistogram";
		this->GetGreenHistogram()->Save((prepend + filename).c_str(), &pngClsid, NULL);
		prepend = L"BlueHistogram";
		this->GetBlueHistogram()->Save((prepend + filename).c_str(), &pngClsid, NULL);
		prepend = L"LuminanceHistogram";
		this->GetLuminanceHistogram()->Save((prepend + filename).c_str(), &pngClsid, NULL);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

Gdiplus::Bitmap * Cv::Histogram::GetRedHistogram()
{
	return this->histogramCanvas[0];
}

Gdiplus::Bitmap * Cv::Histogram::GetGreenHistogram()
{
	return this->histogramCanvas[1];
}

Gdiplus::Bitmap * Cv::Histogram::GetBlueHistogram()
{
	return this->histogramCanvas[2];
}

Gdiplus::Bitmap * Cv::Histogram::GetLuminanceHistogram()
{
	return this->histogramCanvas[3];
}

Gdiplus::Bitmap * Cv::Histogram::GetEqualizedImage()
{
	return this->equalizedImage;
}

void Cv::Histogram::GetImageDimensions()
{
	this->imageHeight = this->originalImage->GetHeight();
	this->imageWidth = this->originalImage->GetWidth();
	this->totalPixels = this->imageHeight * this->imageWidth;
	this->rect = new Gdiplus::Rect(0, 0, this->imageWidth, this->imageHeight);
}

int Cv::Histogram::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array in bytes 

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);

	if (size == 0) {
		return -1; // Failure
	}

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL) {
		return -1; // Failure 
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j; // Success 
		}
	}
	free(pImageCodecInfo);
	return 0; // Failure 
}