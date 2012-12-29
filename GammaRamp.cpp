// GammaRamp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>

#include "GammaRamp.h"

CGammaRamp::CGammaRamp() :
	hGDI32(NULL),
	hScreenDC(NULL),
	pGetDeviceGammaRamp(NULL),
	pSetDeviceGammaRamp(NULL)
{
}

CGammaRamp::~CGammaRamp()
{
	FreeLibrary();
}

BOOL CGammaRamp::LoadLibrary()
{
	BOOL bReturn = FALSE;

	FreeLibrary();

	hGDI32 = ::LoadLibrary(_T("gdi32.dll"));
	if (hGDI32 != NULL)
	{
		pGetDeviceGammaRamp = (LPFNSETDEVICEGAMMARAMP)GetProcAddress(hGDI32, "GetDeviceGammaRamp");
		
		pSetDeviceGammaRamp = 
			(LPFNSETDEVICEGAMMARAMP)GetProcAddress(hGDI32, "SetDeviceGammaRamp");
		
		if (pGetDeviceGammaRamp == NULL || pSetDeviceGammaRamp == NULL)
			FreeLibrary();
		else
			bReturn = TRUE;
	}

	return bReturn;
}

void CGammaRamp::FreeLibrary()
{
	if (hGDI32 != NULL) 
	{
		::FreeLibrary(hGDI32);
		hGDI32 = NULL;
	}
}

BOOL CGammaRamp::LoadLibraryIfNeeded()
{
	BOOL bReturn = FALSE;

	if (hGDI32 == NULL)
		LoadLibrary();

	if (pGetDeviceGammaRamp != NULL && pSetDeviceGammaRamp != NULL)
		bReturn = TRUE;

	return bReturn;
}

BOOL CGammaRamp::SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
	if (LoadLibraryIfNeeded())
		return pSetDeviceGammaRamp(hDC, lpRamp);
	else
		return FALSE;
}

BOOL CGammaRamp::GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
	if (LoadLibraryIfNeeded())
		return pGetDeviceGammaRamp(hDC, lpRamp);
	else
		return FALSE;

}

BOOL CGammaRamp::SetBrightness(WORD wBrightness, HDC hDC)
{
	BOOL bReturn = FALSE;
	HDC hGammaDC = hDC;

	if (hDC == NULL)
		hGammaDC = GetDC(NULL);

	if (hGammaDC != NULL)
	{
		WORD GammaArray[3][256];

		for (int iIndex = 0; iIndex < 256; iIndex++)
		{
			int iArrayValue = iIndex * (wBrightness + 128);

			if (iArrayValue > 65535)
				iArrayValue = 65535;

			GammaArray[0][iIndex] = 
			GammaArray[1][iIndex] = 
			GammaArray[2][iIndex] = (WORD)iArrayValue;
			
		}

		bReturn = SetDeviceGammaRamp(hGammaDC, GammaArray);
	}

	if (hDC == NULL)
		ReleaseDC(NULL, hGammaDC);

	return bReturn;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CGammaRamp GammaRamp;
	WORD wBrightness = 128;
	BOOL bDone = FALSE;

	while (!bDone)
	{
		switch (::_getch())
		{
			case 72:
				GammaRamp.SetBrightness(++wBrightness);
				break;
			case 80:
				GammaRamp.SetBrightness(--wBrightness);
				break;
			case 'q':
			case 'Q':
				bDone = TRUE;
				break;
			default:
				break;
		}

		::_tprintf(_T("\r                          \r%ld"), wBrightness);
	}

	return 0;
}
