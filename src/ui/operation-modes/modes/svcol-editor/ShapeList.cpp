#include "ShapeList.h"
#include "Behaviors.h"
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/svcol/v1.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <ucsl-reflection/accessors/binary.h>
#include <rip/models/binary-file/v2.h>
#include <rip/serialization/binary.h>
#include <span>

namespace ui::operation_modes::modes::svcol_editor {
	using namespace ucsl::resources::svcol::v1;

	void ShapeList::RenderPanel() {
		auto& context = GetContext();

		if (ImGui::BeginCombo("Resources", context.resource == nullptr ? "<none>" : context.resource->GetName())) {
#ifdef DEVTOOLS_TARGET_SDK_miller
			for (auto* resource : hh::fnd::ResourceManager::GetInstance()->unpackedResourceContainer.GetResourcesByTypeInfo(app::ResSvCol2::GetTypeInfo())) {
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
			for (auto* resource : hh::fnd::ResourceManager::GetInstance()->GetResourcesByTypeInfo(app::ResSvCol2::GetTypeInfo())) {
#endif
#ifdef DEVTOOLS_TARGET_SDK_wars
			auto* packfileContainer = hh::fnd::ResourceManager::GetInstance()->GetResourceContainer(hh::fnd::Packfile::GetTypeInfo());

			auto numPackfiles = packfileContainer->GetNumResources();

			for (int i = 0; i < numPackfiles; i++) {
				auto* packfile = (hh::fnd::Packfile*)packfileContainer->GetResourceByIndex(i);

				auto* resContainer = packfile->GetResourceContainer(app::ResSvCol2::GetTypeInfo());

				if (!resContainer)
					continue;

				auto numResources = resContainer->GetNumResources();

				for (int j = 0; j < numResources; j++) {
					auto* resource = resContainer->GetResourceByIndex(j);
#endif
					auto* svColResource = (app::ResSvCol2*)resource;
					if (ImGui::Selectable(resource->GetName(), context.resource == svColResource)) {
						GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
						context.resource = svColResource;
					}
					if (context.resource == svColResource)
						ImGui::SetItemDefaultFocus();
#ifdef DEVTOOLS_TARGET_SDK_wars
				}
#endif
			}
			ImGui::EndCombo();
		}

		if (context.resource == nullptr) {
			ImGui::Text("No resource loaded.");
			return;
		}

		auto& svColData = *context.resource->svColData;

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = &svColData;
			ImGuiFileDialog::Instance()->OpenDialog("ResSvColFile2ExportDialog", "Choose File", ".svcol", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResSvColFile2ExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<ucsl::resources::svcol::v1::SvColData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::svcol::v1::reflections::SvColData> refl{};
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueConstAccessor<decltype(refl)> acc{ exportData, refl };

				rip::models::BinaryFileV2<decltype(refl), he2sdk::ucsl::GameInterface::AllocatorSystem> model{ rip::binary::serializeBinaryToAllocatorSystemBuffer<he2sdk::ucsl::GameInterface::AllocatorSystem, size_t>(acc) };

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::fast_ostream fos{ ofs };
				rip::binary::binary_ostream<rip::binary::fast_ostream, size_t> bos{ fos };

				rip::serialization::binary<decltype(model)>::save(bos, model);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		for (auto& shape : std::span(svColData.shapes, svColData.shapeCount))
			RenderShapeItem(shape);
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ShapeList::RenderShapeItem(ShapeData& shape)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		ImGui::PushID(&shape);
		if (ImGui::Selectable(shape.name, selectionBehavior->GetSelection().find(&shape) != -1))
			selectionBehavior->Select(&shape);
		ImGui::PopID();
	}
}
