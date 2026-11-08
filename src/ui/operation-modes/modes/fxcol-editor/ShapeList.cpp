#include "ShapeList.h"
#include "Behaviors.h"
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/fxcol/v1.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <ucsl-reflection/accessors/binary.h>
#include <rip/models/binary-file/v2.h>
#include <rip/serialization/binary.h>

namespace ui::operation_modes::modes::fxcol_editor {
	void ShapeList::RenderPanel() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

		if (!fxColManager) {
			ImGui::Text("FxColManager service not active.");
			return;
		}

		if (!fxColManager->resource) {
			ImGui::Text("No resource loaded.");
			return;
		}

		auto* fxColData = fxColManager->resource->fxColData;

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = fxColData;
			ImGuiFileDialog::Instance()->OpenDialog("ResFxColFile2ExportDialog", "Choose File", ".fxcol", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResFxColFile2ExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<ucsl::resources::fxcol::v1::FxColData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				ucsl::reflection::providers::simplerfl<he2sdk::ucsl::GameInterface>::RootType<ucsl::resources::fxcol::v1::reflections::FxColData> refl{};
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueConstAccessor<decltype(refl)> acc{ exportData, refl };

				rip::models::BinaryFileV2<decltype(refl), he2sdk::ucsl::GameInterface::AllocatorSystem> model{ rip::binary::serializeBinaryToAllocatorSystemBuffer<he2sdk::ucsl::GameInterface::AllocatorSystem, size_t>(acc) };

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::fast_ostream fos{ ofs };
				rip::binary::binary_ostream<rip::binary::fast_ostream, size_t> bos{ fos };

				rip::serialization::binary<decltype(model)>::save(bos, model);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		auto& context = GetContext();
		const char* targetBoundingVolumePreview = "<none>";
		char targetBoundingVolumeName[100];

		for (size_t i = 0; i < fxColData->kdTreeLeafCount; i++) {
			auto& boundingVolume = fxColData->kdTreeLeaves[i];

			if (context.placementVolume == &boundingVolume) {
				snprintf(targetBoundingVolumeName, sizeof(targetBoundingVolumeName), "Bounding Volume %zd", i);

				targetBoundingVolumePreview = targetBoundingVolumeName;
			}
		}

		if (ImGui::BeginCombo("Target placement volume", targetBoundingVolumePreview)) {
			for (size_t i = 0; i < fxColData->kdTreeLeafCount; i++) {
				auto& boundingVolume = fxColData->kdTreeLeaves[i];
				char name[100];
				snprintf(name, sizeof(name), "Bounding Volume %zd", i);

				if (ImGui::Selectable(name, context.placementVolume == &boundingVolume))
					context.placementVolume = &boundingVolume;
				if (context.placementVolume == &boundingVolume)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Create bounding volume"))
				GetContext().AddBoundingVolume();
			ImGui::EndPopup();
		}

		for (size_t i = 0; i < fxColData->kdTreeLeafCount; i++) {
			auto& boundingVolume = fxColData->kdTreeLeaves[i];
			char name[100];
			snprintf(name, sizeof(name), "Bounding Volume %zd", i);

			ImGui::PushID(&boundingVolume);

			bool isOpen = ImGui::TreeNode(&boundingVolume, name);

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Remove bounding volume"))
					GetContext().RemoveBoundingVolume(&boundingVolume);
				ImGui::EndPopup();
			}

			if (isOpen) {
				for (size_t j = 0; j < boundingVolume.shapeCount; j++)
					RenderShapeItem(fxColData->shapes[boundingVolume.shapeOffset + j]);

				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		for (size_t i = 0; i < fxColData->shapeCount; i++) {
			bool found{};

			for (size_t j = 0; j < fxColData->kdTreeLeafCount; j++) {
				if (i >= fxColData->kdTreeLeaves[j].shapeOffset && i < fxColData->kdTreeLeaves[j].shapeOffset + fxColData->kdTreeLeaves[j].shapeCount) {
					found = true;
					break;
				}
			}

			if (found)
				break;

			RenderShapeItem(fxColData->shapes[i]);
		}
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ShapeList::RenderShapeItem(FxColCollisionShapeData& shape)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		ImGui::PushID(&shape);
		if (ImGui::Selectable(shape.name, selectionBehavior->GetSelection().find(&shape) != -1))
			selectionBehavior->Select(&shape);
		ImGui::PopID();
	}
}
