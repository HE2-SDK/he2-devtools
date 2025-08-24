#include "ResMirageLightEditor.h"
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <rip/binary/containers/mirage/v1.h>

ResMirageLightEditor::ResMirageLightEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResMirageLight* resource, hh::gfx::GOCPointLight* goc, unsigned int idx) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
	if (goc) {
		gocPointLight = goc;
		if (idx < gocPointLight->lights.size()) {
			lightIdx = idx;
			gocLight = &gocPointLight->lights[idx];
		}
	}
}

ResMirageLightEditor* ResMirageLightEditor::Create(csl::fnd::IAllocator* allocator, hh::gfx::ResMirageLight* resource, hh::gfx::GOCPointLight* goc, unsigned int idx) {
	return new (allocator) ResMirageLightEditor(allocator, resource, goc, idx);
}

void ResMirageLightEditor::RenderContents()
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
		//cfg.userDatas = binaryData;
		ImGuiFileDialog::Instance()->OpenDialog("ResMirageLightExportDialog", "Choose File", ".light", cfg);
	}
	if (ImGuiFileDialog::Instance()->Display("ResMirageLightExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			/*auto* exportData = static_cast<ucsl::resources::pba::v1::PbaData*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
			serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);*/
		}
		ImGuiFileDialog::Instance()->Close();
	}

	bool changed = false;

	auto& lightParam = resource->lightParameter;

	const char* typeNames[] = {
		"SUN",
		"POINT",
		"SPOT"
	};

	changed |= ComboEnum("Type", lightParam.lightType, typeNames);
	changed |= Editor("Position", lightParam.lightPosition);
	changed |= Editor("Color", lightParam.color);
	if (lightParam.lightType == ucsl::resources::light::v2::LightType::POINT) {
		changed |= Editor("Shadows Enabled", lightParam.pointLightParam.shadowEnabled);
		changed |= Editor("Attenuation Radius", lightParam.pointLightParam.attenuationRadius);
		changed |= Editor("Radius", lightParam.pointLightParam.radius);
	}
	else if (lightParam.lightType == ucsl::resources::light::v2::LightType::SPOT) {
		changed |= Editor("Shadows Enabled", lightParam.spotLightParam.shadowEnabled);
		changed |= Editor("Attenuation Radius", lightParam.spotLightParam.attenuationRadius);
		changed |= Editor("Inner Cone Angle", lightParam.spotLightParam.innerConeAngle);
		changed |= Editor("Outer Cone Angle", lightParam.spotLightParam.outerConeAngle);
	}

	if (changed) {
		if (gocPointLight) {
			hh::fnd::HFrame* hFrame = gocLight->hFrame;
			gocPointLight->RemoveLight(lightIdx);
			lightIdx = gocPointLight->CreateLight(resource, hFrame);
			gocLight = &gocPointLight->lights[lightIdx];
		}
	}
}
