#ifndef DEVTOOLS_TARGET_SDK_wars
#include "PhysicalAnimation.h"
#include <ui/common/overlays/Tag.h>
#include <utilities/math/MathUtils.h>

namespace devtools::debug_rendering::renderables {
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

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::pba::GOCPhysicalAnimationBullet::GetClass()) {
					auto* gocPBA = static_cast<hh::pba::GOCPhysicalAnimationBullet*>(goc);

					auto animTf = TransformToAffine3f(gocPBA->gocAnimationSingle->GetTransform());

					csl::ut::MoveArray<hh::fnd::WorldPosition> rigidBodyPositions{ hh::fnd::MemoryRouter::GetTempAllocator() };
					gocPBA->GetRigidBodyPositions(rigidBodyPositions);

					for (size_t i = 0; i < gocPBA->physSkelRes->rigidbodies.size(); i++) {
						static const csl::ut::Color8 rigidBodyColor{ 255, 0, 255, 255 };
						auto& rb = gocPBA->physSkelRes->rigidbodies[i];
						auto& pos = rigidBodyPositions[i];

						csl::math::Matrix34 tf{};
						tf.fromPositionOrientationScale(pos.m_Position, pos.m_Rotation, csl::math::Vector3{ 1.0f, 1.0f, 1.0f });

						if (rb.shape == ucsl::resources::pba::v1::RigidBody::Shape::SPHERE) {
							if (rb.shapeHeight > 0)
								ctx.DrawCapsule(animTf * tf, rb.shapeRadius, rb.shapeHeight, rigidBodyColor);
							else
								ctx.DrawSphere(animTf * tf, rb.shapeRadius, rigidBodyColor);
						}
						else
							ctx.DrawOBB(animTf * tf, { rb.shapeRadius, rb.shapeHeight, rb.shapeDepth }, rigidBodyColor);
					}

					if (gocPBA->hasSoftBody) {
						for (auto i = 0; i < gocPBA->physSkelRes->softbodies.size(); i++) {
							static const csl::ut::Color8 softBodyColor{ 0, 255, 0, 255 };
							auto& sb = gocPBA->physSkelRes->softbodies[i];

							csl::ut::MoveArray<csl::math::Vector3> nodePositions{ hh::fnd::MemoryRouter::GetTempAllocator() };
							gocPBA->GetSoftBodyNodePositions(i, nodePositions);

							for (auto x = 0; x < sb.nodes.size(); x++) {
								auto transform{ csl::math::Matrix34::Identity() };
								transform.translate(nodePositions[x]);
								ctx.DrawSphere(transform, 0.01f, softBodyColor);
							}

							auto* tempAlloc = hh::fnd::MemoryRouter::GetTempAllocator();

							unsigned int linkCount = sb.links.size();
							unsigned int linkSize = linkCount * 2;
							hh::gfnd::DrawVertex* vertices = new (tempAlloc) hh::gfnd::DrawVertex[linkSize];
							unsigned short* indices = new (tempAlloc) unsigned short[linkSize];

							for (auto x = 0; x < linkCount; x++) {
								auto& link = sb.links[x];
								csl::math::Vector3 nodes[2]{ nodePositions[link.verts[0]], nodePositions[link.verts[1]] };
								int curVert = x * 2;
								vertices[curVert] = { .x = nodes[0].x(), .y = nodes[0].y(), .z = nodes[0].z(), .color = 0xFF00FF00 };
								vertices[curVert + 1] = { .x = nodes[1].x(), .y = nodes[1].y(), .z = nodes[1].z(), .color = 0xFF00FF00 };
								indices[curVert] = curVert;
								indices[curVert + 1] = curVert + 1;
							}
							ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, linkSize);

							tempAlloc->Free(vertices);
							tempAlloc->Free(indices);
						}
					}
				}
			}
		}
	}

	void PhysicalAnimation::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx)
	{
		/*if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::pba::GOCPhysicalAnimationBullet::GetClass()) {
					auto* physGoc = static_cast<hh::pba::GOCPhysicalAnimationBullet*>(goc);
					for (auto i = 0; i < physGoc->rigidBodies.size(); i++) {
						const csl::ut::Color8 rigidBodyColor = { 255, 0, 255, 255 };
						auto* rigidBody = reinterpret_cast<btCollisionShape*>(physGoc->rigidBodies[i*2]);
						auto& resRigidBody = physGoc->physSkelRes->rigidbodies[i];
						csl::math::Vector3 translation = csl::math::Vector3{ rigidBody->worldTransform.translation() };
						if (!gameObject->GetComponent<app::player::GOCPlayerVisual>())
							translation += physGoc->gocAnimator0->visualModel->frame2->fullTransform.position;
						auto maybePos = ctx.WorldCoordsToImGui(translation);

						if (!maybePos.has_value())
							continue;

						OverlayTag(resRigidBody.boneName, false, maybePos.value(), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					}
				}
			}
		}*/
	}
}
#endif
