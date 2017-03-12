#pragma once
#include <windows.h>									// Header File For The Windows Library

#include "..\include\gl.h"											// Header File For The OpenGL32 Library
#include "..\Objects\Texture.h"
#include <vector>
#include "..\Data\Vector3f.h"


class CCrossFade
{
public:
	CCrossFade();
	~CCrossFade();
	void Activate(CVector3f rgb);
	void Render();
	void Update(float dt);

	void Disable();
private:

	void SetOrthographicProjection();
	void RestorePerspectiveProjection();
	GLint m_viewport[4];

	bool m_isActive;
	float m_elapsedTime;
	float m_totalTime;
	float m_alpha;

	CVector3f color;
};