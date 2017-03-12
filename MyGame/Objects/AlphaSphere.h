#pragma once
#include <windows.h>									// Header File For The Windows Library

#include "..\include\gl.h"											// Header File For The OpenGL32 Library
#include "..\include\glut.h"											// Header File For The OpenGL32 Library
#include "..\Data\Vector3f.h"

class CAlphaSphere
{
public:
	CAlphaSphere();
	~CAlphaSphere();
	void Activate(CVector3f position, float radius, CVector3f startColour, CVector3f endColour, bool zap, float zapLevel);
	void Update(float dt);
	void Render();

	bool IsActive() {return m_isActive;}
	void Disable(){m_isActive = false;}

private:

	float Random();

	CVector3f mpvPosition;
	float m_radius;
	bool m_isActive = false;
	float m_totalTime;
	float m_elapsedTime;
	float m_zapLevel;

	CVector3f m_startColour;
	CVector3f m_endColour;
	CVector3f m_colour;
	bool m_zap;

	
};