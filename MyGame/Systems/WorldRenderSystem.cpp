#include "WorldRenderSystem.h"

#include "../ECS/World.h"
#include "../Components/MovementComponent.h"
#include "../Components/TransformComponent.h"
#include "../Objects/Texture.h"
#include "../include/glut.h"
#include "../include/assimp/scene.h"
#include <windows.h>
#include <assert.h>
#include <vector>

#pragma comment(lib, "lib/assimp.lib")


void WorldRenderSystem::InitialiseSystem()
{
	//Init skybox and terrain
	skybox_terrain.Create("", 0.0f);

	// Load the textures
	CTexture diamond;
	diamond.Load("Resources\\Textures\\diamond_texture_mod.bmp", false);
	diamond_texture = diamond.m_textureID;

	CTexture pyramid;
	pyramid.Load("Resources\\Textures\\pyramid_texture.bmp", false);
	pyramid_texture = pyramid.m_textureID;

	CTexture prism;
	prism.Load("Resources\\Textures\\prism_texture.jpg", false);
	prism_texture = prism.m_textureID;

	CTexture wall;
	wall.Load("Resources\\Textures\\wall_texture.jpg", false);
	wall_texture = wall.m_textureID;

	//Load and map meshes
	_meshes[Player]			= LoadMesh("Resources\\Meshes\\pacman.3ds");
	_meshes[RedGhost]		= LoadMesh("Resources\\Meshes\\red_ghost.3ds");
	_meshes[GreenGhost]		= LoadMesh("Resources\\Meshes\\green_ghost.3ds");
	_meshes[BlueGhost]		= LoadMesh("Resources\\Meshes\\blue_ghost.3ds");
	_meshes[OrangeGhost]	= LoadMesh("Resources\\Meshes\\orange_ghost.3ds");
	_meshes[YellowGhost]	= LoadMesh("Resources\\Meshes\\yellow_ghost.3ds");
	_meshes[PinkGhost]		= LoadMesh("Resources\\Meshes\\pink_ghost.3ds");
	_meshes[Lighthouse]		= LoadMesh("Resources\\Meshes\\faro.3ds");
	_meshes[LighhouseLights] = LoadMesh("Resources\\Meshes\\faro_lights.3ds");
	_meshes[Grenade]		= LoadMesh("Resources\\Meshes\\mk.3ds");

	glClearColor(0, 0, 0, 1);
}

void WorldRenderSystem::Update(float dt)
{
	if(!_playing)	return;

	Render();
}

void WorldRenderSystem::OnEvent(IEvent* _event)
{
	//events here
}

void WorldRenderSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;
}

void WorldRenderSystem::Render()
{
	// Get the camera position and view vector
	CVector3f vPos = _world->GetCameraPosition();
	CVector3f vView = _world->GetCameraViewPoint();

	//do not light up skybox and terrain
	glDisable(GL_LIGHTING);

	skybox_terrain.RenderSkybox(vPos.x, vPos.y, vPos.z, 1000, 500, 1000);
	skybox_terrain.RenderTerrain();

	glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for (auto& entity : _world->EntityManager->_entities)
		{
			if (!entity->IsAlive() || !entity->Matches(Transform))
				continue;

			if (entity->Matches(Prefab))
			{
				auto go = entity->GetPrefab();
				switch (go->Type)
				{
				case Sphere:
				//	RenderSphere(entity->GetTransform());
					RenderGrenade(entity->GetTransform());
					break;
				case Octahedron:
					RenderOctahedron(entity->GetTransform());
					break;
				case PlayerBullet:
					RenderPlayerBullet(&entity->GetTransform()->Position);
					break;
				case Pyramid:
					RenderPyramid(entity->GetTransform());
					break;
				case Prism:
					RenderPrism(entity->GetTransform());
					break;
				case Box:
					RenderBox(entity->GetTransform());
					break;
				case Wall:
					RenderWall(entity->GetTransform());
					break;
				}
			}

			if (entity->Matches(MeshAsset)) 
			{
				RenderMesh(entity->GetMeshAsset()->MeshID, entity->GetTransform());
			}
		}
		glDisable(GL_LIGHTING);
}

