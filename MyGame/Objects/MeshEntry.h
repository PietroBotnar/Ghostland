#pragma once
#include "..\Data\Vertex.h"
#include <vector>

#define INVALID_OGL_VALUE 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define INVALID_MATERIAL 0xFFFFFFFF

struct MeshEntry {
	MeshEntry()
	{
		NumIndices = 0;
		MaterialIndex = INVALID_MATERIAL;
	};

	~MeshEntry() {};

	void Init(const std::vector<CVertex>& Vertices,
		const std::vector<unsigned int>& Indices)
	{
		NumIndices = Indices.size();
	};

	std::vector<CVertex> Vertices;
	std::vector<unsigned int> Indices;

	unsigned int NumIndices;
	unsigned int MaterialIndex;

};
