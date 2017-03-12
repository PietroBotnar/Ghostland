#include "SkyboxTerrain.h"
#include "Texture.h"
#include <string>


SkyboxTerrain::SkyboxTerrain()
{
}

SkyboxTerrain::~SkyboxTerrain()
{}

bool SkyboxTerrain::Create(const char *bmpDirectory, const float fSize)
{
	CTexture skyboxTexture;
	CTexture terrainTexture;

	string folder = "nebula";


	//load skybox
	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\bk.bmp", true);
	skybox_textureID[SKYBOX_BACK_ID] = skyboxTexture.m_textureID;

	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\ft.bmp", true);
	skybox_textureID[SKYBOX_FRONT_ID] = skyboxTexture.m_textureID;

	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\dn.bmp", true);
	skybox_textureID[SKYBOX_BOTTOM_ID] = skyboxTexture.m_textureID;

	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\up.bmp", true);
	skybox_textureID[SKYBOX_TOP_ID] = skyboxTexture.m_textureID;

	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\lf.bmp", true);
	skybox_textureID[SKYBOX_LEFT_ID] = skyboxTexture.m_textureID;

	skyboxTexture.Load("Resources\\Textures\\Skybox\\" + folder + "\\rt.bmp", true);
	skybox_textureID[SKYBOX_RIGHT_ID] = skyboxTexture.m_textureID;

	//load terrain
	terrainTexture.Load("Resources\\Textures\\terrain_dark.png", false);
	terrain_textureID = terrainTexture.m_textureID;
	return true;
}


void SkyboxTerrain::RenderSkybox(float x, float y, float z, float width, float height, float length)
{
	glColor3f(1, 1, 1);

	// Bind the BACK texture of the sky map to the BACK side of the cube
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_BACK_ID]);

	// This centers the sky box around (x, y, z)
	x = x - width  / 2;
	y = (y - height / 2) + 5;
	z = z - length / 2;

	 glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////BACK SIDE
		// Start drawing the side as a QUAD
	 glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		
	glEnd();

	// Bind the FRONT texture of the sky map to the FRONT side of the box
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_FRONT_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////FRONT SIDE
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);	
	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_BOTTOM_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////BOTTOM SIDE
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
	
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// Bind the TOP texture of the sky map to the TOP side of the box
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_TOP_ID]);
	
	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////TOP SIDE
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the TOP Side
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();

	// Bind the LEFT texture of the sky map to the LEFT side of the box
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_LEFT_ID]);
	
	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////LEFT SIDE
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	// Bind the RIGHT texture of the sky map to the RIGHT side of the box
	glBindTexture(GL_TEXTURE_2D, skybox_textureID[SKYBOX_RIGHT_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////RIGHT SIDE
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		

		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}

void SkyboxTerrain::RenderTerrain()
{
	float TERRAIN_SIZE = 1000;

	GLfloat x, y, z, length, width;
	length = TERRAIN_SIZE / 2.0f;
	width = TERRAIN_SIZE / 2.0f;

	// Centre terrain around 0, 0, 0;
	x = -width / 2;
	y = 0;
	z = -length / 2;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);

	// Bind the texture	
	glBindTexture(GL_TEXTURE_2D, terrain_textureID);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glNormal3f(0.0f, 1.0f, 0.0f);

	// Draw a quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();
}
