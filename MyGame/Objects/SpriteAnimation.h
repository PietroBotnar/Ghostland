#pragma once
#include <windows.h>									// Header File For The Windows Library
#include "..\include\gl.h"								// Header File For The OpenGL32 Library
#include "..\Objects\Texture.h"
#include "..\Data\Vector3f.h"

//THIS CLASS IS A MODIFICATION OF THE PROVIDED CODE IN CSpriteExplosion

//Plays 2D a sprite animation
class SpriteAnimation
{
public:
	SpriteAnimation();
	SpriteAnimation(const char *filename, const int frameWidth, const int frameHeight, const int numFrames);
	~SpriteAnimation();

	void Update(float dt);
	void Render();

	//Activate the animation with the given properties
	void Activate(CVector3f p, CVector3f lookAt, float width, float height);

	bool IsActive(){return m_active;}

private:
	bool m_active;

	unsigned int m_frame;
	unsigned int m_totalFrames;
	float m_elapsedTime;

	unsigned int m_frameWidth;
	unsigned int m_frameHeight;

	unsigned int m_framesX;
	unsigned int m_framesY;

	CVector3f position;
	CVector3f lookAt;
	float width;
	float height;

	CTexture m_texture;
};