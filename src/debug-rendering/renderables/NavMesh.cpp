#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "NavMesh.h"
#include <DetourNavMesh.h>

namespace devtools::debug_rendering::renderables {
	void NavMesh::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		if (auto* navmeshMgr = gameManager->GetService<hh::navmesh::NavMeshManager>()) {
			auto impl = navmeshMgr->implementation;
			for (auto& tile : impl->tiles) {
				auto* resTile = tile.resTile;
				char* binaryData = (char*)resTile->unpackedBinaryData;

				unsigned char* vandData = (unsigned char*)&binaryData[0x18];
				int vandSize = static_cast<int>(resTile->size - 0x18);

				dtMeshHeader* header = reinterpret_cast<dtMeshHeader*>(vandData);
				if (header->magic != DT_NAVMESH_MAGIC || header->version != DT_NAVMESH_VERSION)
					continue;

                float* verts = (float*)(vandData + sizeof(dtMeshHeader));
                dtPoly* polys = (dtPoly*)((unsigned char*)verts + sizeof(float) * header->vertCount * 3);
                dtLink* links = (dtLink*)((unsigned char*)polys + sizeof(dtPoly) * header->polyCount);
                dtPolyDetail* details = (dtPolyDetail*)((unsigned char*)links + (sizeof(dtLink) + 4) * header->maxLinkCount);
                float* detailVerts = (float*)((unsigned char*)details + sizeof(dtPolyDetail) * header->detailMeshCount);
                unsigned char* detailTris = (unsigned char*)(detailVerts + header->detailVertCount * 3);

                auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
                csl::ut::MoveArray<hh::gfnd::DrawVertex> objVerts{ allocator };
                csl::ut::MoveArray<unsigned short> objIndices{ allocator };

                for (int i = 0; i < header->polyCount; ++i) {
                    const dtPoly& poly = polys[i];
                    const dtPolyDetail& detail = details[i];

                    for (int j = 0; j < detail.triCount; ++j) {
                        const unsigned char* tri = &detailTris[(detail.triBase + j) * 4];

                        for (int k = 0; k < 3; ++k) {
                            int index = tri[k];
                            const float* v = nullptr;

                            if (index < poly.vertCount)
                                v = &verts[poly.verts[index] * 3];
                            else
                                v = &detailVerts[(detail.vertBase + (index - poly.vertCount)) * 3];

                            size_t base = objVerts.size();
                            objVerts.push_back(hh::gfnd::DrawVertex{
                                .x = v[0],
                                .y = v[1],
                                .z = v[2],
                                .color = 0x6400FF00
                                });
                            objIndices.push_back(base - 1);
                        }
                    }
                }

                ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_STRIP, objVerts.begin(), objIndices.begin(), objIndices.size());
			}
		}
	}
}
#endif
