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

static const char* keyframeTypeNames[]{
	"LINEAR",
	"CONSTANT",
	"HERMITE"
};

// TODO: Clean up, very janky right now
bool DerivativeEditor(float* value, float* pos, unsigned int idx, bool invert = false) {
	bool changed = false;
	double valueX = value[0];
	double valueY = value[1];
	if (invert) {
		valueX = -valueX;
		valueY = -valueY;
	}

	double x = valueX/10 + pos[0];
	double y = valueY/10 + pos[1];

	if (changed |= ImPlot::DragPoint(idx, &x, &y, ImVec4{ 1, 1, 0, 1 })) {
		value[0] = (x - pos[0])*10;
		value[1] = (y - pos[1])*10;
		if (invert) {
			value[0] = -value[0];
			value[1] = -value[1];
		}
		value[0] = std::clamp(value[0], 0.0f, 1.0f);
		value[1] = std::clamp(value[1], -1.0f, 1.0f);
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
		value.time = std::clamp((float)time, 0.0f, INFINITY);
		value.intensity = std::clamp((float)intensity, 0.0f, INFINITY);
	}

	if (value.type == ucsl::resources::vibration::v21::KeyframeType::HERMITE) {
		changed |= DerivativeEditor(value.in, &value.time, idx |= (1 << 28), true);
		changed |= DerivativeEditor(value.out, &value.time, idx |= (1 << 29));
	}

	if (clicked && !held)
		ImGui::OpenPopup("Editor");

	if (ImGui::BeginPopup("Editor")) {
		changed |= ::Editor("Flags", value.flags);
		changed |= ComboEnum("Type", value.type, keyframeTypeNames);
		changed |= ::Editor("Derivative In", value.in);
		changed |= ::Editor("Derivative Out", value.out);
		ImGui::EndPopup();
	}

	if (hovered || held)
		ImGui::SetTooltip("Time: %f\nIntensity: %f", time, intensity);
	ImGui::PopID();

	return changed;
}

static constexpr int POINTS_PER_SEGMENT = 20;

// TODO: Implement motor pre and post types(?) @ 0x140F26A10 - rangers
static ImPlotPoint GeneratePlotLine(int i, void* userData) {
	auto& motor = *(VibrationMotor*)userData;

	auto segment = i / POINTS_PER_SEGMENT;
	auto point = i % POINTS_PER_SEGMENT;
	auto& kf = motor.keyframes[segment];
	auto& nextKf = motor.keyframes[segment + 1];

	auto t = static_cast<double>(point) / static_cast<double>(POINTS_PER_SEGMENT - 1);
	auto dx = static_cast<double>(nextKf.time - kf.time);

	switch (kf.type) {
	case ucsl::resources::vibration::v21::KeyframeType::CONSTANT: {
		return ImPlotPoint{ kf.time + dx * t, kf.intensity };
	}
	case ucsl::resources::vibration::v21::KeyframeType::LINEAR: {
		return ImPlotPoint{ kf.time + dx * t, kf.intensity + (nextKf.intensity - kf.intensity) * t };
	}
	case ucsl::resources::vibration::v21::KeyframeType::HERMITE: {
		float derivativeOut = 0;
		if (fabs(kf.out[0]) > 0.000001)
			derivativeOut = kf.out[1] / kf.out[0];

		float derivativeIn = 0;
		if (fabs(nextKf.in[0]) > 0.000001)
			derivativeIn = nextKf.in[1] / nextKf.in[0];

		return ImPlotPoint{ kf.time + dx * t,
			kf.intensity * (1.0 + 2.0 * t) * (1.0 - t) * (1.0 - t) +
			derivativeOut * (dx * t) * (1.0 - t) * (1.0 - t) +
			nextKf.intensity * (1.0 + 2.0 * (1.0 - t)) * t * t +
			derivativeIn * (dx * (t - 1.0)) * t * t };
	}
	}

	return ImPlotPoint{ 0, 0 };
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
		if (ImPlot::BeginPlot("##Track", ImVec2(450, 250), ImPlotFlags_CanvasOnly)) {
			ImPlot::SetupAxis(ImAxis_X1, "Time");
			ImPlot::SetupAxis(ImAxis_Y1, "Amplitude");

			if (value.keyframeCount > 1) {
				ImPlot::SetNextFillStyle(color, 0.3f);
				ImPlot::PlotLineG("X", GeneratePlotLine, &value, (value.keyframeCount - 1) * POINTS_PER_SEGMENT + 3, ImPlotLineFlags_Shaded);
			}
			
			for (unsigned int l = 0; l < value.keyframeCount; l++) changed |= Editor(value.keyframes[l], l);
			ImPlot::EndPlot();
		}

		ImGui::TreePop();
	}
	ImGui::PopID();

	return changed;
}

static hh::hid::VibrationContainer::VibrationObj vibTestObj;
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
		if (ImGui::Selectable("Play Vibration")) {
			if (auto* vibrationManager = hh::hid::DeviceManagerWin32::GetInstance()->vibrationManager)
				vibrationManager->PlayVibration(0, value.name, hh::hid::VibrationContainer::VibrationType::ONESHOT, &vibTestObj);
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
