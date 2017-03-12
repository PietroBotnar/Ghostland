#ifndef GAMEhwnd_H
#define GAMEhwnd_H

#include <windows.h>

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

class GameWindow {
public:
	static GameWindow& GetInstance();
	GameWindow();

	enum {
		SCREEN_WIDTH = 1280,
		SCREEN_HEIGHT = 720,
		SCREEN_DEPTH = 24
	};

	HDC Init(HINSTANCE hinstance);
	void Deinit();

	void SizeOpenGLScreen(int width, int height);
	void SetDimensions(RECT dimensions) {m_dimensions = dimensions;}

	bool GetFullScreen() const { return m_fullScreen; }
	HDC GetHdc() const { return m_hdc; }
	HINSTANCE GetHinstance() const { return m_hinstance; }
	HGLRC GetHrc() const { return m_hrc; }
	HWND  GetHwnd() const { return m_hwnd; }
	void UpdateFrameRate(double dt);

private:
	GameWindow(const GameWindow&);
	void operator = (const GameWindow&);

	bool SetupPixelFormat();
	void ChangeToFullScreen();
	void CreateGameWindow(LPSTR title);
	void InitOpenGL();


	HDC   m_hdc;
	HINSTANCE m_hinstance;
	HGLRC m_hrc;
	HWND  m_hwnd;

	bool  m_fullScreen;
	LPSTR m_sClass;
	RECT  m_dimensions;
};

#endif