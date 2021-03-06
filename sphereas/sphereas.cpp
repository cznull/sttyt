
#include "stdafx.h"
#include "sphereas.h"
#include "vec.h"
#include <gl/glew.h>
#include <stdio.h>
#include <vector>

//#include <opencv2/opencv.hpp>


//#pragma comment(lib,"opencv_world401.lib")
//#pragma comment(lib,"opencv_world401d.lib")

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
//#pragma comment(lib,"glew32.lib")

#define MAX_LOADSTRING 100
#define PI 3.14159265358979324

int tl;
int c = 0;
int next[4];
int fi = 1;

int fileindex(void) {
	FILE *fi;
	int index = 0;
	if (!fopen_s(&fi, "D:/files/data/sp/img/index.txt", "rb")) {
		fscanf_s(fi, "%d", &index);
		fclose(fi);
	}
	if (!fopen_s(&fi, "D:/files/data/sp/img/index.txt", "wb")) {
		fprintf(fi, "%d", index + 1);
		fclose(fi);
	}
	return index;
}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HDC hdc1, hdc2;
HGLRC m_hrc;
int mx, my, cx, cy;
double ang1, ang2, len, cenx, ceny, cenz;
GLuint imgtex;

HWND bbhwnd;
HWND bbhwnd1;
HDC bbhdc, bbhdcc;
HBITMAP hbitmap;
BITMAP bmp;
RECT bbrect;
std::vector<float2> lines;
unsigned char *data;

int arl[512];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//cv::Mat *frame; 

void draw(void) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(0x00004100);

	glBindTexture(GL_TEXTURE_2D, imgtex);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0, -1.0f, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(-1.0f, 1.0f, 0.0f);
	glScalef(2.0f / bmp.bmWidth, -2.0f / bmp.bmHeight, 1.0f);
	glVertexPointer(2, GL_FLOAT, 0, lines.data());
	glDrawArrays(GL_LINES, 0, lines.size());
	SwapBuffers(hdc1);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPHEREAS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPHEREAS));

    MSG msg;

    // Main message loop:
	for (;;) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (bbhwnd) {
				//PrintWindow(bbhwnd, bbhdcc, 1);
				BitBlt(bbhdcc, 720, 140, 500, 800, bbhdc, 720, 140, SRCCOPY);
				GetBitmapBits(hbitmap, bmp.bmHeight*bmp.bmWidthBytes, data);
				int y0 = 668;
				int x0 = 960;
				int i;
				for (i = 0; i < 4; i++) {
					if (data[(y0-18) * bmp.bmWidth * 4 + (x0+80) * 4 + 1] == 103&& data[(y0-35) * bmp.bmWidth * 4 + (x0 + 44) * 4 + 1] == 103) {
						next[i] = 0;
						y0 -= 45;
						x0 += 45;
					}
					else if (data[(y0-18) * bmp.bmWidth * 4 + (x0-80) * 4 + 1] == 103 && data[(y0 - 35) * bmp.bmWidth * 4 + (x0 - 44) * 4 + 1] == 103) {
						next[i] = 1;
						y0 -= 45;
						x0 -= 45;
					}
					else {
						break;
					}
				}
				if (i == 4) {
					c++;
					for (i = 0; i < 4; i++) {
						if (next[i]) {
							SendMessageA(bbhwnd1, WM_LBUTTONDOWN, 0, 800 + (631 << 16));
							SendMessageA(bbhwnd1, WM_LBUTTONUP, 0, 800 + (631 << 16));
							Sleep(160);
						}
						else {
							SendMessageA(bbhwnd1, WM_LBUTTONDOWN, 0, 1120 + (631 << 16));
							SendMessageA(bbhwnd1, WM_LBUTTONUP, 0, 1120 + (631 << 16));
							Sleep(160);
						}
					}
					Sleep(200);
				}
			}
		}
	}
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPHEREAS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPHEREAS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            // TODO: Add any drawing code that uses hdc here...
			draw();
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CREATE: {
		int i, j;
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,0,0,
			0,
			0,0,0,0,
			32,
			0,0,
			PFD_MAIN_PLANE,
			0,0,0,0
		};
		hdc1 = GetDC(hWnd);
		hdc2 = GetDC(NULL);
		int uds = ::ChoosePixelFormat(hdc1, &pfd);
		::SetPixelFormat(hdc1, uds, &pfd);
		m_hrc = ::wglCreateContext(hdc1);
		::wglMakeCurrent(hdc1, m_hrc);
		//glewInit();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);

		glGenTextures(1, &imgtex);
		glBindTexture(GL_TEXTURE_2D, imgtex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		bbhwnd1 = FindWindowA(NULL, "北大食堂“跳一跳” - Google Chrome");
		bbhwnd = GetDesktopWindow();
		bbhdc = GetDC(bbhwnd);
		bbhdcc = CreateCompatibleDC(bbhdc);
		GetClientRect(bbhwnd, &bbrect);
		hbitmap = CreateCompatibleBitmap(bbhdc, bbrect.right - bbrect.left, bbrect.bottom - bbrect.top);
		SelectObject(bbhdcc, hbitmap);
		BitBlt(bbhdcc, 0, 0, bbrect.right - bbrect.left, bbrect.bottom - bbrect.top, bbhdc, 0, 0, SRCCOPY);
		//PrintWindow(bbhwnd, bbhdcc, PW_CLIENTONLY);
		GetObject(hbitmap, sizeof(BITMAP), &bmp);
		//frame = new cv::Mat(bmp.bmHeight, bmp.bmWidth, CV_8UC4, cv::Scalar(0, 0, 0, 0));
		data = (unsigned char *)malloc(bmp.bmHeight*bmp.bmWidth * 4);
		GetBitmapBits(hbitmap, bmp.bmHeight*bmp.bmWidthBytes, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.bmWidth, bmp.bmHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		RECT ra, rb;
		GetClientRect(hWnd, &ra);
		GetWindowRect(hWnd, &rb);
		MoveWindow(hWnd, 0, 0, bmp.bmWidth/4+rb.right-ra.right+ra.left-rb.left, bmp.bmHeight/4+rb.bottom-ra.bottom+ra.top-rb.top, true);
		break;
	}
	case WM_SIZE: {
		cx = lParam & 0xffff;
		cy = (lParam & 0xffff0000) >> 16;
		/*if (cx <= cy) {
		glViewport(0, (cy - cx) / 2, cx, cx);
		}
		else {
		glViewport((cx - cy) / 2, 0, cy, cy);
		}*/
		glViewport(0, 0, cx, cy);
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
