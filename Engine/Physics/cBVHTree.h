#pragma once

// Includes
//=========

#include <Engine/Graphics/cLine.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

#include <list>
#include <memory>
#include <vector>


// BVH Tree Default Setting
//=============

namespace eae6320
{
namespace Physics
{

#define DEFAULT_BVH_MARGIN 0.05f

}// Namespace Physics
}// Namespace eae6320


// BVH Tree Node
//=============

namespace eae6320
{
namespace Physics
{

	struct sBVHNode
	{

		// Data
		//=========================

		sBVHNode* parent;
		sBVHNode* children[2];

		// fatAABB whose min/max extent contains the world extents of all children of this node
		// min/max extent uses world coordinate directly. World position should be ZERO
		cAABBCollider fatAABB;

		// link to the actual gameobject's collider
		cCollider* collider;

		// TODO: need to optmize this
		bool childrenCrossed;


		// Interface
		//=========================

		sBVHNode();

		bool IsLeaf() const;

		/* Make this ndoe a branch node */
		void SetBranch(sBVHNode* i_node0, sBVHNode* i_node1);

		/* Make this node a leaf */
		void SetLeaf(cCollider* i_collider);

		/* Update the fat AABB */
		void UpdateFatAABB(float i_margin);

		sBVHNode* GetSibling() const;
	};

}// Namespace Physics
}// Namespace eae6320


// BVH Tree Class Declaration
//=============

namespace eae6320
{
namespace Physics
{

	class cBVHTree
	{
		// Interface
		//=========================

	public:
		cBVHTree() : m_root(nullptr), m_margin(DEFAULT_BVH_MARGIN) {}
		cBVHTree(float i_margin) : m_root(nullptr), m_margin(i_margin) { }

		sBVHNode* Search(cCollider* i_collider);
		void Add(cCollider* i_collider);
		void Remove(cCollider* i_collider);
		void Update();

		std::list<std::pair<cCollider*, cCollider*>>& ComputePairs();
		std::vector<cCollider*> Query(cCollider* i_collider) const;

		void InitialzieRenderData();
		
		std::list<std::pair<std::weak_ptr<Graphics::cLine>, Math::cMatrix_transformation>> GetRenderData();

		// TODO
		//virtual cCollider* Pick(const Math::sVector& i_point) const;
		//virtual RayCastResult RayCast(const Ray3& ray) const;



		// Implementation
		//=========================

	private:

		void InsertNode(sBVHNode* i_node, sBVHNode** i_parent);
		void RemoveNode(sBVHNode* i_node);

		void UpdateInvalidNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes);
		void ComputePairsHelper(sBVHNode* i_node0, sBVHNode* i_node1);
		void ClearChildrenCrossFlagHelper(sBVHNode* i_node);
		void CrossChildren(sBVHNode* i_node);
		void RenderInitializeHelper(std::shared_ptr<Graphics::cLine>& io_AABBLine);
		void RenderUpdateHelper();

		// Data
		//=========================

	private:

		float m_margin;
		sBVHNode* m_root;
		std::list<std::pair<cCollider*, cCollider*>> m_pairs;
		std::vector<sBVHNode*> m_invalidNodes;
		std::list<std::pair<std::shared_ptr<Graphics::cLine>, Math::cMatrix_transformation>> m_renderData;
	};

}// Namespace Physics
}// Namespace eae6320
