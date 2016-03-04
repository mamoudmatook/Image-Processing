#include "IncludeWindows.h"
#include "WinMainParameters.h"
#include "resource.h"
#include "Win32Window.h"
#include <stdexcept>
#include <string>
#include <sstream>

using namespace GUI;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

enum ProcessType
{
	GRAYSCALE,
	FILTER,
	EQUALIZATION
} processType;
std::wstring imageUrl = L"C:\\Users\\David\\Documents\\Visual Studio 2015\\Projects\\PIAD1\\lenna.jpg";
Cv::FilterType::FilterType filterType = Cv::FilterType::FilterType::Mean;
Cv::CorrectionMode correctionMode = Cv::CorrectionMode::Cut;
Cv::EqualizationType equalizationType = Cv::EqualizationType::Simple;
Cv::GrayScaleType grayScaleType = Cv::GrayScaleType::Mean;

// ISO C++ conformant entry point. The project properties explicitly sets this as the entry point in the manner
// documented for the linker's /ENTRY option: http://msdn.microsoft.com/en-us/library/f9t8842e.aspx . As per
// the documentation, the value set as the entry point is "mainCRTStartup", not "main". Every C or C++ program
// must perform any initialization required by the language standards before it begins executing our code. In
// Visual C++, this is done by the *CRTStartup functions, each of which goes on to call the developer's entry
// point function.
int main(int argc, char* argv[]) {
	MSG msg{ };
	msg.message = WM_NULL;
	HINSTANCE hInstance = WinMainParameters::GetHInstance();

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	const int maxLoadString = 100;
	WCHAR appTitle[maxLoadString];	// The title bar text
	LoadStringW(hInstance, IDS_APP_TITLE, appTitle, maxLoadString);
	
	HMENU hMenu = nullptr;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_GUI));

	if (hMenu == nullptr) {
		DWORD lastError = GetLastError();
		::std::stringstream errMsg;
		errMsg << "Failed call to LoadMenu in main: GetLastError returned '" << lastError << "'.";
		throw ::std::logic_error(errMsg.str());
	}

	HACCEL hAccelTable = nullptr;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUI));

	if (hAccelTable == nullptr) {
		DWORD lastError = GetLastError();
		::std::stringstream errMsg;
		errMsg << "Failed call to LoadAccelerators in main: GetLastError returned '" << lastError << "'.";
		throw ::std::logic_error(errMsg.str());
	}

	Win32Window mainWindow(&WndProc, appTitle, hMenu);

	return mainWindow.Join(hAccelTable);
}

//VOID DrawImage(HDC hdc, std::wstring file, Cv::FilterType filterType, Cv::CorrectionMode correctionMode)
VOID DrawImage(HDC hdc, std::wstring file)
{
	Gdiplus::Graphics graphics(hdc);
	Cv::GrayScale gimage = Cv::GrayScale();
	Cv::ImageFilter fimage = Cv::ImageFilter();
	Cv::Histogram *himage = new Cv::Histogram();

	switch (processType)
	{
	case GRAYSCALE:
		gimage.SetImage(file);

		switch (grayScaleType)
		{
		case Cv::GrayScaleType::Luminance:
			gimage.Luminance();
			graphics.DrawImage(gimage.GetProcessedImage(), 10, 10);
			gimage.Save(L"luminance.png");
			break;
		case Cv::GrayScaleType::Luminosity:
			gimage.Luminosity();
			graphics.DrawImage(gimage.GetProcessedImage(), 10, 10);
			gimage.Save(L"luminosity.png");
			break;
		case Cv::GrayScaleType::Mean:
			gimage.Mean();
			graphics.DrawImage(gimage.GetProcessedImage(), 10, 10);
			gimage.Save(L"meangray.png");
			break;
		default:
			break;
		}
		break;
	case FILTER:
		fimage.SetImage(file);
		if (filterType != Cv::FilterType::FilterType::Gaussian)
		{
			fimage.Filter(filterType, correctionMode);
		}
		else 
		{
			fimage.Filter(filterType, correctionMode, 0.8);
		}
		graphics.DrawImage(fimage.GetProcessedImage(), 10, 10);
		fimage.Save(Cv::FilterNames[filterType] + L".png");
		break;
	case EQUALIZATION:
		himage->SetImage(file);
		himage->FillBins();
		himage->CDF();
		himage->DrawHistogram();

		if (equalizationType != Cv::EqualizationType::Exponential) {
			himage->Equalize(equalizationType);
			graphics.DrawImage(himage->GetProcessedImage(), 10, 10);
			himage->Save(Cv::EqualizationNames[equalizationType] + L".png");
		}else {	
			himage->Equalize(Cv::EqualizationType::Exponential, 0.02);
			graphics.DrawImage(himage->GetProcessedImage(), 10, 10);
			himage->Save(L"Exponential.png");
		}
		break;
	default:
		break;
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message) {
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		//case IDM_ABOUT:
		//	//DialogBox(WinMainParameters::GetHInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
		//	break;
		case ID_GRAYSCALE_LUMINOSITY:
			processType = GRAYSCALE;
			grayScaleType = Cv::GrayScaleType::Luminosity;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_GRAYSCALE_LUMINANCE:
			processType = GRAYSCALE;
			grayScaleType = Cv::GrayScaleType::Luminance;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_GRAYSCALE_MEAN:
			processType = GRAYSCALE;
			grayScaleType = Cv::GrayScaleType::Mean;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_DIRECTIONALH:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::DirectionalH;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_DIRECTIONALV:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::DirectionalV;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_EMBOSS:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Emboss;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_GAUSSIAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Gaussian;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_LAPLACIAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Laplacian;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_MEAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Mean;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_MEDIAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Median;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_MINUSLAPLACIAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::MinusLaplacian;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_MOTIONBLUR:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::MotionBlur;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_OUTLINE:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::Outline;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_SOBELCOLS:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::SobelCols;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_SOBELROWS:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::SobelRows;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_FILTER_WEIGHTEDMEAN:
			processType = FILTER;
			filterType = Cv::FilterType::FilterType::WeightedMean;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_EQUALIZATION_EXPONENTIAL:
			processType = EQUALIZATION;
			equalizationType = Cv::EqualizationType::Exponential;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_EQUALIZATION_GENERAL:
			processType = EQUALIZATION;
			equalizationType = Cv::EqualizationType::General;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_EQUALIZATION_SIMPLE:
			processType = EQUALIZATION;
			equalizationType = Cv::EqualizationType::Simple;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_EQUALIZATION_STRETCHING:
			processType = EQUALIZATION;
			equalizationType = Cv::EqualizationType::Stretching;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case ID_EQUALIZATION_UNIFORM:
			processType = EQUALIZATION;
			equalizationType = Cv::EqualizationType::Uniform;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			DrawImage(hdc, imageUrl);
		}
		EndPaint(hWnd, &ps);
		
		break;
	case WM_DESTROY:
		Gdiplus::GdiplusShutdown(gdiplusToken);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}