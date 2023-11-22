// Includes
//=========

#include <iostream>

#include "cScrollShooterGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>


// TODO: Tempory code for collider testing
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/Collision.h>
#include <vector>
#include <iostream>

using namespace eae6320;

// Inherited Implementation
//=========================

// Run
//----

void ScrollShooterGame::cScrollShooterGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Escape))
	{
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
}


void ScrollShooterGame::cScrollShooterGame::UpdateSimulationBasedOnInput()
{
	m_camera.UpdateBasedOnInput();

	m_player.UpdateBasedOnInput();

}


void ScrollShooterGame::cScrollShooterGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	m_renderObject_triangle.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_renderObject_Ganyu.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_renderObject_Keqing.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);


	// TODO: Temporary code for player update
	m_player.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	for (cBullet* bullet : m_bulletList)
	{
		bullet->UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	}


	// TODO: Temporary code for collider debug

	Physics::Collision::Update_CollisionDetection();

	Physics::Collision::Update_CollisionResolution();

}


void ScrollShooterGame::cScrollShooterGame::SubmitDataToBeRendered(
	const float i_elapsedSecondCount_systemTime,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Submit camera data
	{
		// aspectRatio = width / height
		uint16_t width, height;
		GetCurrentResolution(width, height);

		Graphics::SubmitCameraMatrices(
			m_camera.CreateWorldToCameraMatrix(i_elapsedSecondCount_sinceLastSimulationUpdate),
			m_camera.CreateCameraToProjectedMatrix(static_cast<float>(width), static_cast<float>(height)));
	}

	// Submit background color
	{
		Graphics::SubmitBackgroundColor(0.5f, 0.5f, 0.5f);
	}


	// Submit normal render data
	{
		size_t renderObjectNum = m_renderObjectList.size();
		size_t arraySize = m_renderObjectList.size() + m_bulletList.size();

		Graphics::ConstantBufferFormats::sNormalRender* normalRenderDataArray = new Graphics::ConstantBufferFormats::sNormalRender[arraySize];

		// Render data of render objects 
		for (size_t i = 0; i < m_renderObjectList.size(); i++)
		{

			normalRenderDataArray[i].Initialize(
				m_renderObjectList[i]->GetMesh(), m_renderObjectList[i]->GetEffect(),
				m_renderObjectList[i]->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		//TODO: Render data for bullets
		for (size_t i = renderObjectNum; i < arraySize; i++)
		{
			normalRenderDataArray[i].Initialize(
				m_bulletList[i - renderObjectNum]->GetMesh(), m_bulletList[i - renderObjectNum]->GetEffect(),
				m_bulletList[i - renderObjectNum]->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}


		Graphics::SubmitNormalRenderData(normalRenderDataArray, static_cast<uint32_t>(arraySize));

		// clean up 
		for (size_t i = 0; i < arraySize; i++)
		{
			normalRenderDataArray[i].CleanUp();
		}

		delete[] normalRenderDataArray;
	}


	// Submit debug render data (for colliders)
	{
		auto BVHRenderData = std::vector<std::pair<eae6320::Graphics::cLine*, eae6320::Math::cMatrix_transformation>>();
		BVHRenderData = Physics::Collision::GetBVHRenderData();

		size_t staticSize = m_colliderObjectList.size();
		size_t arraySize = BVHRenderData.size() + staticSize;

		Graphics::ConstantBufferFormats::sDebugRender* debugDataArray = new Graphics::ConstantBufferFormats::sDebugRender[arraySize];

		// Render data of hard-coded collider
		for (size_t i = 0; i < m_colliderObjectList.size(); i++)
		{
			auto collider = m_colliderObjectList[i];
			debugDataArray[i].Initialize(collider->GetColliderLine(), collider->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		// Render data of BVH tree
		for (size_t i = staticSize; i < arraySize; i++)
		{
			debugDataArray[i].Initialize(BVHRenderData[i - staticSize].first, BVHRenderData[i - staticSize].second);
		}

		Graphics::SubmitDebugRenderData(debugDataArray, static_cast<uint32_t>(arraySize));

		// Clean up
		for (size_t i = 0; i < arraySize; i++)
		{
			debugDataArray[i].CleanUp();
		}

		delete[] debugDataArray;
	}
}


// Initialize / Clean Up
//----------------------

eae6320::cResult ScrollShooterGame::cScrollShooterGame::Initialize()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Initialization Successfull!! \n");


	InitializeCamera();

	InitializeMeshData();

	InitializeGameObject();

	// TODO: temporary code for initialize collision system
	InitializeCollisionSystem();


	return Results::Success;
}


eae6320::cResult ScrollShooterGame::cScrollShooterGame::CleanUp()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Clan Up Successfull!!\n");

	CleanUpGameObject();

	return Results::Success;
}


void ScrollShooterGame::cScrollShooterGame::InitializeMeshData()
{
	Graphics::cEffect::Create(m_effect_animate, m_vertexShaderPath, m_fragmentShaderPath_animate);

	Graphics::cEffect::Create(m_effect_standard, m_vertexShaderPath, m_fragmentShaderPath_standard);
}


void ScrollShooterGame::cScrollShooterGame::InitializeCamera()
{
	m_camera.Initialize(
		0.0f, 0.0f, 5.0f,
		Math::ConvertDegreesToRadians(45.0f),
		0.1f,
		100.0f);
}


void ScrollShooterGame::cScrollShooterGame::InitializeGameObject()
{
	// Game object initialization
	{
		// Tirangle
		m_renderObject_triangle.InitializeMesh(m_triangleMeshPath);
		m_renderObject_triangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_triangle.GetRigidBody().angularSpeed = 1.0f;
		m_renderObject_triangle.GetRigidBody().angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 1.0f);

		// Rectangle
		m_renderObject_rectangle.InitializeMesh(m_rectangleMeshPath);
		m_renderObject_rectangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_rectangle.GetRigidBody().orientation = Math::cQuaternion(0.5f, Math::sVector(1, 0, 1));
		m_renderObject_rectangle.GetRigidBody().position = Math::sVector(+0.0f, -1.0f, +0.5f);

		// Plane
		m_renderObject_plane.InitializeMesh(m_planeMeshPath);
		m_renderObject_plane.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_plane.GetRigidBody().position = Math::sVector(+6.0f, -5.0f, -3.0f);

		// Cube
		m_renderObject_cube.InitializeMesh(m_cubeMeshPath);
		m_renderObject_cube.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);
		m_renderObject_cube.GetRigidBody().position = Math::sVector(-2.0f, -2.0f, -3.0f);

		// Keqing
		m_renderObject_Keqing.InitializeMesh(m_keqingMeshPath);
		m_renderObject_Keqing.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Keqing.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);

		// Keqing - skin
		m_renderObject_Keqing_skin.InitializeMesh(m_keqing_SkinMeshPath);
		m_renderObject_Keqing_skin.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Keqing_skin.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);

		// Ganyu
		m_renderObject_Ganyu.InitializeMesh(m_ganyuMeshPath);
		m_renderObject_Ganyu.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Ganyu.GetRigidBody().position = Math::sVector(5.0f, -10.0f, -20.0f);


		m_temp.InitializeMesh(m_cubeMeshPath);
		m_temp.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);
		


		//m_renderObjectList.push_back(&m_renderObject_triangle);
		//m_renderObjectList.push_back(&m_renderObject_rectangle);
		//m_renderObjectList.push_back(&m_renderObject_plane);
		//m_renderObjectList.push_back(&m_renderObject_cube);
		m_renderObjectList.push_back(&m_renderObject_Keqing);
		//m_renderObjectList.push_back(&m_renderObject_Keqing_skin);
		m_renderObjectList.push_back(&m_renderObject_Ganyu);
	}


	// TODO: temporary code for initialize colldier object
	{

	}

	// TODO: temporary code for player object
	{
		m_player.InitializeMesh(m_cubeMeshPath);
		m_player.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_player.GetRigidBody().position = Math::sVector(0.0f, 0.0f, -5.0f);

		m_player.bulletCreation = [this]() -> void 
			{
				m_temp.CleanUp();
				

				cBullet* newBullet = new cBullet();
				newBullet->Initialize(m_player.GetRigidBody().position, Math::sVector(0, 1, 0));
				newBullet->InitializeMesh(m_triangleMeshPath);
				newBullet->InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
				m_bulletList.push_back(newBullet);
			};



		m_renderObjectList.push_back(&m_player);
	}
}


