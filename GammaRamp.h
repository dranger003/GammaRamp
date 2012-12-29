#pragma once

class CGammaRamp
{
public:
	CGammaRamp();
	virtual ~CGammaRamp();

	BOOL LoadLibrary();
	void FreeLibrary();
	BOOL LoadLibraryIfNeeded();
	BOOL SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
	BOOL GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
	BOOL SetBrightness(WORD wBrightness, HDC hDC = NULL);

protected:
	HMODULE hGDI32;
	HDC hScreenDC;
	typedef BOOL (WINAPI* LPFNSETDEVICEGAMMARAMP)(HDC hDC, LPVOID lpRamp);

	LPFNSETDEVICEGAMMARAMP pGetDeviceGammaRamp;
	LPFNSETDEVICEGAMMARAMP pSetDeviceGammaRamp;
};
