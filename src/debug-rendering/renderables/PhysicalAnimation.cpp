#ifndef DEVTOOLS_TARGET_SDK_wars
#include "PhysicalAnimation.h"
#include <ui/common/overlays/Tag.h>

namespace devtools::debug_rendering::renderables {
	struct btCollisionShape {
		int64_t vftable;
		csl::math::Matrix34 worldTransform;
	};

	struct Node {
		char padding0[16];
		csl::math::Vector3 position;
		char padding1[224];
	};

	struct alignas(16) Link {
		char padding0[32];
		Node* nodes[2];
		char padding1[32];
	};

	struct alignas(16) btSoftBody {
		char padding0[952];
		int nodeCount;
		int nodeCapacity;
		Node* nodes;
		char padding1[48];
		int linkCount;
		int linkCapacity;
		Link* links;
	};

	csl::math::Matrix34 convertBulletToDirectX(const csl::math::Matrix34& bulletStyle) {
		csl::math::Matrix34 directXStyle;
		directXStyle.linear() = bulletStyle.linear().transpose();
		directXStyle.translation() = bulletStyle.translation();
		return directXStyle;
	}

	void PhysicalAnimation::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		//auto* gameManager = hh::game::GameManager::GetInstance();
		//if (!gameManager)
		//	return;

		//for (auto* gameObject : gameManager->objects) {
		//	for (auto* goc : gameObject->components) {
		//		if (goc->pStaticClass == hh::pba::GOCPhysicalAnimationBullet::GetClass()) {
		//			auto* physGoc = static_cast<hh::pba::GOCPhysicalAnimationBullet*>(goc);
		//			for (auto i = 0; i < physGoc->rigidBodies.size(); i++) {
		//				const csl::ut::Color8 rigidBodyColor = { 255, 0, 255, 255 };
		//				auto* rigidBody = reinterpret_cast<btCollisionShape*>(physGoc->rigidBodies[i*2]);
		//				csl::math::Matrix34 worldTransform = convertBulletToDirectX(rigidBody->worldTransform);
		//				if (!gameObject->GetComponent<app::player::GOCPlayerVisual>())
		//					worldTransform.translation() += physGoc->gocAnimator0->visualModel->frame2->fullTransform.position;
		//				auto& resRigidBody = physGoc->physSkelRes->rigidbodies[i];
		//				if (resRigidBody.isShapeBox)
		//					ctx.DrawOBB(worldTransform, csl::math::Vector3(resRigidBody.shapeRadius, resRigidBody.shapeHeight, resRigidBody.shapeDepth), rigidBodyColor);
		//				else {
		//					if (resRigidBody.shapeHeight > 0)
		//						ctx.DrawCapsule(worldTransform, resRigidBody.shapeRadius, resRigidBody.shapeHeight, rigidBodyColor);
		//					else
		//						ctx.DrawSphere(worldTransform, resRigidBody.shapeRadius, rigidBodyColor);
		//				}
		//			}
		//			if (physGoc->hasSoftBody) {
		//				for (auto i = 0; i < physGoc->softbodies.size(); i++) {
		//					const csl::ut::Color8 softBodyColor = { 0, 255, 0, 255 };
		//					auto* softBody = reinterpret_cast<btSoftBody*>(physGoc->softbodies[i * 2]);
		//					auto& resSoftBody = physGoc->physSkelRes->softbodies[i];
		//					for (auto x = 0; x < softBody->nodeCount; x++) {
		//						auto& node = softBody->nodes[x];
		//						auto transform = csl::math::Matrix34::Identity();
		//						transform.translate(node.position);
		//						ctx.DrawSphere(transform, 0.01f, softBodyColor);
		//					}
		//					auto* tempAlloc = hh::fnd::MemoryRouter::GetTempAllocator();
		//					hh::gfnd::DrawVertex* vertices = static_cast<hh::gfnd::DrawVertex*>(tempAlloc->Alloc((2 * softBody->linkCount) * sizeof(hh::gfnd::DrawVertex), 4));
		//					unsigned short* indices = static_cast<unsigned short*>(tempAlloc->Alloc((2 * softBody->linkCount) * sizeof(short), 2));
		//					for (auto x = 0; x < softBody->linkCount; x++) {
		//						auto& link = softBody->links[x];
		//						int curVert = x * 2;
		//						vertices[curVert].x = link.nodes[0]->position.x();
		//						vertices[curVert].y = link.nodes[0]->position.y();
		//						vertices[curVert].z = link.nodes[0]->position.z();
		//						vertices[curVert].color = 0xFF00FF00;
		//						vertices[curVert + 1].x = link.nodes[1]->position.x();
		//						vertices[curVert + 1].y = link.nodes[1]->position.y();
		//						vertices[curVert + 1].z = link.nodes[1]->position.z();
		//						vertices[curVert + 1].color = 0xFF00FF00;
		//						indices[curVert] = curVert;
		//						indices[curVert + 1] = curVert + 1;
		//					}
		//					ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, softBody->linkCount * 2);
		//					tempAlloc->Free(vertices);
		//					tempAlloc->Free(indices);
		//				}
		//			}
		//		}
		//	}
		//}
	}

	void PhysicalAnimation::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx)
	{
		if (!enabled)
			return;

		//auto* gameManager = hh::game::GameManager::GetInstance();
		//if (!gameManager)
		//	return;

		//for (auto* gameObject : gameManager->objects) {
		//	for (auto* goc : gameObject->components) {
		//		if (goc->pStaticClass == hh::pba::GOCPhysicalAnimationBullet::GetClass()) {
		//			auto* physGoc = static_cast<hh::pba::GOCPhysicalAnimationBullet*>(goc);
		//			for (auto i = 0; i < physGoc->rigidBodies.size(); i++) {
		//				const csl::ut::Color8 rigidBodyColor = { 255, 0, 255, 255 };
		//				auto* rigidBody = reinterpret_cast<btCollisionShape*>(physGoc->rigidBodies[i*2]);
		//				auto& resRigidBody = physGoc->physSkelRes->rigidbodies[i];
		//				csl::math::Vector3 translation = csl::math::Vector3{ rigidBody->worldTransform.translation() };
		//				if (!gameObject->GetComponent<app::player::GOCPlayerVisual>())
		//					translation += physGoc->gocAnimator0->visualModel->frame2->fullTransform.position;
		//				auto maybePos = ctx.WorldCoordsToImGui(translation);

		//				if (!maybePos.has_value())
		//					continue;

		//				OverlayTag(resRigidBody.boneName, false, maybePos.value(), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		//			}
		//		}
		//	}
		//}
	}
}
#endif
