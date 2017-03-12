#ifndef VERTEX_H
#define VERTEX_H

class CVertex
{
public:
	CVertex(float xIn, float yIn, float zIn, float nxIn, float nyIn, float nzIn, float sIn, float tIn)
	{
		x = xIn;
		y = yIn;
		z = zIn;
		nx = nxIn;
		ny = nyIn;
		nz = nzIn;
		s = sIn;
		t = tIn;
	};


	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

#endif