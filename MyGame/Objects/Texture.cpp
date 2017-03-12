#include "..\include\glew.h"
#include "..\Objects\Texture.h"

#include "..\include\freeimage\FreeImage.h"

#pragma comment(lib, "lib/FreeImage.lib")



CTexture::CTexture()
{}

CTexture::~CTexture()
{}



// Create a texture from the data stored in bData.  
void CTexture::CreateFromData(BYTE* bData, int iWidth, int iHeight, int iBPP, GLenum format, bool clamp)
{
	// Generate an OpenGL texture ID for this texture
	glEnable(GL_TEXTURE_2D);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 4); // Free image is designed to have word alignment
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	if(format == GL_RGBA || format == GL_BGRA) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);
	} else if(format == GL_RGB || format == GL_BGR) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);
	} else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 1, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);

	// Set texture filtering modes
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if (clamp) {
		// The default GL_TEXTURE_WRAP_S and ""_WRAP_T property is GL_REPEAT.
		// We need to turn this to GL_CLAMP_TO_EDGE, otherwise it creates ugly seems
		// in our sky box.  GL_CLAMP_TO_EDGE does not repeat when bound to an object.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	
	m_width = iWidth;
	m_height = iHeight;
}

// Loads a 2D texture given the filename (sPath) using FreeImage (http://freeimage.sourceforge.net/).  
bool CTexture::Load(string sPath, bool clamp)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sPath.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sPath.c_str());

	if(!dib) {
		char message[1024];
		sprintf_s(message, "Cannot load image\n%s\n", sPath.c_str());
		MessageBox(NULL, message, "Error", MB_ICONERROR);
		return false;
	}

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	unsigned int width = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);
	unsigned int bpp = FreeImage_GetBPP(dib);
	if(bDataPointer == NULL || width == 0 || height == 0) 
		return false;

	GLenum format;
	if(bpp == 32)format = GL_BGRA;
	if(bpp == 24)format = GL_BGR;
	if(bpp == 8) format = GL_DEPTH_COMPONENT;
	

	CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, clamp);

	
	FreeImage_Unload(dib);


	m_sPath = sPath;

	
	return true; // Success
}



void CTexture::CreateFromRGB(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	GLuint textureId;
	glGenTextures(1, &textureId);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, textureId);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);

	m_textureID = textureId;

}

void CTexture::Bind()
{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
}