void WorldRenderSystem::RenderMesh(MeshID id, TransformComponent* transform)
{
	if (!_meshes[id]->IsValid) return;

	auto meshAsset = _meshes[id];

	glPushMatrix();

	if (id == LighhouseLights) {
		////set lights material
		GLfloat ambientMaterial[] = { 1.0f, 1.0f, 1.0f, 0.1f };
		GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 0.1f };
		GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 0.1f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
	}
	else {
		////set material
		GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 80.0f);
	}

	if (id == Grenade) {
		auto &position = transform->Position;
		auto &scale = transform->Scale;
		float theta = atan2(transform->Direction.z, transform->Direction.x) * 180.0f / (float)M_PI;
		float m_theta = 90.0f - (180.0f / (float)M_PI) * acos(transform->Direction.y);
		float m_phi = (180.0f / (float)M_PI) * atan2(transform->Direction.x, transform->Direction.z);
		glTranslatef(position.x, position.y, position.z);
		glRotatef(m_phi, 0, 1, 0);
		glRotatef(-m_theta, 1, 0, 0);
		if (transform->Rotation.Length() > 0)
			glRotatef(transform->Rotation.Length(), transform->Rotation.x, transform->Rotation.y, transform->Rotation.z);	// Rotate the ball resulting from torque
		glScalef(scale.x, scale.y, scale.z);
	}
	else {
		glTRS(transform);
	}

	for (unsigned int i = 0; i < meshAsset->MeshEntries.size(); i++) {

		MeshEntry *pMesh = &meshAsset->MeshEntries[i];

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].s));

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].nx));

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].x));


		const unsigned int MaterialIndex = pMesh->MaterialIndex;
		if (MaterialIndex < meshAsset->Textures.size() && meshAsset->Textures[MaterialIndex])
			meshAsset->Textures[MaterialIndex]->Bind();

		glDrawElements(GL_TRIANGLES, pMesh->NumIndices, GL_UNSIGNED_INT, &(pMesh->Indices[0]));

	}
	glPopMatrix();
}

void WorldRenderSystem::RenderPlayerBullet(CVector3f* position)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor4f(0.89f, 0.35f, 0.13f, 0.9f);
	glPushMatrix();
	glTranslatef(position->x, position->y, position->z);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}

