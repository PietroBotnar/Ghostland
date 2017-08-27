#include "gamewindow.h"

#include ".\include\glew.h"									// Header File For The Glaux Library
#include ".\include\gl.h"											// Header File For The OpenGL32 Library
#include ".\include\glu.h"									// Header File For The GLu32 Library
#include <stdio.h>

GameWindow& GameWindow::GetInstance()
{
	static GameWindow instance;

	return instance;
}

GameWindow::GameWindow()
{
	m_fullScreen = false; // set to true for full screen
}

HDC GameWindow::Init(HINSTANCE hinstance)
{
	m_hinstance = hinstance;

	CreateGameWindow("GHOSTLAND");

	// If we never got a valid window handle, quit the program
	if (m_hwnd == NULL) {
		return NULL;
	}
	else {
		// INIT OpenGL
		InitOpenGL();
		return m_hdc;
	}
}

void GameWindow::CreateGameWindow(LPSTR title)
{
	m_sClass = title;
	WNDCLASS hwnd_class;
	memset(&hwnd_class, 0, sizeof(WNDCLASS));

	hwnd_class.style = CS_HREDRAW | CS_VREDRAW;
	hwnd_class.lpfnWndProc = WinProc;
	hwnd_class.hInstance = m_hinstance;
	hwnd_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	hwnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	hwnd_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	hwnd_class.lpszClassName = m_sClass;

	RegisterClass(&hwnd_class);

	DWORD style;
	if (m_fullScreen) {
		style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		ChangeToFullScreen();
		ShowCursor(FALSE);
	}
	else {
		style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor(FALSE);
	}

	RECT boundaries;
	boundaries.left = 0;
	boundaries.right = SCREEN_WIDTH;
	boundaries.top = 0;
	boundaries.bottom = SCREEN_HEIGHT;

	AdjustWindowRect(&boundaries, style, false);

	m_hwnd = CreateWindow(m_sClass, m_sClass, style, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, m_hinstance, NULL);

	if (m_hwnd) {
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);

		SetFocus(m_hwnd);
	}
}

void GameWindow::InitOpenGL()
{
	GetClientRect(m_hwnd, &m_dimensions);
	m_hdc = GetDC(m_hwnd);

	if (!SetupPixelFormat())
		PostQuitMessage(0);

	m_hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hrc);

	// Initialse GLEW
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	SizeOpenGLScreen(m_dimensions.right, m_dimensions.bottom);
}

bool GameWindow::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = SCREEN_DEPTH;
	pfd.cDepthBits = SCREEN_DEPTH;
	pfd.cAccumBits = 0;
	pfd.cStencilBits = 0;

	int format = ChoosePixelFormat(m_hdc, &pfd);
	if (format == FALSE) {
		MessageBox(NULL, "ChoosePixelFormat failed.", "Error", MB_OK);
		return false;
	}

	if (SetPixelFormat(m_hdc, format, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat failed.", "Error", MB_OK);
		return false;
	}

	return true;
}

void GameWindow::SizeOpenGLScreen(int width, int height)
{
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.5f, 3000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameWindow::ChangeToFullScreen()
{
	DEVMODE settings = { 0 };

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &settings)) {
		MessageBox(NULL, "Could not retrieve display settings.", "Error", MB_OK);
		return;
	}

	settings.dmPelsWidth = SCREEN_WIDTH;
	settings.dmPelsHeight = SCREEN_HEIGHT;
	settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	int result = ChangeDisplaySettings(&settings, CDS_FULLSCREEN);
	if (result != DISP_CHANGE_SUCCESSFUL) {
		MessageBox(NULL, "Display mode not compatible.", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

void GameWindow::Deinit()
{
	if (m_hrc) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);
	}

	if (m_hdc) {
		ReleaseDC(m_hwnd, m_hdc);
	}

	if (m_fullScreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	UnregisterClass(m_sClass, m_hinstance);
	PostQuitMessage(0);
}

void GameWindow::UpdateFrameRate(double dt)
{
	static int framesPerSecond = 0;
	static float elapsedTime = 0.0f;

	// Increase the elapsed time and frame counter
	elapsedTime += (float)dt;
	framesPerSecond++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (elapsedTime > 1.0f)
	{
		elapsedTime = 0.0f;

		//// Copy the frames per second into a string to display in the window title bar
		char strFrameRate[50] = { 0 };			// We will store the string here for the window title
		sprintf_s(strFrameRate, "Current Frames Per Second: %d", int(framesPerSecond));

		//// Set the window title bar to our string
		SetWindowText(GameWindow::m_hwnd, strFrameRate);

		// Reset the frames per second
		framesPerSecond = 0;
	}
}