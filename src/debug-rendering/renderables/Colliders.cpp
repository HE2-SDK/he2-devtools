#include "Colliders.h"

namespace devtools::debug_rendering::renderables {
	void Colliders::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			//if (auto* gocV = gameObject->GetComponent<GOCVisualTransformed>()) {
			//	renderer->drawContext->DrawOBB(gocV->worldMatrix, { 1, 1, 1 }, { 255, 255, 255, 0 });
			//	renderer->drawContext->DrawAABB(gocV->transformedAabb.m_Min, gocV->transformedAabb.m_Max, { 255, 255, 255, 255 });
			//}
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCSphereCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawSphere(cGoc->GetWorldTransform(), cGoc->radius, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCBoxCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawOBB(cGoc->GetWorldTransform(), cGoc->dimensions, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCapsuleCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawCapsule(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight / 2, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCylinderCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawCylinder(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight, { 0, 255, 255, 255 });
				}
#ifdef DEVTOOLS_TARGET_SDK_rangers
				// WIP - sometimes meshes don't render fully or at all. or they render at the wrong position
				else if (meshEnabled 
					&& goc->pStaticClass == hh::physics::GOCMeshCollider::GetClass()) 
				{
					auto* cGoc = static_cast<hh::physics::GOCMeshCollider*>(goc);
					auto meshResource = cGoc->meshResource;

					auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
					csl::ut::MoveArray<hh::physics::ResPhysicsMesh::MeshInfo*> meshInfos{ allocator };

					if (meshResource->GetMeshInfo(meshInfos, allocator)) {
						for (auto* meshInfo : meshInfos) {
							size_t vertexCount = meshInfo->vertices.size();
							size_t indexCount = meshInfo->faces.size();

							hh::gfnd::DrawVertex* vertices = new (allocator) hh::gfnd::DrawVertex[vertexCount];
							for (int i = 0; i < vertexCount; i++) {
								auto& pos = meshInfo->vertices[i];
								vertices[i].x = pos.x();
								vertices[i].y = pos.y();
								vertices[i].z = pos.z();
								vertices[i].color = 0xFFFFFF00;
							}

							unsigned short* indices = new (allocator) unsigned short[indexCount * 3 * 2];
							for (int i = 0; i < indexCount; i++) {
								auto& f = meshInfo->faces[i];
								auto curIndex = i * 6;
								indices[curIndex] = f.a;
								indices[curIndex + 1] = f.b;
								indices[curIndex + 2] = f.b;
								indices[curIndex + 3] = f.c;
								indices[curIndex + 4] = f.c;
								indices[curIndex + 5] = f.a;
							}

							ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, indexCount * 3 * 2);

							allocator->Free(vertices);
							allocator->Free(indices);

							meshInfo->vertices.~MoveArray();
							meshInfo->faces.~MoveArray();
							allocator->Free(meshInfo);
						}
					}
				}
#endif
			}
		}
	}
}