void ScrollShooterGame::cScrollShooterGame::CleanUpGameObject()
{
	m_camera.CleanUp();

	m_effect_animate->DecrementReferenceCount();
	m_effect_standard->DecrementReferenceCount();


	// clean up render object
	m_renderObject_triangle.CleanUp();
	m_renderObject_rectangle.CleanUp();
	m_renderObject_plane.CleanUp();
	m_renderObject_cube.CleanUp();
	//m_renderObject_Keqing.CleanUp();
	m_renderObject_Keqing_skin.CleanUp();
	//m_renderObject_Ganyu.CleanUp();
	for (cGameObject* renderObject : m_renderObjectList)
	{
		renderObject->CleanUp();
	}


	// TODO: temporary code for clean up colldier object
	for (cPhysicDebugObject* colliderObject : m_colliderObjectList)
	{
		colliderObject->CleanUp();
	}

	// TODO: temporary code for clean up colldier object
	m_player.CleanUp();
	for (cBullet* bulletObjet : m_bulletList)
	{
		bulletObjet->CleanUp();
	}
}



void ScrollShooterGame::cScrollShooterGame::InitializeCollisionSystem()
{
	std::vector<Physics::cCollider*> colliderList;

	for (cPhysicDebugObject* colliderObject : m_colliderObjectList)
	{
		colliderList.push_back(colliderObject->GetCollider());
	}

	Physics::Collision::Initialize(colliderList, Physics::Collision::BroadPhase_BVH | Physics::Collision::NarrowPhase_Overlaps);
}


 