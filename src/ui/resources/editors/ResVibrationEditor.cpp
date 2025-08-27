#include "ResVibrationEditor.h"
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/vibration/v21.h>
#include <rip/binary/containers/binary-file/v2.h>

bool ResVibrationEditor::Editor(Sound& value) {
	bool changed = false;

	if (ImGui::TreeNode("Sound")) {
		changed |= InputText("Sound Name", value.name, resource);
		changed |= ::Editor("Unk0", value.unk0);
		changed |= ::Editor("Unk1", value.unk1);
		ImGui::TreePop();
	}

	return changed;
}

bool ResVibrationEditor::Editor(VibrationKeyframe& value, unsigned int idx) {
	bool changed = false;

	ImGui::PushID(idx);

	double time = value.time;
	double intensity = value.intensity;
	bool clicked;
	bool hovered;
	bool held;

	if (changed |= ImPlot::DragPoint(idx, &time, &intensity, color, 4.0f, 0, &clicked, &hovered, &held)) {
		value.time = time;
		value.intensity = intensity;
	}

	if (clicked && !held)
		ImGui::OpenPopup("Editor");

	if (ImGui::BeginPopup("Editor")) {
		changed |= ::Editor("Flags", value.flags);
		changed |= ::Editor("Unk1", value.unk1);
		changed |= ::Editor("Unk2", value.unk2);
		ImGui::EndPopup();
	}

	if (hovered || held)
		ImGui::SetTooltip("Time: %f\nIntensity: %f", time, intensity);
	ImGui::PopID();

	return changed;
}

bool ResVibrationEditor::Editor(VibrationMotor& value, unsigned int idx, Vibration& parent) {
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("motor", "%d", value.motorId);

	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Motor%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (ImGui::Selectable("Remove Motor")) {
			resources::ManagedCArray<VibrationMotor, unsigned char> motors{ resource, parent.motors, parent.motorCount };
			motors.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		if (ImPlot::BeginPlot("##Track", ImVec2(450, 250), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
			ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
			ImPlot::SetupAxis(ImAxis_Y1, "Amplitude", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);

			auto* tempAllocator = hh::fnd::MemoryRouter::GetTempAllocator();
			float* time = static_cast<float*>(tempAllocator->Alloc(value.keyframeCount * sizeof(float), alignof(float)));
			float* intensity = static_cast<float*>(tempAllocator->Alloc(value.keyframeCount * sizeof(float), alignof(float)));
			for (unsigned int l = 0; l < value.keyframeCount; l++) {
				auto& keyframe = value.keyframes[l];
				time[l] = keyframe.time;
				intensity[l] = keyframe.intensity;
			}
			ImPlot::SetNextFillStyle(color, 0.3f);
			ImPlot::PlotLine<float>("X", time, intensity, value.keyframeCount, ImPlotLineFlags_Shaded);
			tempAllocator->Free(time);
			tempAllocator->Free(intensity);
			for (unsigned int l = 0; l < value.keyframeCount; l++) changed |= Editor(value.keyframes[l], l);
			ImPlot::EndPlot();
		}

		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResVibrationEditor::Editor(Vibration& value, unsigned int idx, VibData& parent) {
	bool changed = false;

	ImGui::PushID(&value);

	bool isOpen = ImGui::TreeNode("%s###", value.name);

	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Vibration%dOptions", idx);

	if (ImGui::BeginPopupContextItem(buffer)) {
		if (ImGui::Selectable("Remove Vibration")) {
			resources::ManagedCArray<Vibration, unsigned int> vibs{ resource, parent.vibrations, parent.vibrationCount };
			vibs.remove(idx);
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		changed |= InputText("Name", value.name, resource);
		changed |= ::Editor("Unk0", value.unk0);
		changed |= ::Editor("Looping", value.looping);
		changed |= ::Editor("Unk1", value.unk1);

		bool isOpen = ImGui::TreeNode("Motors");

		if (ImGui::BeginPopupContextItem("MotorsOptions")) {
			if (ImGui::Selectable("Add Motor")) {
				resources::ManagedCArray<VibrationMotor, unsigned char> motors{ resource, value.motors, value.motorCount };

				auto& motor = motors.emplace_back();
				motor = {};
				motor.motorId = motors.size() - 1;
			}
			ImGui::EndPopup();
		}

		if (isOpen) {
			for (unsigned char x = 0; x < value.motorCount; x++) changed |= Editor(value.motors[x], x, value);
			ImGui::TreePop();
		}
		Editor(*value.sound);
		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

bool ResVibrationEditor::Editor(VibData& value) {
	bool changed = false;
	
	bool isOpen = ImGui::TreeNode("Vibrations");

	if (ImGui::BeginPopupContextItem("VibrationsOptions")) {
		if (ImGui::Selectable("Add Vibration")) {
			resources::ManagedCArray<Vibration, unsigned int> vibs{ resource, value.vibrations, value.vibrationCount };

			auto& vib = vibs.emplace_back();
			vib = {};
			vib.name = "new vibration";
		}
		ImGui::EndPopup();
	}

	if (isOpen) {
		for (unsigned int i = 0; i < value.vibrationCount; i++) changed |= Editor(value.vibrations[i], i, value);
		ImGui::TreePop();
	}

	return changed;
}

ResVibrationEditor::ResVibrationEditor(csl::fnd::IAllocator* allocator, hh::hid::ResVibration* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->unpackedBinaryData);
	SetTitle(namebuf);
}

ResVibrationEditor* ResVibrationEditor::Create(csl::fnd::IAllocator* allocator, hh::hid::ResVibration* resource) {
	return new (allocator) ResVibrationEditor(allocator, resource);
}

void ResVibrationEditor::RenderContents()
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
		cfg.userDatas = resource->vibData;
		ImGuiFileDialog::Instance()->OpenDialog("ResVibrationExportDialog", "Choose File", ".vib", cfg);
	}
	if (ImGuiFileDialog::Instance()->Display("ResVibrationExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* exportData = static_cast<VibData*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
			rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
			serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	bool changed = Editor(*resource->vibData);
}
