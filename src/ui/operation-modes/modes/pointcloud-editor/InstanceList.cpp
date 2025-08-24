#include "InstanceList.h"
#include "Behaviors.h"
#include <ui/GlobalSettings.h>
#include <resources/ReloadManager.h>
#include <utilities/ResourceTypes.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>
#include <ucsl-reflection/reflections/resources/pointcloud/v2.h>
#include <rip/binary/containers/binary-file/v2.h>
#include <span>

namespace ui::operation_modes::modes::pointcloud_editor {
	using namespace ucsl::resources::pointcloud::v2;

	void InstanceList::RenderPanel() {
		auto& context = GetContext();

		if (ImGui::BeginCombo("Resources", context.resource == nullptr ? "<none>" : context.resource->GetName())) {
#ifdef DEVTOOLS_TARGET_SDK_miller
			for (auto* resource : hh::fnd::ResourceManager::GetInstance()->unpackedResourceContainer.GetResourcesByTypeInfo(context.pcType)) {
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
			for (auto* resource : hh::fnd::ResourceManager::GetInstance()->GetResourcesByTypeInfo(context.pcType)) {
#endif
#ifdef DEVTOOLS_TARGET_SDK_wars
			auto* packfileContainer = hh::fnd::ResourceManager::GetInstance()->GetResourceContainer(hh::fnd::Packfile::GetTypeInfo());

			auto numPackfiles = packfileContainer->GetNumResources();

			for (int i = 0; i < numPackfiles; i++) {
				auto* packfile = (hh::fnd::Packfile*)packfileContainer->GetResourceByIndex(i);

				auto* resContainer = packfile->GetResourceContainer(context.pcType);

				if (!resContainer)
					continue;

				auto numResources = resContainer->GetNumResources();

				for (int j = 0; j < numResources; j++) {
					auto* resource = resContainer->GetResourceByIndex(j);
#endif
					auto* pointcloudResource = (hh::gfx::ResPointcloud*)resource;
					if (ImGui::Selectable(resource->GetName(), context.resource == pointcloudResource)) {
						GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
						context.resource = pointcloudResource;
					}
					if (context.resource == pointcloudResource)
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

		auto& pointcloudData = *context.resource->pointcloudData;

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = &pointcloudData;
			ImGuiFileDialog::Instance()->OpenDialog("ResPointcloudExportDialog", "Choose File", ".pointcloud", cfg);
		}

		if (ImGui::Button("Reload"))
			ReloadManager::instance->ReloadSync(context.resource);

		if (ImGuiFileDialog::Instance()->Display("ResPointcloudExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<ucsl::resources::pointcloud::v2::PointcloudData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		for (auto& instance : std::span(pointcloudData.instances, pointcloudData.instanceCount))
			RenderInstance(instance);
	}

	PanelTraits InstanceList::GetPanelTraits() const {
		return PanelTraits{ "Instance List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void InstanceList::RenderInstance(InstanceData& instance)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		ImGui::PushID(&instance);
		if (ImGui::Selectable(instance.name, selectionBehavior->GetSelection().find(&instance) != -1))
			selectionBehavior->Select(&instance);
		ImGui::PopID();
	}
}
