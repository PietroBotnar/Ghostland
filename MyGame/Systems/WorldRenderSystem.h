#pragma once
#include "../ECS/ISystem.h"
#include "../ECS/World.h"
#include "../Data//Vector3f.h"

#include "../Objects//SkyboxTerrain.h"
#include "../Components/MeshAssetComponent.h"

#include "..\include/glew.h"
#include "..\include/assimp/Importer.hpp"      // C++ importer interface
#include "..\include/assimp/scene.h"       // Output data structure
#include "..\include/assimp/PostProcess.h" 

#define _USE_MATH_DEFINES								
#include <math.h>
#include <wtypes.h>
#include <vector>
#include "../Objects/MeshEntry.h"
#include "../Objects/Texture.h"
#include <map>
#include "../Data/Enums.h"
#include <string>
#include "../ECS/GameFSM.h"

struct Mesh
{
	bool IsValid;
	std::vector<MeshEntry> MeshEntries;
	std::vector<CTexture*> Textures;
};

//Renders all objects present in the game world
class WorldRenderSystem : public ISystem
{
public:
	explicit WorldRenderSystem(){};

	~WorldRenderSystem(){};

	void InitialiseSystem() override;
	void Update(float dt) override;
	void Render();

	SkyboxTerrain		skybox_terrain;

	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;

private:
	
	//textures
	UINT diamond_texture;
	UINT pyramid_texture;
	UINT prism_texture;
	UINT wall_texture;

	//render gameobjects
	void RenderPlayerBullet(CVector3f* position);
	void RenderMesh(MeshID meshID, TransformComponent* transform);
	void RenderSphere(TransformComponent* transform);
	void RenderOctahedron(TransformComponent* transform);
	void RenderPyramid(TransformComponent* transform);
	void RenderPrism(TransformComponent* transform);
	void RenderBox(TransformComponent* transform);
	void RenderWall(TransformComponent* transform);
	void RenderLighthouseLights(TransformComponent* transform);
	void RenderGrenade(TransformComponent* transform);

	std::map<MeshID, Mesh*> _meshes;

	//mesh loading
	Mesh* LoadMesh(const std::string& filename);
	bool InitFromScene(Mesh* mesh, const aiScene* pScene, const std::string& Filename);
	void InitMesh(Mesh* mesh, unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(Mesh* mesh, const aiScene* pScene, const std::string& Filename);

	//calls gl Transform/Rotate/Scale
	void glTRS(TransformComponent* transform);

	CVector3f FromGLfloat(const GLfloat* vector);

	bool _playing = false;
};

