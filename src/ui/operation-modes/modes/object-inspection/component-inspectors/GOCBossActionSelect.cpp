#include "GOCBossActionSelect.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::game::GOCBossActionSelect& component) {
	Editor("currentState", component.currentState);
	Editor("timeSinceLastChange", component.timeSinceLastChange);
	Editor("timeSinceLastAction", component.timeSinceLastAction);
	if (ImGui::TreeNode("unk3")) {
		for (auto x = 0; x < component.unk3.size(); x++) {
			char buffer[8];
			snprintf(buffer, sizeof(buffer), "%d", x);
			if (ImGui::TreeNode(buffer)) {
				auto& unk = component.unk3[x];
				Editor("unk0", unk.unk0);
				Editor("unk1", unk.unk1);
				Editor("unk2", unk.unk2);
				Editor("unk3", unk.unk3);
				Editor("unk4", unk.unk4);
				Editor("unk5", unk.unk5);
				Editor("unk6", unk.unk6);
				Editor("unk7", unk.unk7);
				Editor("unk8", unk.unk8);
				Editor("unk9", unk.unk9);

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	Editor("flags", component.flags);
}
