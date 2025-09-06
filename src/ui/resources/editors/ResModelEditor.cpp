#include "ResModelEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/Textures.h>

using namespace hh::gfx;
using namespace hh::needle;

ResModelEditor::~ResModelEditor()
{
	delete previewer;
}

ResModelEditor::ResModelEditor(csl::fnd::IAllocator* allocator, ResModel* resource) : StandaloneWindow{ allocator }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource);
	SetTitle(namebuf);

	static const char* nameTemplate = "ModelPreview";
	static const size_t nameTemplateLen = strlen(nameTemplate);

	const char* resName = resource->name.c_str();
	const size_t resNameLen = strlen(resName);

	const size_t nameLen = resNameLen + nameTemplateLen + 1;
	char* nameRaw = static_cast<char*>(pAllocator->Alloc(nameLen, 4));
	snprintf(nameRaw, nameLen, "%s%s", resName, nameTemplate);

	previewer = new (allocator) Previewer{ allocator };
	previewer->Setup({.name = nameRaw});
	previewer->AddModel(resource, true);
}

ResModelEditor* ResModelEditor::Create(csl::fnd::IAllocator* allocator, ResModel* resource) {
	return new (allocator) ResModelEditor(allocator, resource);
}

void ResModelEditor::RenderContents()
{
	if (auto* texture = previewer->GetTextureID()) {
		ImVec2 pos = ImGui::GetCursorScreenPos();

		auto& dimensions = previewer->viewportData.viewportDimensions;
		ImVec2 size = { static_cast<float>(dimensions.resX), static_cast<float>(dimensions.resY) };

		ImGui::GetWindowDrawList()->AddRectFilled(
			pos,
			ImVec2(pos.x + size.x, pos.y + size.y),
			IM_COL32(0, 0, 0, 255)
		);

		ImGui::Image(texture, size);
	}
}
