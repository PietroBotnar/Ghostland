#include "CameraSystem.h"

#include <Windows.h>

#include "..\include\glew.h"									// Header File For The Glaux Library
#include "..\include\gl.h"											// Header File For The OpenGL32 Library
#include "..\include\glu.h"									// Header File For The GLu32 Library

#include <math.h>

#include "..\GameWindow.h"
#include "..\ECS\World.h"
#include "..\ECS\IEvent.h"
#include "..\Components\TransformComponent.h"

void CameraSystem::InitialiseSystem()
{
	m_position = CVector3f(0.0f, 4.0f, 0.0f);
	m_viewPoint = CVector3f(0.0f, 1.0f, 5.0f);
	m_upVector = CVector3f(0.0f, 1.0f, 0.0f);
	m_speed = 5.0f;
	m_totalVerticalRotation = 0.0f;

	_world->SetCameraPostion(&m_position);
	_world->SetCameraViewpoint(&m_viewPoint);

	auto player = _world->EntityManager->GetTaggedEntity(PlayerTag);
	PlayerTransform = player->GetTransform();
}

void CameraSystem::Update(float dt)
{
	Look();

	//follow player
	auto playerDirection = PlayerTransform->Direction;
	auto playerPosition = PlayerTransform->Position;
	
	float A = 10.0f;
	float B = 15.0f;
	float C = 20.0f;

	CVector3f p = playerPosition + A * CVector3f(0, 1, 0) - B * playerDirection;
	CVector3f v = playerPosition + C * playerDirection;

	Set(p, v, CVector3f(0, 1, 0), 5.0f);
}

void CameraSystem::Set(const CVector3f &position, const CVector3f &view, const CVector3f &up_vector, const float &speed)
{
	m_position = position;
	m_viewPoint = view;
	m_upVector = up_vector;
	m_speed = speed;

	Look();
}

void CameraSystem::Strafe(float speed)
{
	m_position.x += m_strafeVector.x * speed;
	m_position.z += m_strafeVector.z * speed;

	m_viewPoint.x += m_strafeVector.x * speed;
	m_viewPoint.z += m_strafeVector.z * speed;
}

void CameraSystem::Advance(float speed)
{
	CVector3f view = m_viewPoint - m_position;
	view.Normalise();

	m_position += view * speed;
	m_viewPoint += view * speed;
}

void CameraSystem::MoveUp(float speed)
{
	m_position += m_upVector * speed;
	m_viewPoint += m_upVector * speed;
}

void CameraSystem::Look()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		m_position.x, m_position.y, m_position.z,
		m_viewPoint.x, m_viewPoint.y, m_viewPoint.z,
		m_upVector.x, m_upVector.y, m_upVector.z);

	m_strafeVector = (m_viewPoint - m_position) % m_upVector;
	m_strafeVector.Normalise();
}

void CameraSystem::RotateView(float angle, CVector3f &point)
{
	CVector3f view = m_viewPoint - m_position;
	CVector3f rotatedView = view.RotateAboutAxis(angle, point);
	m_viewPoint = m_position + rotatedView;
}

void CameraSystem::SetViewByMouse()
{
	int middle_x = GameWindow::SCREEN_WIDTH >> 1;
	int middle_y = GameWindow::SCREEN_HEIGHT >> 1;


	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	SetCursorPos(middle_x, middle_y);

	float dx = (float)(middle_x - mouse.x) / 50.0f;
	float dy = (float)(middle_y - mouse.y) / 50.0f;


	m_totalVerticalRotation += dy;

	// Clip the rotation rotation about the strafe vector to [-MAX_STRAFE_ROTATION, MAX_STRAFE_ROTATION]
	if (m_totalVerticalRotation > MAX_VERTICAL_ROTATION) {
		m_totalVerticalRotation = MAX_VERTICAL_ROTATION;
	}
	else if (m_totalVerticalRotation < -MAX_VERTICAL_ROTATION) {
		m_totalVerticalRotation = -MAX_VERTICAL_ROTATION;
	}
	else {
		RotateView(dy, m_strafeVector);
	}

	RotateView(dx, CVector3f(0, 1, 0));
}

void CameraSystem::OnEvent(IEvent* _event)
{
	//listen to events here
}
