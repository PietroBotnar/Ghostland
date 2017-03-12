#pragma once
#include "..\ECS\IComponent.h"
#include "..\Data\Vector3f.h"

class RigidBodyComponent : public IComponent
{
public:
	RigidBodyComponent(float radius, float mass, float rotationInertia, float restitution, float contactTime)
	{
		m_radius					= radius; // in meters
		m_mass						= mass; // in kg
		m_rotationalInertia			= rotationInertia * m_mass * m_radius * m_radius; // in kg m^2
		m_coefficientOfRestitution	= restitution; // percentage
		m_contactTime				= contactTime; // in seconds

		m_velocity					= CVector3f();
		m_acceleration				= CVector3f();
		m_instantaneousAcceleration = CVector3f();
		m_angle						= CVector3f();
		m_angularVelocity			= CVector3f();
		m_angularAcceleration		= CVector3f();
		m_instantaneousAngularAcceleration = CVector3f();

	};
	~RigidBodyComponent(){};

	void AddForce(CVector3f force) {
		m_instantaneousAcceleration = force / m_mass;
	}

	void AddTorque(CVector3f torque) {
		m_instantaneousAngularAcceleration = torque / m_rotationalInertia;
	}

	// Linear physical quantities
	CVector3f m_velocity;
	CVector3f m_acceleration;
	CVector3f m_instantaneousAcceleration;
	float m_radius;

	// Rotational physical quantities
	CVector3f m_angle;
	CVector3f m_angularVelocity;
	CVector3f m_angularAcceleration;
	CVector3f m_instantaneousAngularAcceleration;

	// Other physical quantities
	float m_mass; // in kg
	float m_rotationalInertia; // in kg m^2
	float m_contactTime; // in seconds
	float m_coefficientOfRestitution; // as a percentage
};

