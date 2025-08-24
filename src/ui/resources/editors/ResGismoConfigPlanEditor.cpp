#include "ResGismoConfigPlanEditor.h"
#include <ucsl-reflection/reflections/resources/rfl/v1.h>
#include <ucsl-reflection/reflections/resources/rfl/v2.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/algorithms/copy.h>
#include <ucsl-reflection/traversals/traversal.h>
#include <rip/binary/containers/binary-file/v2.h>
#include <ui/common/editors/Reflection.h>
#include <ui/GlobalSettings.h>

using namespace hh::fnd;

ResGismoConfigPlanEditor::ResGismoConfigPlanEditor(csl::fnd::IAllocator* allocator, app::ResGismoConfigPlan* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
}

ResGismoConfigPlanEditor* ResGismoConfigPlanEditor::Create(csl::fnd::IAllocator* allocator, app::ResGismoConfigPlan* resource) {
	return new (allocator) ResGismoConfigPlanEditor(allocator, resource);
}

void ResGismoConfigPlanEditor::RenderContents()
{
	bool clickedExport = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Export", nullptr, &clickedExport);
		ImGui::EndMenuBar();
	}

	if (clickedExport) {
		IGFD::FileDialogConfig cfg{};
		cfg.path = GlobalSettings::defaultFileDialogDirectory;
		cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
		cfg.userDatas = resource;
		ImGuiFileDialog::Instance()->OpenDialog("ResGismoConfigPlanExportDialog", "Choose File", ".gismop", cfg);
	}

	if (ImGuiFileDialog::Instance()->Display("ResGismoConfigPlanExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* res = static_cast<app::ResGismoConfigPlan*>(ImGuiFileDialog::Instance()->GetUserDatas());

			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };

#if DEVTOOLS_TARGET_SDK_miller
			using namespace ucsl::resources::rfl::v2;
			thread_local const RflClass* gRflClass = &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigPlanData::rflClass);
			auto refl = ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::template reflect < reflections::Ref2Data < Ref2RflData, [](const Ref2Data<Ref2RflData>& d) -> const char* { return gRflClass->GetName(); } >> ();
			serializer.serialize(*(Ref2Data<Ref2RflData>*)res->ManagedResource::GetData(), refl);
#else
			using namespace ucsl::resources::rfl::v1;
			thread_local const RflClass* gRflClass = &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigPlanData::rflClass);
			auto refl = ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::template reflect < reflections::Ref1Data < Ref1RflData, [](const Ref1Data<Ref1RflData>& d) -> const char* { return gRflClass->GetName(); } >> ();
			serializer.serialize(*(Ref1Data<Ref1RflData>*)res->unpackedBinaryData, refl);
#endif
		}
		ImGuiFileDialog::Instance()->Close();
	}

	ReflectionEditor("Config Plan", resource->unpackedBinaryData, &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigPlanData::rflClass));
}
