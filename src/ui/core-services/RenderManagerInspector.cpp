#include "RenderManagerInspector.h"
#include <ui/common/editors/Needle.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::gfx;

bool Editor(const char* label, hh::gfnd::RenderManagerBase::SetupInfo& setupInfo) {
	bool edited{};

	Viewer("Shader packfile path", setupInfo.shaderPackfilePath);
	edited |= Editor("Aspect ratio", setupInfo.aspectRatio);
	edited |= Editor("unk2a", setupInfo.unk2a);
	edited |= Editor("unk3", setupInfo.unk3);
	edited |= Editor("Resolution X", setupInfo.resX);
	edited |= Editor("Resolution Y", setupInfo.resY);
	edited |= Editor("Render Resolution X", setupInfo.renderResX);
	edited |= Editor("Render Resolution Y", setupInfo.renderResY);
	edited |= Editor("unk6", setupInfo.unk6);
	edited |= Editor("unk7", setupInfo.unk7);
	edited |= Editor("unk8", setupInfo.unk8);
	//Viewer("renderDocDLL", setupInfo.renderDocDLLFilepath);

	return edited;
}

RenderManagerInspector::RenderManagerInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("RenderManager");
}

void RenderManagerInspector::RenderContents()
{
	if (auto* renderMgr = static_cast<RenderManager*>(RenderManager::GetInstance())) {
		Editor("Viewports", renderMgr->implementation->viewports);
		Viewer("Cameras", renderMgr->implementation->fxCameras);
		if (ImGui::TreeNode("Render parameters")) {
			Editor("Render parameters", renderMgr->implementation->renderParam);
			ImGui::TreePop();
		}
		Editor("unk2", renderMgr->implementation->unk2);
		Editor("numMainViewports", renderMgr->implementation->numMainViewports);
		Editor("unk3", renderMgr->implementation->unk3);
		Editor("unk4", renderMgr->implementation->unk4);
		Editor("unk5", renderMgr->implementation->unk5);
		Editor("unk6", renderMgr->implementation->unk6);
		Editor("maybeSupportFXWidth", renderMgr->implementation->maybeSupportFXWidth);
		Editor("maybeSupportFXHeight", renderMgr->implementation->maybeSupportFXHeight);
		Editor("Setup info", renderMgr->implementation->setupInfo);
		Editor("unk25", renderMgr->implementation->unk25);

		Editor("unk29suppfx", renderMgr->implementation->renderingEngine->GetSupportFX()->unk29);

		if (auto* drc = renderMgr->GetDynamicResolutionController()) {
			ImGui::SeparatorText("Dynamic Resolution Controller");
			Viewer("Ratio", drc->GetResolutionRatio());
		}
	}
}
