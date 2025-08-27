#include "RenderingEngineInspector.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Needle.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::gfx;

RenderingEngineInspector::RenderingEngineInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("RenderingEngine");
}

void RenderingEngineInspector::RenderContents()
{
	auto* renderingEngine = static_cast<RenderingEngineRangers*>(static_cast<RenderManager*>(RenderManager::GetInstance())->GetNeedleResourceDevice());

	Editor("Weird toggle", *reinterpret_cast<uint32_t*>(0x143CFB388));

	ImGui::SeparatorText("Main rendering pipeline");
	Editor("Main rendering pipeline", *renderingEngine->mainRenderingPipeline);

	ImGui::SeparatorText("Main render unit");
	Editor("Main render unit", *renderingEngine->mainRenderUnit);

	ImGui::SeparatorText("SupportFX render units");
	for (auto& renderUnit : renderingEngine->supportFX.renderUnits) {
		if (ImGui::TreeNode(renderUnit.nameId, "%s", renderUnit.nameId->name)) {
			Editor("Pipeline", *static_cast<hh::needle::RenderTexturePipeline*>(&*renderUnit.pipeline));
			Editor("RenderUnit", renderUnit);
			ImGui::TreePop();
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SeparatorText("Render targets");
	for (size_t i = 0; i < 65; i++) {
		auto& rt = renderingEngine->supportFX.renderManager->renderTargetManager->renderTargetViews[i];

		if (rt == nullptr)
			continue;

		char name[20];
		snprintf(name, sizeof(name), "%zd", i);


		auto opos = ImGui::GetCursorPos();

		Editor(name, rt);

		if (i != 64) {
			ImGui::SameLine();
			if (ImGui::GetContentRegionAvail().x < 256)
				ImGui::Dummy({ 0, 0 });
		}

		auto npos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(opos);
		ImGui::Text("%zd", i);
		ImGui::SetCursorPos(npos);
	}

	ImGui::SeparatorText("Depth stencils");
	for (size_t i = 0; i < 24; i++) {
		auto& rt = renderingEngine->supportFX.renderManager->renderTargetManager->depthStencilViews[i];

		if (rt == nullptr)
			continue;

		char name[20];
		snprintf(name, sizeof(name), "%zd", i);

		auto opos = ImGui::GetCursorPos();

		Editor(name, rt);

		if (i != 23) {
			ImGui::SameLine();
			if (ImGui::GetContentRegionAvail().x < 256)
				ImGui::Dummy({ 0, 0 });
		}

		auto npos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(opos);
		ImGui::Text("%zd", i);
		ImGui::SetCursorPos(npos);
	}

	ImGui::SeparatorText("Unknown targets");
	for (size_t i = 0; i < 4; i++) {
		auto& rt = renderingEngine->supportFX.renderManager->renderTargetManager->unk7[i];

		if (rt == nullptr)
			continue;

		char name[20];
		snprintf(name, sizeof(name), "%zd", i);

		auto opos = ImGui::GetCursorPos();

		Editor(name, rt);

		if (i != 3) {
			ImGui::SameLine();
			if (ImGui::GetContentRegionAvail().x < 256)
				ImGui::Dummy({ 0, 0 });
		}

		auto npos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(opos);
		ImGui::Text("%zd", i);
		ImGui::SetCursorPos(npos);
	}

	ImGui::PopStyleColor();


	ImGui::SeparatorText("SupportFX textures");
	for (size_t i = 0; i < 13; i++) {
		auto& rt = renderingEngine->supportFX.textures[i];

		if (rt == nullptr)
			continue;

		char name[20];
		snprintf(name, sizeof(name), "%zd", i);

		auto opos = ImGui::GetCursorPos();

		Editor(name, rt);

		if (i != 12) {
			ImGui::SameLine();
			if (ImGui::GetContentRegionAvail().x < 256)
				ImGui::Dummy({ 0, 0 });
		}

		auto npos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(opos);
		ImGui::Text("%zd", i);
		ImGui::SetCursorPos(npos);
	}

	ImGui::SeparatorText("SupportFX");
	Editor("World scale", renderingEngine->supportFX.worldScale);

	ImGui::SeparatorText("SceneContextManager");

	for (auto& ctxMgr : renderingEngine->supportFX.sceneContextManagers) {
		if (ImGui::TreeNode(&ctxMgr, "%s", ctxMgr->nameId->name)) {
			for (auto& ctx : ctxMgr->sceneContexts) {
				if (ImGui::TreeNode(&ctx, "%s", ctx->GetName())) {
					if (ctx->GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440D0968ull)) {
						auto* sctx = static_cast<hh::needle::SCLocalLight*>(&*ctx);

						Viewer("Number of lights?", sctx->implementation->numLights);
						Viewer("Max lights?", sctx->implementation->maxLights);
					}
					if (ctx->GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440C8B64ull)) {
						auto* sctx = static_cast<hh::needle::SCAtmosphere*>(&*ctx);

						Editor("renderTargetViews[0]", sctx->implementation->renderTargetViews[0]);
						Editor("renderTargetViews[1]", sctx->implementation->renderTargetViews[1]);
						Editor("unk12", sctx->implementation->unk12);
						Editor("unk25", sctx->implementation->unk25);
						Editor("unk26", sctx->implementation->unk26);
					}
					if (ctx->GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440D0CE0ull)) {
						auto* sctx = static_cast<hh::needle::SCIBL*>(&*ctx);

						if (sctx->implementation->probeBVH != nullptr) {
							auto& probeBVH = sctx->implementation->probeBVH;

							size_t i{};
							for (auto& node : probeBVH->implementation->probeNodes) {
								ImGui::PushID(i++);
								if (ImGui::TreeNode("Node")) {
									Viewer("Texture name", node->textureName);
									Viewer("Resource name", node->resourceName);

									if (node->texture1 != nullptr)
										Editor("texture1", node->texture1);

									if (node->texture2 != nullptr)
										Editor("texture2", node->texture2);

									ImGui::TreePop();
								}
								ImGui::PopID();
							}

							if (probeBVH->implementation->ggxSamplingTexture.implementation != nullptr) {
								auto& ggxTexImpl = probeBVH->implementation->ggxSamplingTexture.implementation;
								
								if (ggxTexImpl->texture != nullptr)
									Editor("Texture", ggxTexImpl->texture);
							}
						}
					}
					//if (ctx->GetNameHash() == *reinterpret_cast<unsigned int*>(0x1440C8B64ull)) {
					//	auto* sctx = static_cast<hh::needle::SCAtmosphere*>(&*ctx);

					//	size_t i{};
					//	for (auto& rt : sctx->implementation->renderTargetViews) {
					//		if (rt == nullptr)
					//			continue;

					//		char name[20];
					//		snprintf(name, sizeof(name), "%zd", i);

					//		auto opos = ImGui::GetCursorPos();

					//		Editor(name, rt);

					//		if (i != 12) {
					//			ImGui::SameLine();
					//			if (ImGui::GetContentRegionAvail().x < 256)
					//				ImGui::Dummy({ 0, 0 });
					//		}

					//		auto npos = ImGui::GetCursorPos();

					//		ImGui::SetCursorPos(opos);
					//		ImGui::Text("%zd", i);
					//		ImGui::SetCursorPos(npos);
					//		i++;
					//	}
					//}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}
