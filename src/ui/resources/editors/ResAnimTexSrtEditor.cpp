#include "ResAnimTexSrtEditor.h"
#include <imgui_internal.h>

using namespace hh::gfx;
using namespace hh::needle;
using namespace hh::fnd;

void ResAnimTexSrtEditor::SetModel(hh::needle::PBRModelInstance* model) {
	previewModel = model;

	if (blender)
		previewModel->SetTexSrtBlender(blender->needleBlender);

	if (!viewport) return;

	viewport->ClearModels();
	viewport->AddModel(model, true);
}

void ResAnimTexSrtEditor::SetModel(hh::gfx::ResModel* model) {
	auto* renderMgr = reinterpret_cast<hh::gfx::RenderManager*>(hh::gfnd::RenderManagerBase::GetInstance());
	hh::gfx::RenderManager::SModelCreationInfo createInfo{ pAllocator };
	createInfo.meshResource = model->GetMeshResource();

	Model* mdl;
	renderMgr->CreateModelFromResource2(&mdl, createInfo);
	auto* modelInstance = static_cast<PBRModelInstance*>(renderMgr->CreateModelInstance(mdl, createInfo));

	SetModel(modelInstance);
}

ResAnimTexSrtEditor::ResAnimTexSrtEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResAnimTexSrt* resource) : StandaloneWindow{ allocator }, resource{ resource } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource);
	SetTitle(namebuf);

	blender = new (allocator) TexSrtBlenderHH{ allocator };
	TexSrtBlenderDesc blenderDesc{ .animationCount = 1 };
	blender->Initialize(blenderDesc);
	blender->needleBlender->controls[0] = hh::needle::AnimTexSrtControl::Create(resource->resource);

	static const char* nameTemplate = "UVAnimViewport";
	static const size_t nameTemplateLen = strlen(nameTemplate);

	const char* resName = resource->name.c_str();
	const size_t resNameLen = strlen(resName);

	const size_t nameLen = resNameLen + nameTemplateLen + 1;
	char* nameRaw = new (GetAllocator()) char[nameLen];
	snprintf(nameRaw, nameLen, "%s%s", resName, nameTemplate);

	viewport = new (allocator) Viewport{ allocator };
	viewport->Setup({ .name = nameRaw });

	GetAllocator()->Free(nameRaw);
}

ResAnimTexSrtEditor* ResAnimTexSrtEditor::Create(csl::fnd::IAllocator* allocator, hh::gfx::ResAnimTexSrt* resource) {
	return new (allocator) ResAnimTexSrtEditor{ allocator, resource };
}

// TODO: Requires a different way of handling the UVAnimation, or a manual update function
void ResAnimTexSrtEditor::RenderContents() {
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->ContentRegionRect, ImGui::GetCurrentWindow()->ID)) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResModel::GetTypeInfo())
				SetModel(reinterpret_cast<ResModel*>(res));
		}
		ImGui::EndDragDropTarget();
	}

	viewport->Render();
}
