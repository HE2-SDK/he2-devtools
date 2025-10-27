#include "GOCPlayerKinematicParams.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::player::GOCPlayerKinematicParams& component)
{
	Editor("Transform", component.transform);
	ImGui::PushID("WorldPos");
	ImGui::SeparatorText("World position");
	ImGui::PopID();
	Editor("World position", component.worldPosition);
	ImGui::SeparatorText("Physics");
	Editor("Velocity", component.velocity);
	Editor("Unk1", component.unk1);
	Editor("Unk2", component.unk2);
	Editor("Unk3", component.unk3);
	Editor("Unk4", component.unk4);
	Editor("Unk5", component.unk5);
	Editor("Unk6", component.unk6);
	ImGui::Text("Unk7");
	Viewer("Unk7", component.unk7);

#ifdef DEVTOOLS_TARGET_SDK_miller
	Editor("Unk9.unk1", component.unk9.unk1);
	Editor("Unk9.unk2", component.unk9.unk2);
#endif
	Editor("Unk11.unk3", component.unk11.unk3);
	Editor("Unk14.unk1", component.unk14.unk1);
	Editor("Unk14.unk2", component.unk14.unk2);
	Editor("Unk14.unk3", component.unk14.unk3);
	Editor("Unk14.unk102", component.unk14.unk102);
	Editor("Unk14.unk103", component.unk14.unk103);
	Editor("Unk15.unk1", component.unk15.unk1);
	Editor("Unk15.unk2", component.unk15.unk2);
	Editor("Unk15.unk3", component.unk15.unk3);
	Editor("Unk15.unk102", component.unk15.unk102);
	Editor("Unk15.unk103", component.unk15.unk103);
	Editor("Unk16.unk1", component.unk16.unk1);
	Editor("Unk16.unk2", component.unk16.unk2);

	//if (ImGui::TreeNode("Movement flags")) {
	//	for (size_t i = 0; i < 16; i++) {
	//		char idxName[10];
	//		snprintf(idxName, sizeof(idxName), "%zd", i);

	//		CheckboxFlags(idxName, component.unk9.unk7, static_cast<uint16_t>(1 << i));
	//	}
	//	ImGui::TreePop();
	//}

	//if (ImGui::TreeNode("Movement flags 2")) {
	//	for (size_t i = 0; i < 16; i++) {
	//		char idxName[10];
	//		snprintf(idxName, sizeof(idxName), "%zd", i);

	//		CheckboxFlags(idxName, component.unk10.unk7, static_cast<uint16_t>(1 << i));
	//	}
	//	ImGui::TreePop();
	//}
}
