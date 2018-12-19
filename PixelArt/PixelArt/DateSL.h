#pragma once

#include "stdafx.h"
#define BUFFER(x,y) *(pBuffer + y * GridSize + x)

#ifdef UNICODE
#ifndef TSTRING
#define TSTRING std::wstring
#endif
#else
#ifndef TSTRING
#define TSTRING std::string
#endif
#endif

BOOL WriteBmp(const TSTRING &strFile, const std::vector<BYTE> &vtData, const SIZE &sizeImg);
BOOL WriteBmp(const TSTRING &strFile, HDC hdc);
BOOL WriteBmp(const TSTRING &strFile, HDC hdc, const RECT &rcDC);

void SaveBMPFile(HWND hwnd, RECT rect);