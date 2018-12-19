// PixelArt.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PixelArt.h"

#define MAX_LOADSTRING 100
#define SCREENW 907
#define SCREENH 660
#define SMALLSIZE 280
#define MEDIUMSIZE 420
#define LARGESIZE 560
#define SMALLGRID 20
#define MEDIUMGRID 30
#define LARGEGRID 40
// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
COLORREF FrameColor = RGB(251, 148, 24);
COLORREF BackgroundColor = RGB(207, 216, 220);
COLORREF *pBuffer;
COLORREF Color[20] = {
	RGB(0,0,0),RGB(127,127,127),RGB(136,0,21),RGB(237,28,36),RGB(255,127,39),
	RGB(255,242,0),RGB(34,177,76),RGB(0,162,232),RGB(63,72,204),RGB(163,73,164),
	RGB(255,255,255),RGB(195,195,195),RGB(185,122,87),RGB(255,174,201),RGB(255,204,14),
	RGB(239,228,176),RGB(181,230,29),RGB(153,217,234),RGB(112,146,190),RGB(200,191,231)
};
COLORREF BlockColor;
RECT WindowRect, DrawingRect;
HWND hDlgDRAWINGINT, hDlgCOLOR, hDlgSAVE, hDlgGRID, hDlgDRAWINGSAVE;
int Size, GridSize;
// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MainFrame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	ColorEdit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	SaveOption(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	GridOption(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DrawingSave(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void FillBlock(HWND hwnd, int x, int y, COLORREF rgb);
void PaintAll(HWND hwnd, POINT OriginPt);
void PaintGrid(HWND hwnd, POINT OriginPt);
void SetColor(HWND hwnd, COLORREF color);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PIXELART, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIXELART));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIXELART));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PIXELART);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, SCREENW, SCREENH, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	POINT pt;
    switch (message)
    {
	case WM_CREATE:
		//��ʼ������
		Size = SMALLSIZE;
		GridSize = SMALLGRID;
		//��ʼ��λͼ����
		if (pBuffer != NULL)
			free(pBuffer);
		pBuffer = (COLORREF*)malloc(GridSize*GridSize*sizeof(COLORREF));
		for (int i = 0; i < GridSize; i++)
			for (int j = 0; j < GridSize; j++)
				BUFFER(i,j) = RGB(255, 255, 255);
		//��ʼ�����Ի���
		hDlgDRAWINGINT = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DRAWINGINT), hWnd, MainFrame);
		ShowWindow(hDlgDRAWINGINT, SW_NORMAL);
		hDlgCOLOR = CreateDialog(hInst, MAKEINTRESOURCE(IDD_COLOR), hWnd, ColorEdit);
		ShowWindow(hDlgCOLOR, SW_NORMAL);
		hDlgSAVE = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SAVE), hWnd, SaveOption);
		ShowWindow(hDlgSAVE, SW_NORMAL);
		hDlgGRID = CreateDialog(hInst, MAKEINTRESOURCE(IDD_GRID), hWnd, GridOption);
		ShowWindow(hDlgGRID, SW_NORMAL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDM_SAVE:
				hDlgDRAWINGSAVE= CreateDialog(hInst, MAKEINTRESOURCE(IDD_DRAWINGSAVE), hWnd, DrawingSave);
				ShowWindow(hDlgDRAWINGSAVE, SW_NORMAL);
				SendMessage(hDlgDRAWINGSAVE, WM_CLOSE, NULL, NULL);
				break;
			case IDM_NEW:
				//����λͼ����
				if (pBuffer != NULL)
					free(pBuffer);
				pBuffer = (COLORREF*)malloc(GridSize*GridSize*sizeof(COLORREF));
				for (int i = 0; i < GridSize; i++)
					for (int j = 0; j < GridSize; j++)
						BUFFER(i, j) = RGB(255, 255, 255);
				//�����ػ�
				InvalidateRect(hWnd, NULL, false);
				UpdateWindow(hWnd);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HPEN hPen= CreatePen(PS_SOLID, 1, FrameColor), hOldPen;
			HBRUSH hBrush = CreateSolidBrush(BackgroundColor),hOldBrush;
			hOldPen = (HPEN)SelectObject(hdc, hPen);
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 0, 0, SCREENW, SCREENH);
			SelectObject(hdc,hOldPen);
			SelectObject(hdc, hOldBrush);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK MainFrame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT center,OriginPt,MousePt;

	static bool Drawing;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		GetWindowRect(hDlg, &WindowRect);
		center.x = (WindowRect.right - WindowRect.left) / 2;
		center.y = (WindowRect.bottom - WindowRect.top) / 2;
		OriginPt.x = center.x - Size / 2;
		OriginPt.y = center.y - Size / 2;
		DrawingRect.left = center.x - Size / 2;
		DrawingRect.top = center.y - Size / 2;
		DrawingRect.right = center.x + Size / 2;
		DrawingRect.bottom = center.y + Size / 2;
		return (INT_PTR)TRUE;
	case WM_MOUSEMOVE:
		GetCursorPos(&MousePt);
		ScreenToClient(hDlg, &MousePt);
		if (MousePt.x >= DrawingRect.left&&MousePt.x < DrawingRect.right&&MousePt.y >= DrawingRect.top&&MousePt.y < DrawingRect.bottom)
		{
			SetClassLong(hDlg, GCL_HCURSOR, (long)LoadCursor(NULL, IDC_HAND));
			Drawing = true;
		}
		else
		{
			SetClassLong(hDlg, GCL_HCURSOR, (long)LoadCursor(NULL, IDC_ARROW));
			Drawing = false;
		}
		if (wParam == MK_LBUTTON&&Drawing)
		{
			for (int i = 0; i < GridSize; i++)
				for (int j = 0; j < GridSize; j++)
				{
					if (OriginPt.x + j * 14 <= MousePt.x&&OriginPt.x + (j + 1) * 14 > MousePt.x&&OriginPt.y + i * 14 <= MousePt.y&&OriginPt.y + (i + 1) * 14 > MousePt.y)
					{
						MousePt.x = OriginPt.x + j * 14;
						MousePt.y = OriginPt.y + i * 14;
						BUFFER(j, i) = BlockColor;
						break;
					}

				}
			FillBlock(hDlg, MousePt.x, MousePt.y, BlockColor);
			PaintGrid(hDlg, OriginPt);
		}
		return (INT_PTR)TRUE;
	case WM_LBUTTONDOWN:
		if (Drawing)
		{
			for (int i = 0; i < GridSize; i++)
				for (int j = 0; j < GridSize; j++)
				{
					if (OriginPt.x + j * 14 <= MousePt.x&&OriginPt.x + (j + 1) * 14 > MousePt.x&&OriginPt.y + i * 14 <= MousePt.y&&OriginPt.y + (i + 1) * 14 > MousePt.y)
					{
						MousePt.x = OriginPt.x + j * 14;
						MousePt.y = OriginPt.y + i * 14;
						BUFFER(j, i) = BlockColor;
						break;
					}
				}
			FillBlock(hDlg, MousePt.x, MousePt.y, BlockColor);
			PaintGrid(hDlg, OriginPt);
		}
		return (INT_PTR)TRUE;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		//���ƻ�ͼ���򱳾�
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(250, 250, 250));
		HBRUSH hBrush = CreateSolidBrush(RGB(250, 250, 250));
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 0, 0, WindowRect.right- WindowRect.left, WindowRect.bottom- WindowRect.top);

		//���ƿɻ������򱳾�
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		Rectangle(hdc, center.x - Size / 2, center.y - Size / 2, center.x + Size / 2, center.y + Size / 2);
		
		//��������
		if (IsDlgButtonChecked(hDlgGRID,IDC_CHECK1)==BST_CHECKED)
		{
			PaintAll(hDlg, OriginPt);
			PaintGrid(hDlg, OriginPt);
		}
		else
			PaintAll(hDlg, OriginPt);
		DeleteObject(hPen);
		DeleteObject(hBrush);
		DeleteObject(SelectObject(hdc, hOldPen));
		DeleteObject(SelectObject(hdc, hOldBrush));
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ColorEdit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hBox[20],hMyBox[20],Block;
	int rValue, gValue, bValue;
	int iColor;
	COLORREF rgb;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		for (int i = 0; i < 20; i++)
		{
			hBox[i] = GetDlgItem(hDlg, i + 1);
			hMyBox[i] = GetDlgItem(hDlg, i + 21);
		}
		Block = GetDlgItem(hDlg, IDB_COLOR);
		return (INT_PTR)TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		for (int i = 0; i < 20; i++)
		{
			SetColor(hBox[i], Color[i]);
			SetColor(hMyBox[i], RGB(255, 255, 255));
		}
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_COLOR:
		{
			HDC hdc = GetDC(Block);
			rgb = GetPixel(hdc, 5, 5);
			rValue = GetRValue(rgb);
			gValue = GetGValue(rgb);
			bValue = GetBValue(rgb);
			BlockColor = RGB(rValue, gValue, bValue);
			ReleaseDC(Block, hdc);
			break;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		{
			iColor = LOWORD(wParam);
			HDC hdc = GetDC((HWND)lParam);
			rgb = GetPixel(hdc, 5, 5);
			rValue = GetRValue(rgb);
			gValue = GetGValue(rgb);
			bValue = GetBValue(rgb);
			BlockColor = RGB(rValue, gValue, bValue);
			ReleaseDC((HWND)lParam, hdc);

			HDC hDrawingDC = GetDC(hDlgDRAWINGINT);
			HPEN hPen = CreatePen(PS_SOLID, 1, BlockColor);
			hPen = (HPEN)SelectObject(hDrawingDC, hPen);
			HBRUSH hBrush = CreateSolidBrush(BlockColor);
			hBrush = (HBRUSH)SelectObject(hDrawingDC, hBrush);
			DeleteObject(SelectObject(hDrawingDC, hPen));
			DeleteObject(SelectObject(hDrawingDC, hBrush));
			ReleaseDC(hDlgDRAWINGINT, hDrawingDC);

			SetColor(Block, BlockColor);
			break;
		}
		default:
			break;
		}
	
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SaveOption(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDC_RADIO2, BM_SETCHECK, 1, 0);
		return (INT_PTR)TRUE;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GridOption(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDC_RADIO1, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hDlg, IDC_CHECK1, BM_SETCHECK, 1, 0);
		return (INT_PTR)TRUE;
		break;
	case WM_PAINT:
	{
		RECT WindowRect;
		GetWindowRect(hDlg, &WindowRect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1:
			InvalidateRect(GetParent(hDlg), NULL, false);
			UpdateWindow(GetParent(hDlg));
			break;
		case IDC_CHECK2:
			MessageBox(hDlg, L"��Ҳ��֪�����Ǹ����õ�", L"¥��Խ", MB_OK);
			break;
		case IDC_RADIO1:
			Size = SMALLSIZE;
			GridSize = SMALLGRID;
			SendMessage(hDlgDRAWINGINT, WM_INITDIALOG, 0, 0);
			if (pBuffer != NULL)
				free(pBuffer);
			pBuffer = (COLORREF*)malloc(GridSize*GridSize*sizeof(COLORREF));
			for (int i = 0; i < GridSize; i++)
				for (int j = 0; j < GridSize; j++)
					BUFFER(i, j) = RGB(255, 255, 255);
			InvalidateRect(GetParent(hDlg), NULL, false);
			UpdateWindow(GetParent(hDlg));
			break;
		case IDC_RADIO2:
			//���Ļ�����С
			Size = MEDIUMSIZE;
			GridSize = MEDIUMGRID;

			SendMessage(hDlgDRAWINGINT, WM_INITDIALOG, 0, 0);
			if (pBuffer != NULL)
				free(pBuffer);
			pBuffer = (COLORREF*)malloc(GridSize*GridSize*sizeof(COLORREF));
			for (int i = 0; i < GridSize; i++)
				for (int j = 0; j < GridSize; j++)
					BUFFER(i, j) = RGB(255, 255, 255);

			InvalidateRect(GetParent(hDlg), NULL, false);
			UpdateWindow(GetParent(hDlg));
			break;
		case IDC_RADIO3:
			Size = LARGESIZE;
			GridSize = LARGEGRID;

			SendMessage(hDlgDRAWINGINT, WM_INITDIALOG, 0, 0);
			if (pBuffer != NULL)
				free(pBuffer);
			pBuffer = (COLORREF*)malloc(GridSize*GridSize*sizeof(COLORREF));
			for (int i = 0; i < GridSize; i++)
				for (int j = 0; j < GridSize; j++)
					BUFFER(i, j) = RGB(255, 255, 255);

			InvalidateRect(GetParent(hDlg), NULL, false);
			UpdateWindow(GetParent(hDlg));
			break;
		}
		return (INT_PTR)TRUE;
	}
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK DrawingSave(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT WindowRect,DrawingRect;
	static POINT center, OriginPt;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		GetWindowRect(hDlg, &WindowRect);
		center.x = (WindowRect.right - WindowRect.left) / 2;
		center.y = (WindowRect.bottom - WindowRect.top) / 2;
		OriginPt.x = center.x - Size/2;
		OriginPt.y = center.y - Size/2;
		DrawingRect.left = center.x - Size / 2;
		DrawingRect.top = center.y - Size / 2;
		DrawingRect.right = center.x + Size / 2;
		DrawingRect.bottom = center.y + Size / 2;
		return (INT_PTR)TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, center.x - Size / 2, center.y - Size / 2, center.x + Size / 2, center.y + Size / 2);
		DeleteObject(hPen);
		DeleteObject(hBrush);
		DeleteObject(SelectObject(hdc, hOldPen));
		DeleteObject(SelectObject(hdc, hOldBrush));
		if(IsDlgButtonChecked(hDlgSAVE,IDC_RADIO2)==BST_CHECKED)
			PaintAll(hDlg, OriginPt);
		else
		{
			PaintAll(hDlg, OriginPt);
			PaintGrid(hDlg, OriginPt);
		}
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	case WM_CLOSE:
		SaveBMPFile(hDlg, DrawingRect);
		EndDialog(hDlg, (INT_PTR)TRUE);
	}
	return (INT_PTR)FALSE;
}


