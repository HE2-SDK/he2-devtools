#include "GOCPlayerEffect.h"
bool Editor(const char* label, app::player::PlayerEffect& effect);
bool Editor(const char* label, app::player::EffectList& effectList);
#include <ui/common/editors/Basic.h>

bool Editor(const char* label, app::player::PlayerEffect& effect) {
	bool edited{};
	
	if (ImGui::TreeNode(label)) {
		edited |= Editor("unk1", effect.unk1);
		edited |= Editor("unk2", effect.unk2);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::player::EffectList& effectList) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("Effects", effectList.effects);
		edited |= Editor("Loaded", effectList.loaded);
		ImGui::TreePop();
	}

	return edited;
}

void RenderComponentInspector(app::player::GOCPlayerEffect& component) {
	Editor("Effect lists", component.effectLists);
	Editor("unk205", component.unk205);
	Editor("unk206", component.unk206);
}