void WorldRenderSystem::RenderSphere(TransformComponent* transform)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glTRS(transform);

	glColor3f(1, 0, 0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void WorldRenderSystem::RenderOctahedron(TransformComponent* transform)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, diamond_texture);

	glPushMatrix();

	////set material
	GLfloat ambientMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0f);

	glTRS(transform);

	GLfloat top[3] = { 0, 1, 0 };

	GLfloat v1[3] = { -1, 0,  1 };
	GLfloat v2[3] = {  1, 0,  1 };
	GLfloat v3[3] = {  1, 0, -1 };
	GLfloat v4[3] = { -1, 0, -1 };

	GLfloat bot[3] = { 0, -1, 0 };

	glBegin(GL_TRIANGLES);

	//face 1
		//calculate normal
	auto edg1 = FromGLfloat(v1) - FromGLfloat(top);
	auto edg2 = FromGLfloat(v2) - FromGLfloat(top);
	auto norm = (edg1 % edg2);
	norm.Normalise();
	glNormal3f(norm.x, norm.y, norm.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(0, 0);
	glVertex3fv(v1);

	glTexCoord2f(0, 1);
	glVertex3fv(v2);


	//face2
	//calculate normal
	auto edg11 = FromGLfloat(v2) - FromGLfloat(bot);
	auto edg12 = FromGLfloat(v1) - FromGLfloat(bot);
	auto norm1 = (edg11 % edg12);
	norm1.Normalise();
	glNormal3f(norm1.x, norm1.y, norm1.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(bot);

	glTexCoord2f(0, 0);
	glVertex3fv(v2);

	glTexCoord2f(0, 1);
	glVertex3fv(v1);


	//face3
	//calculate normal
	auto edg21 = FromGLfloat(v2) - FromGLfloat(top);
	auto edg22 = FromGLfloat(v3) - FromGLfloat(top);
	auto norm2 = (edg21 % edg22);
	norm2.Normalise();
	glNormal3f(norm2.x, norm2.y, norm2.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(0, 1);
	glVertex3fv(v2);

	glTexCoord2f(1, 1);
	glVertex3fv(v3);


	//face 4
	//calculate normal
	auto edg31 = FromGLfloat(v3) - FromGLfloat(bot);
	auto edg32 = FromGLfloat(v2) - FromGLfloat(bot);
	auto norm3 = (edg31 % edg32);
	norm3.Normalise();
	glNormal3f(norm3.x, norm3.y, norm3.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(bot);
	
	glTexCoord2f(0, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 1);
	glVertex3fv(v2);


	//face 5
	//calculate normal
	auto edg41 = FromGLfloat(v3) - FromGLfloat(top);
	auto edg42 = FromGLfloat(v4) - FromGLfloat(top);
	auto norm4 = (edg41 % edg42);
	norm4.Normalise();
	glNormal3f(norm4.x, norm4.y, norm4.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(1, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 0);
	glVertex3fv(v4);


	//face 6
	//calculate normal
	auto edg51 = FromGLfloat(v4) - FromGLfloat(bot);
	auto edg52 = FromGLfloat(v3) - FromGLfloat(bot);
	auto norm5 = (edg51 % edg52);
	norm5.Normalise();
	glNormal3f(norm5.x, norm5.y, norm5.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(bot);
	
	glTexCoord2f(1, 1);
	glVertex3fv(v4);

	glTexCoord2f(1, 0);
	glVertex3fv(v3);


	//face 7
	//calculate normal
	auto edg61 = FromGLfloat(v4) - FromGLfloat(top);
	auto edg62 = FromGLfloat(v1) - FromGLfloat(top);
	auto norm6 = (edg61 % edg62);
	norm6.Normalise();
	glNormal3f(norm6.x, norm6.y, norm6.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(1, 0);
	glVertex3fv(v4);

	glTexCoord2f(0, 0);
	glVertex3fv(v1);


	//face 8
	//calculate normal
	auto edg71 = FromGLfloat(v1) - FromGLfloat(bot);
	auto edg72 = FromGLfloat(v4) - FromGLfloat(bot);
	auto norm7 = (edg71 % edg72);
	norm7.Normalise();
	glNormal3f(norm7.x, norm7.y, norm7.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(bot);
	
	glTexCoord2f(1, 0);
	glVertex3fv(v1);

	glTexCoord2f(0, 0);
	glVertex3fv(v4);

	glEnd();

	glPopMatrix();

	glDisable(GL_CULL_FACE);
}

void WorldRenderSystem::RenderPyramid(TransformComponent* transform)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pyramid_texture);

	glPushMatrix();

	////set material
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

	glTRS(transform);

	GLfloat top[3] = { 0, 1, 0 };

	//pentagon vertices

	GLfloat c1 = 0.31;
	GLfloat c2 = 0.81;
	GLfloat s1 = 0.95;
	GLfloat s2 = 0.59;

	GLfloat v1[3] = { 1, 0,  0 };
	GLfloat v2[3] = { c1, 0,  -s1 };
	GLfloat v3[3] = { -c2, 0, -s2 };
	GLfloat v4[3] = { -c2, 0, s2 };
	GLfloat v5[3] = { c1, 0, s1 };

	glBegin(GL_TRIANGLES);

	//face 1
	//calculate normal
	auto edg1 = FromGLfloat(v1) - FromGLfloat(top);
	auto edg2 = FromGLfloat(v2) - FromGLfloat(top);
	auto norm = (edg1 % edg2);
	norm.Normalise();
	glNormal3f(norm.x, norm.y, norm.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(0, 0);
	glVertex3fv(v1);

	glTexCoord2f(0, 1);
	glVertex3fv(v2);


	//face2
	//calculate normal
	auto edg21 = FromGLfloat(v2) - FromGLfloat(top);
	auto edg22 = FromGLfloat(v3) - FromGLfloat(top);
	auto norm2 = (edg21 % edg22);
	norm2.Normalise();
	glNormal3f(norm2.x, norm2.y, norm2.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(0, 1);
	glVertex3fv(v2);

	glTexCoord2f(1, 1);
	glVertex3fv(v3);

	//face 3
	//calculate normal
	auto edg41 = FromGLfloat(v3) - FromGLfloat(top);
	auto edg42 = FromGLfloat(v4) - FromGLfloat(top);
	auto norm4 = (edg41 % edg42);
	norm4.Normalise();
	glNormal3f(norm4.x, norm4.y, norm4.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(1, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 0);
	glVertex3fv(v4);

	//face 4
	//calculate normal
	auto edg61 = FromGLfloat(v4) - FromGLfloat(top);
	auto edg62 = FromGLfloat(v5) - FromGLfloat(top);
	auto norm6 = (edg61 % edg62);
	norm6.Normalise();
	glNormal3f(norm6.x, norm6.y, norm6.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(1, 0);
	glVertex3fv(v4);

	glTexCoord2f(0, 0);
	glVertex3fv(v5);

	//face 5
	//calculate normal
	auto edg71 = FromGLfloat(v5) - FromGLfloat(top);
	auto edg72 = FromGLfloat(v1) - FromGLfloat(top);
	auto norm7 = (edg71 % edg72);
	norm7.Normalise();
	glNormal3f(norm7.x, norm7.y, norm7.z);

	glTexCoord2f(0.5, 0.5);
	glVertex3fv(top);

	glTexCoord2f(1, 0);
	glVertex3fv(v5);

	glTexCoord2f(0, 0);
	glVertex3fv(v1);

	glEnd();
	glBegin(GL_POLYGON);

	//face 6
	//calculate normal
	auto edg81 = FromGLfloat(v3) - FromGLfloat(v5);
	auto edg82 = FromGLfloat(v1) - FromGLfloat(v5);
	auto norm8 = (edg81 % edg82);
	norm8.Normalise();
	glNormal3f(norm8.x, norm8.y, norm8.z);

	glTexCoord2f(0, 0);
	glVertex3fv(v5);

	glTexCoord2f(0, 0);
	glVertex3fv(v4);

	glTexCoord2f(0, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 1);
	glVertex3fv(v2);

	glTexCoord2f(1, 0);
	glVertex3fv(v1);

	glEnd();

	glPopMatrix();
	glDisable(GL_CULL_FACE);
}

void WorldRenderSystem::RenderPrism(TransformComponent* transform)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, prism_texture);

	glPushMatrix();

	////set material
	GLfloat ambientMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

	glTRS(transform);

	GLfloat top1[3] = { 0, 1, 1 };
	GLfloat top2[3] = { 0, 1, -1 };

	GLfloat v1[3] = { -1, 0,  -1 };
	GLfloat v2[3] = { -1, 0,  1 };
	GLfloat v3[3] = {  1, 0,  1 };
	GLfloat v4[3] = {  1, 0, -1 };

	glBegin(GL_QUADS);

	//quad 1
	//calculate normal
	auto edgq21 = FromGLfloat(top2) - FromGLfloat(top1);
	auto edgq22 = FromGLfloat(v2) - FromGLfloat(top1);
	auto normq3 = (edgq21 % edgq22);
	normq3.Normalise();
	glNormal3f(normq3.x, normq3.y, normq3.z);

	glTexCoord2f(0, 1);
	glVertex3fv(top1);

	glTexCoord2f(1, 1);
	glVertex3fv(top2);

	glTexCoord2f(1, 0);
	glVertex3fv(v1);

	glTexCoord2f(0, 0);
	glVertex3fv(v2);


	////quad 2
	////calculate normal
	auto edgq11 = FromGLfloat(v2) - FromGLfloat(v1);
	auto edgq12 = FromGLfloat(v3) - FromGLfloat(v1);
	auto normq1 = (edgq11 % edgq12);
	normq1.Normalise();
	glNormal3f(normq1.x, normq1.y, normq1.z);

	glTexCoord2f(0, 1);
	glVertex3fv(v4);

	glTexCoord2f(1, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 0);
	glVertex3fv(v2);

	glTexCoord2f(0, 0);
	glVertex3fv(v1);


	//quad 3
	//calculate normal
	auto edgq1 = FromGLfloat(v3) - FromGLfloat(top1);
	auto edgq2 = FromGLfloat(v4) - FromGLfloat(top1);
	auto normq2 = (edgq1 % edgq2);
	normq2.Normalise();
	glNormal3f(normq2.x, normq2.y, normq2.z);

	glTexCoord2f(0, 1);
	glVertex3fv(v3);

	glTexCoord2f(1, 1);
	glVertex3fv(v4);

	glTexCoord2f(1, 0);
	glVertex3fv(top2);

	glTexCoord2f(0, 0);
	glVertex3fv(top1);

	glEnd();

	glBegin(GL_TRIANGLES);

	//face 1
	//calculate normal
	auto edg1 = FromGLfloat(v2) - FromGLfloat(top1);
	auto edg2 = FromGLfloat(v3) - FromGLfloat(top1);
	auto norm = (edg1 % edg2);
	norm.Normalise();
	glNormal3f(norm.x, norm.y, norm.z);

	glTexCoord2f(1, 0.5);
	glVertex3fv(top1);

	glTexCoord2f(0, 0);
	glVertex3fv(v2);

	glTexCoord2f(0, 1);
	glVertex3fv(v3);


	//face2
	//calculate normal
	auto edg21 = FromGLfloat(v4) - FromGLfloat(top2);
	auto edg22 = FromGLfloat(v1) - FromGLfloat(top2);
	auto norm2 = (edg21 % edg22);
	norm2.Normalise();
	glNormal3f(norm2.x, norm2.y, norm2.z);

	glTexCoord2f(1, 0.5);
	glVertex3fv(top2);

	glTexCoord2f(0, 0);
	glVertex3fv(v4);

	glTexCoord2f(0, 1);
	glVertex3fv(v1);

	glEnd();

	glPopMatrix();
	glDisable(GL_CULL_FACE);
}

void WorldRenderSystem::RenderBox(TransformComponent* transform)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glTRS(transform);

	glColor3f(0, 1, 0);
	glutWireCube(1);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void WorldRenderSystem::RenderWall(TransformComponent* transform)
{
	float width = transform->Scale.x / 2;
	float heigth = transform->Scale.y / 2;
	float depth = transform->Scale.z / 2;

	//calculate texture height/depth proportionate to scaling
	float textY = heigth / width;
	float textZ = depth / heigth;

	GLfloat v1[3] = { -width, -heigth, -depth };
	GLfloat v2[3] = { -width, heigth, -depth };
	GLfloat v3[3] = { width, heigth, -depth };
	GLfloat v4[3] = { width, -heigth, -depth };
	GLfloat v21[3] = { -width, -heigth, depth };
	GLfloat v22[3] = { -width, heigth, depth };
	GLfloat v23[3] = { width, heigth, depth };
	GLfloat v24[3] = { width, -heigth, depth };

	////set material
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularMaterial[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 70.0f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glPushMatrix();

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTranslatef(transform->Position.x, transform->Position.y, transform->Position.z);

	float theta = atan2(transform->Direction.z, transform->Direction.x) * 180.0f / (float)M_PI;
	glRotatef(-theta, 0, 1, 0);

	glBegin(GL_QUADS);

	//calculate normal
	auto edg1 = FromGLfloat(v2) - FromGLfloat(v1);
	auto edg2 = FromGLfloat(v3) - FromGLfloat(v1);
	auto norm1 = (edg1 % edg2);
	norm1.Normalise();
	glNormal3f(norm1.x, norm1.y, norm1.z);

	glTexCoord2f(1, 1 - textY);
	glVertex3fv(v1);

	glTexCoord2f(1, 1);
	glVertex3fv(v2);

	glTexCoord2f(0, 1);
	glVertex3fv(v3);

	glTexCoord2f(0, 1 - textY);
	glVertex3fv(v4);

	//calculate normal
	auto edg11 = FromGLfloat(v23) - FromGLfloat(v24);
	auto edg12 = FromGLfloat(v22) - FromGLfloat(v24);
	auto norm2 = (edg11 % edg12);
	norm2.Normalise();
	glNormal3f(norm2.x, norm2.y, norm2.z);

	glTexCoord2f(0, 1 - textY);
	glVertex3fv(v24);

	glTexCoord2f(0, 1);
	glVertex3fv(v23);

	glTexCoord2f(1, 1);
	glVertex3fv(v22);

	glTexCoord2f(1, 1 - textY);
	glVertex3fv(v21);

	//calculate normal
	auto edg21 = FromGLfloat(v22) - FromGLfloat(v1);
	auto edg22 = FromGLfloat(v2) - FromGLfloat(v1);
	auto norm3 = (edg21 % edg22);
	norm3.Normalise();
	glNormal3f(norm3.x, norm3.y, norm3.z);

	glTexCoord2f(0, 0);
	glVertex3fv(v21);

	glTexCoord2f(0, 1);
	glVertex3fv(v22);

	glTexCoord2f(textZ, 1);
	glVertex3fv(v2);

	glTexCoord2f(textZ, 0);
	glVertex3fv(v1);

	//calculate normal
	auto edg31 = FromGLfloat(v3) - FromGLfloat(v24);
	auto edg32 = FromGLfloat(v23) - FromGLfloat(v24);
	auto norm4 = (edg31 % edg32);
	norm4.Normalise();
	glNormal3f(norm4.x, norm4.y, norm4.z);

	glTexCoord2f(textZ, 0);
	glVertex3fv(v4);

	glTexCoord2f(textZ, 1);
	glVertex3fv(v3);

	glTexCoord2f(0, 1);
	glVertex3fv(v23);

	glTexCoord2f(0, 0);
	glVertex3fv(v24);

	//calculate normal
	auto edg41 = FromGLfloat(v22) - FromGLfloat(v2);
	auto edg42 = FromGLfloat(v23) - FromGLfloat(v2);
	auto norm5 = (edg41 % edg42);
	norm5.Normalise();
	glNormal3f(norm5.x, norm5.y, norm5.z);

	glTexCoord2f(0, 1);
	glVertex3fv(v2);

	glTexCoord2f(0, 1 - textZ);
	glVertex3fv(v22);

	glTexCoord2f(1, 1 - textZ);
	glVertex3fv(v23);

	glTexCoord2f(1, 1);
	glVertex3fv(v2);

	glEnd();

	glPopMatrix();

	glDisable(GL_CULL_FACE);
}

void WorldRenderSystem::RenderLighthouseLights(TransformComponent* transform)
{
	////set material
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 0.5f };
	GLfloat diffuseMaterial[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	GLfloat specularMaterial[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);
	glPushMatrix();

	auto &position = transform->Position;
	auto &scale = transform->Scale;
	float theta = atan2(transform->Direction.z, transform->Direction.x) * 180.0f / (float)M_PI;

	glTranslatef(position.x + 50, position.y, position.z);
	glRotatef(-theta, 0, 1, 0);
	glRotatef(180, 1, 0, 0);
	glScalef(scale.x, scale.y, scale.z);

	glutSolidCone(10, 50, 20, 20);

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_LIGHTING);
}

void WorldRenderSystem::RenderGrenade(TransformComponent* transform)
{
	auto &position = transform->Position;
	auto &scale = transform->Scale;
	auto &rotation = transform->Rotation;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);

	if (rotation.Length() > 0)
		glRotatef(rotation.Length(), rotation.x, rotation.y, rotation.z);

	glScalef(scale.x, scale.y, scale.z);

	glColor3f(1, 0, 0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);


}

void WorldRenderSystem::glTRS(TransformComponent * transform)
{
	auto &position = transform->Position;
	auto &scale = transform->Scale;
	float theta = atan2(transform->Direction.z, transform->Direction.x) * 180.0f / (float)M_PI;

	glTranslatef(position.x, position.y, position.z);
	glRotatef(-theta, 0, 1, 0);
	glScalef(scale.x, scale.y, scale.z);
}

CVector3f WorldRenderSystem::FromGLfloat(const GLfloat* vector)
{
	return CVector3f(vector[0], vector[1], vector[2]);
}

#pragma region MeshLoading

Mesh* WorldRenderSystem::LoadMesh(const string& filename)
{
	Mesh* mesh = new Mesh;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene) {
		InitFromScene(mesh, pScene, filename);
		mesh->IsValid = true;
	}
	else {
		mesh->IsValid = false;
		MessageBox(NULL, Importer.GetErrorString(), "Error loading mesh model", MB_ICONHAND);
	}
	return mesh;
}

bool WorldRenderSystem::InitFromScene(Mesh* mesh, const aiScene* pScene, const std::string& Filename)
{
	mesh->MeshEntries.resize(pScene->mNumMeshes);
	mesh->Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < mesh->MeshEntries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(mesh, i, paiMesh);
	}

	return InitMaterials(mesh, pScene, Filename);
}

void WorldRenderSystem::InitMesh(Mesh* mesh, unsigned int Index, const aiMesh* paiMesh)
{
	mesh->MeshEntries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		CVertex v(pPos->x, pPos->y, pPos->z, pNormal->x, pNormal->y, pNormal->z, pTexCoord->x, 1.0f - pTexCoord->y);

		mesh->MeshEntries[Index].Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		mesh->MeshEntries[Index].Indices.push_back(Face.mIndices[0]);
		mesh->MeshEntries[Index].Indices.push_back(Face.mIndices[1]);
		mesh->MeshEntries[Index].Indices.push_back(Face.mIndices[2]);
	}

	mesh->MeshEntries[Index].Init(mesh->MeshEntries[Index].Vertices, mesh->MeshEntries[Index].Indices);
}

bool WorldRenderSystem::InitMaterials(Mesh* mesh, const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("\\");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "\\";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		mesh->Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "\\" + Path.data;

				mesh->Textures[i] = new CTexture();
				if (!mesh->Textures[i]->Load(FullPath, true)) {
					MessageBox(NULL, FullPath.c_str(), "Error loading mesh texture", MB_ICONHAND);
					delete mesh->Textures[i];
					mesh->Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}


		// Load a single colour texture matching the diffuse colour if no texture added
		if (!mesh->Textures[i]) {

			aiColor3D color(0.0f, 0.0f, 0.0f);
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);

			mesh->Textures[i] = new CTexture();
			BYTE data[3];
			data[0] = (BYTE)(color[2] * 255);
			data[1] = (BYTE)(color[1] * 255);
			data[2] = (BYTE)(color[0] * 255);
			mesh->Textures[i]->CreateFromRGB(data[2], data[1], data[0]);

		}
	}

	return Ret;
}

#pragma endregion