void FillBlock(HWND hwnd, int x, int y, COLORREF rgb)
{
	HDC hdc = GetDC(hwnd);
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	hPen = CreatePen(PS_SOLID, 1, rgb);
	hBrush = CreateSolidBrush(rgb);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, x,y,x+14,y+14);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	DeleteObject(SelectObject(hdc, hOldPen));
	DeleteObject(SelectObject(hdc, hOldBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintAll(HWND hwnd,POINT OriginPt)
{
	HDC hdc = GetDC(hwnd);
	HPEN hPen=NULL,hOldPen;
	HBRUSH hBrush=NULL,hOldBrush;
	hOldPen=(HPEN)SelectObject(hdc, hPen);
	hOldBrush=(HBRUSH)SelectObject(hdc, hBrush);
	for (int i = 0; i < GridSize; i++)
		for (int j = 0; j < GridSize; j++)
		{
			hPen = CreatePen(PS_SOLID, 1, BUFFER(j, i));
			hBrush = CreateSolidBrush(BUFFER(j, i));
			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);
			Rectangle(hdc, OriginPt.x + j * 14, OriginPt.y + i * 14, OriginPt.x + j * 14 + 14, OriginPt.y + i * 14 + 14);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
	DeleteObject(SelectObject(hdc, hOldPen));
	DeleteObject(SelectObject(hdc, hOldBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintGrid(HWND hwnd, POINT OriginPt)
{
	HDC hdc = GetDC(hwnd);
	HPEN hPen;
	hPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	hPen = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, OriginPt.x, OriginPt.y, NULL);
	for (int i = 0; i <= GridSize; i++)
	{
		MoveToEx(hdc, OriginPt.x + i * 14, OriginPt.y, NULL);
		LineTo(hdc, OriginPt.x + i * 14, OriginPt.y + Size);
	}
	for (int i = 0; i <= GridSize; i++)
	{
		MoveToEx(hdc, OriginPt.x, OriginPt.y + i * 14, NULL);
		LineTo(hdc, OriginPt.x + Size, OriginPt.y + i * 14);
	}
	DeleteObject(SelectObject(hdc, hPen));
	ReleaseDC(hwnd, hdc);
}

void SetColor(HWND hwnd, COLORREF color)
{
	HBRUSH          hBrush;
	HPEN			hPen;
	HDC             hdc;
	RECT            rect;

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);                    //�õ���ǰ��Ļ��С
	hPen = CreatePen(PS_SOLID, 1, RGB(218, 218, 218));
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	hBrush = CreateSolidBrush(color);  //������Ӧ��ˢ����ɫ
	hPen = CreatePen(PS_SOLID, 1, color);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hPen = (HPEN)SelectObject(hdc, hPen);
	Rectangle(hdc, rect.left+2, rect.top+2, rect.right-2, rect.bottom-2);  //ָ��Ϳɫ����
	DeleteObject(SelectObject(hdc, hPen));
	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnd, hdc);
}


