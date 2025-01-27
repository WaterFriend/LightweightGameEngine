// Includes
//=========

#include "cCamera.h"

#include <Engine/UserInput/UserInput.h>


void eae6320::Camera::cCamera::Initialize(
	float i_posX, float i_posY, float i_posZ,
	float i_verticalFieldOfView_inRadians, 
	float i_z_nearPlane, 
	float i_z_farPlane)
{
	m_rigidBody.position = { i_posX, i_posY, i_posZ };
	m_verticalFieldOfView_inRadians = i_verticalFieldOfView_inRadians;
	m_z_nearPlane = i_z_nearPlane;
	m_z_farPlane = i_z_farPlane;
}


void eae6320::Camera::cCamera::UpdateBasedOnInput()
{
	// Strafe left and right
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) || 
		UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		Math::sVector strafeDirection = Math::Cross(m_rigidBody.GetForwardDirection(), Math::sVector(0, 1, 0));

		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
			m_rigidBody.velocity = strafeDirection * -2.0f;
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
			m_rigidBody.velocity = strafeDirection * 2.0f;
	}
	// Move up and down
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) ||
			 UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
			m_rigidBody.velocity.y = -2.0f;
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
			m_rigidBody.velocity.y = 2.0f;
	}
	// Move forward and backward
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Home) ||
			 UserInput::IsKeyPressed(UserInput::KeyCodes::End))
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Home))
			m_rigidBody.velocity = m_rigidBody.GetForwardDirection() * 4.0f;
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::End))
			m_rigidBody.velocity = m_rigidBody.GetForwardDirection() * -4.0f;
	}
	else
		m_rigidBody.velocity = Math::sVector();


	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Delete))
	{
		m_rigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
		m_rigidBody.angularSpeed = 2.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::PageDown))
	{
		m_rigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
		m_rigidBody.angularSpeed = -2.0f;
	}
	else
	{
		m_rigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 0.0f);
		m_rigidBody.angularSpeed = 0.0f;
	}
}


eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::CreateWorldToCameraMatrix(const float i_secondCountToExtrapolate)
{
	Math::sVector predictPos = m_rigidBody.PredictFuturePosition(i_secondCountToExtrapolate);

	return Math::cMatrix_transformation::CreateWorldToCameraTransform(m_rigidBody.orientation, predictPos);
}


eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::CreateCameraToProjectedMatrix(float windowWidth, float windowHeight)
{
	return Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
		m_verticalFieldOfView_inRadians,
		windowWidth / windowHeight,
		m_z_nearPlane,
		m_z_farPlane);
}

