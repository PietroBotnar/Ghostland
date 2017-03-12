#pragma once
#include "..\ECS\IComponent.h"
#include "..\Data\Enums.h"

//Holds the ID of the mesh to be rendered
class MeshAssetComponent : public IComponent
{
public:
	explicit MeshAssetComponent(MeshID meshID)
	{
		MeshID = meshID;
	};

	~MeshAssetComponent(){};

	MeshID MeshID;
};