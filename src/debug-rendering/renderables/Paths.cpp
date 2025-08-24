#include "Paths.h"
#include <ui/common/overlays/Tag.h>

namespace devtools::debug_rendering::renderables {
	constexpr unsigned int maxStepCount = 256;

	void Paths::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::path::PathComponent::GetClass()) {
					hh::path::PathEvaluator pathEvaluator{ static_cast<hh::path::PathComponent*>(goc) };
					hh::gfnd::DrawVertex segments[2 * maxStepCount];
					hh::gfnd::DrawVertex normals[2 * maxStepCount];
					hh::gfnd::DrawVertex tangents[2 * maxStepCount];
					unsigned short indices[2 * maxStepCount];

					float len = pathEvaluator.GetLength();

					unsigned int stepCount{ std::min(static_cast<unsigned int>(len), maxStepCount) };
					unsigned int vertexCount{ 2 * stepCount };

					for (size_t i = 0; i < stepCount; i++) {
						float time = len * (float)i / static_cast<float>(stepCount);
						float nextTime = len * (float)(i + 1) / static_cast<float>(stepCount);

						csl::math::Vector3 pos, nrm, tan;
						pathEvaluator.GetPNT(time, &pos, &nrm, &tan);
#ifdef DEVTOOLS_TARGET_SDK_wars
						csl::math::Vector3 nrmEnd = pos + nrm * 5.0f;
						csl::math::Vector3 tanEnd = pos + tan * 5.0f;
#else
						csl::math::Vector3 nrmEnd = pos + nrm * 0.50f;
						csl::math::Vector3 tanEnd = pos + tan * 0.50f;
#endif

						segments[i * 2] = { pos.x(), pos.y(), pos.z(), 0xFF00FF00 };

						if (normalsEnabled) {
							normals[i * 2] = { pos.x(), pos.y(), pos.z(), 0xFF0000FF };
							normals[i * 2 + 1] = { nrmEnd.x(), nrmEnd.y(), nrmEnd.z(), 0xFF0000FF };
						}

						if (tangentsEnabled) {
							tangents[i * 2] = { pos.x(), pos.y(), pos.z(), 0xFFFF0000 };
							tangents[i * 2 + 1] = { tanEnd.x(), tanEnd.y(), tanEnd.z(), 0xFFFF0000 };
						}

						pos = pathEvaluator.GetWorldPosition(nextTime);

						segments[i * 2 + 1] = { pos.x(), pos.y(), pos.z(), 0xFF00FF00 };
					}

					for (unsigned short i = 0; i < vertexCount; i++)
						indices[i] = i;

					ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, segments, indices, vertexCount);

					if (normalsEnabled)
						ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, normals, indices, vertexCount);

					if (tangentsEnabled)
						ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, tangents, indices, vertexCount);
				}
			}
		}
	}

	void Paths::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) {
		if (!enabled)
			return;

		for (auto* gameObject : hh::game::GameManager::GetInstance()->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::path::PathComponent::GetClass()) {
					auto* pathGoc = static_cast<hh::path::PathComponent*>(goc);
					auto maybePos = ctx.WorldCoordsToImGui(hh::path::PathEvaluator{ pathGoc }.GetWorldPosition(0.0f));

					if (!maybePos.has_value())
						continue;

					OverlayTag(pathGoc->GetName(), false, maybePos.value(), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

					if (ImGui::BeginDragDropSource()) {
						ImGui::SetDragDropPayload("Path", &pathGoc, sizeof(pathGoc));
						ImGui::EndDragDropSource();
					}
				}
			}
		}
	}
}
