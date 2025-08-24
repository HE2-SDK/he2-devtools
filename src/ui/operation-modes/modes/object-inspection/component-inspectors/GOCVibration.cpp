#include "GOCVibration.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::game::GOCVibration& component) {
	if (component.hFrame)
		Viewer("HFrame", *component.hFrame);
	if (ImGui::TreeNode("Vibration Objects")) {
		for (auto& obj : component.vibrationObjs) {
			ImGui::PushID(&obj.container);
			char buffer[16];
			snprintf(buffer, sizeof(buffer), "%d", obj.handle);
			if (ImGui::TreeNode(buffer)) {
				if (ImGui::TreeNode("Data Containers")) {
					for (auto* container : obj.container->dataContainers) {
						ImGui::PushID(container);
						if (ImGui::TreeNode(container->vibData->name)) {
							Editor("Unk2", container->unk2);
							Editor("Max Position X", container->maxPositionX);
							Editor("Unk4", container->unk4);
							Editor("Unk6", container->unk6);
							Editor("Unk7", container->unk7);
							Editor("Flags", container->flags.bits);
							Editor("Unk9", container->unk9);
							Editor("Unk10", container->unk10);
							Editor("Unk11", container->unk11);
							ImGui::TreePop();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				Editor("World Matrix", obj.worldMatrix);
				Editor("Duration", obj.duration);
				Editor("Timer", obj.timer);
				Editor("Easeout Time", obj.easeoutTime);
				Editor("Easeout Timer", obj.easeoutTimer);
				Editor("ID", obj.id);
				Editor("Distance", obj.distance);
				CheckboxFlags("Time Attenuation", obj.flags, hh::game::GOCVibration::VibrationObj::Flags::TIME_ATTENTUATION);
				CheckboxFlags("Distance Attenuation", obj.flags, hh::game::GOCVibration::VibrationObj::Flags::DISTANCE_ATTENTUATION);
				CheckboxFlags("Easeout", obj.flags, hh::game::GOCVibration::VibrationObj::Flags::EASEOUT);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}
