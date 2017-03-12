#pragma once
#include <windows.h>		
#include "..\include\glew.h"							
#include "..\include\gl.h"								
#include <string>
#include <vector>
#include <map>

typedef enum {
	SKYBOX_BACK_ID,
	SKYBOX_FRONT_ID,
	SKYBOX_BOTTOM_ID,
	SKYBOX_TOP_ID,
	SKYBOX_LEFT_ID,
	SKYBOX_RIGHT_ID
} BoxSideTypes;

class SkyboxTerrain
{
public:
	SkyboxTerrain();
	~SkyboxTerrain();
	bool Create(const char *bmpDirectory, const float fSize);

	void RenderSkybox(float x, float y, float z, float width, float height, float length);
	void RenderTerrain();


private:
	unsigned int skybox_textureID[6];
	UINT terrain_textureID;
};