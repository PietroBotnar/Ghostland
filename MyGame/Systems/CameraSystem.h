#pragma once
#include "..\Data\Vector3f.h"
#include "..\ECS\ISystem.h"
#include "..\ECS\Entity.h"
#include "..\Components\TransformComponent.h"


#define MAX_VERTICAL_ROTATION 85 

//Controls the camera in the game
class CameraSystem : public ISystem
{
public:
	explicit CameraSystem(){};
	~CameraSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;

	// Set a specific camera
	void Set(const CVector3f &position, const CVector3f &view, const CVector3f &up_vector, const float &speed);

	// Return the camera position (in world coordinates)
	CVector3f GetPosition() const {return m_position; };

	// Return the point where the camera is looking (in world coordinates)
	CVector3f GetViewPoint() const {return m_viewPoint; };		

	// Return the camera up vector (in world coordinates)
	CVector3f GetUpVector() const { return m_upVector; };

	// Return the camera strafe vector (in world coordinates)
	CVector3f GetStrafeVector() const {return m_strafeVector; };	

	void Strafe(float speed);	// Strafing moves the camera left / right
	void Advance(float speed);	// Advance moves the camera forward / backward
	void MoveUp(float speed);	// MoveUp moves the camera up / down

	// Calls gluLookAt() to put the viewing matrix on the modelview matrix stack
	void Look();				

    // Rotate the camera viewpoint around a vector
	void RotateView(float angle, CVector3f &point);

	// Get the camera rotation using the mouse
	void SetViewByMouse();

	virtual void OnEvent(IEvent* _event) override;

private:
	CVector3f m_position;
	CVector3f m_viewPoint;
	CVector3f m_upVector;
	CVector3f m_strafeVector;

	float m_speed;
	float m_totalVerticalRotation;

	TransformComponent* PlayerTransform;
};

