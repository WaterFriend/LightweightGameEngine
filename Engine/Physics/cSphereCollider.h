#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

#include <vector>

namespace eae6320
{
namespace Physics
{

	class cSphereCollider : public cCollider
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		cSphereCollider() : cCollider(eColliderType::Sphere) { };
		cSphereCollider(const Math::sVector& i_center, float i_radius)
			: cCollider(eColliderType::Sphere), m_center(i_center), m_radius(i_radius) { }
		cSphereCollider(float i_x, float i_y, float i_z, float i_radius)
			: cCollider(eColliderType::Sphere), m_center(Math::sVector(i_x, i_y, i_z)), m_radius(i_radius) { }

		~cSphereCollider() = default;


		// Property Getters
		//--------------------------

		Math::sVector GetMinExtent_world() const final;

		Math::sVector GetMaxExtent_world() const final;

		Math::sVector GetMinExtent_local() const final;

		Math::sVector GetMaxExtent_local() const final;

		Math::sVector GetCentroid_world() const final;

		Math::sVector GetCentroid_local() const final;

		Math::sVector GetWorldPosition() const final;

		float GetRadius() const;

		// Overlap Detection
		//--------------------------

		bool IsOverlaps(const cSphereCollider& i_other);

		bool IsOverlaps(const cAABBCollider& i_other);

		// Render / Debug
		//--------------------------

		void GenerateRenderData(
			uint32_t& o_vertexCount, std::vector<Math::sVector>& o_vertexData,
			uint32_t& o_indexCount, std::vector<uint16_t>& o_indexData) final;


		// Data
		//=====================
	
	private:

		float m_radius = 0.0f;
		Math::sVector m_center;
	};


}// Namespace Physics
}// Namespace eae6320