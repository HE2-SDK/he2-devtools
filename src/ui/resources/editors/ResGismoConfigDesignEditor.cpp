#include "ResGismoConfigDesignEditor.h"
#include <ucsl-reflection/reflections/resources/rfl/v1.h>
#include <ucsl-reflection/reflections/resources/rfl/v2.h>
#include <ucsl-reflection/accessors/binary.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <rip/binary/serialization2/binary.h>
#include <rip/binary/containers/binary-file/v2.h>
#include <rip/serialization/binary.h>
#include <ui/common/editors/Reflection.h>
#include <ui/GlobalSettings.h>

using namespace hh::fnd;

ResGismoConfigDesignEditor::ResGismoConfigDesignEditor(csl::fnd::IAllocator* allocator, app::ResGismoConfigDesign* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
}

ResGismoConfigDesignEditor* ResGismoConfigDesignEditor::Create(csl::fnd::IAllocator* allocator, app::ResGismoConfigDesign* resource) {
	return new (allocator) ResGismoConfigDesignEditor(allocator, resource);
}

inline std::string get_rfl_class() { return "GismoConfigDesignData"; }

void ResGismoConfigDesignEditor::RenderContents()
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
		ImGuiFileDialog::Instance()->OpenDialog("ResGismoConfigDesignExportDialog", "Choose File", ".gismod", cfg);
	}

	if (ImGuiFileDialog::Instance()->Display("ResGismoConfigDesignExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* res = static_cast<app::ResGismoConfigDesign*>(ImGuiFileDialog::Instance()->GetUserDatas());

#if DEVTOOLS_TARGET_SDK_miller
			ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::rfl::v2::reflections::Ref2Data<ucsl::resources::rfl::v2::Ref2RflData, simplerfl::selector_resolver<std::string>::impl<get_rfl_class>>> refl{};
#else
			ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::rfl::v1::reflections::Ref1Data<ucsl::resources::rfl::v1::Ref1RflData, simplerfl::selector_resolver<std::string>::impl<get_rfl_class>>> refl{};
#endif
			ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueConstAccessor<decltype(refl)> acc{ res, refl };

			rip::models::BinaryFileV2<decltype(refl), he2sdk::ucsl::GameInterface::AllocatorSystem> model{ rip::binary::serializeBinaryToAllocatorSystemBuffer<he2sdk::ucsl::GameInterface::AllocatorSystem, size_t>(acc) };

			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::fast_ostream fos{ ofs };
			rip::binary::binary_ostream<rip::binary::fast_ostream, size_t> bos{ fos };

			rip::serialization::binary<decltype(model)>::save(bos, model);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	ReflectionEditor("Config Design", resource->unpackedBinaryData, &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigDesignData::rflClass));
}
