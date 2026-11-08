#include "GOCHealth.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void Editor(app::game::GOCHealth::DamageEffectManager& effectMgr) {
	Editor("damageDealt", effectMgr.damageDealt);
	Editor("damageThreshold", effectMgr.damageThreshold);
	Editor("damageDecay", effectMgr.damageDecay);
	Editor("checkInterval", effectMgr.checkInterval);
	Editor("timeUntilNextCheck", effectMgr.timeUntilNextCheck);
}

void Editor(app::game::GOCHealth::Unk2& value) {
	Editor("csl__math__vector3160", value.csl__math__vector3160);
	Editor("dword170", value.dword170);
	Editor("qword178", value.qword178);
	Editor("word180", value.word180);
	Editor("byte182", value.byte182);
}

void Editor(app::game::GOCHealth::Unk3& value) {
	Editor("byte150", value.byte150);
	if (ImGui::TreeNode("byte160")) {
		Editor(value.byte160);
		ImGui::TreePop();
	}
}

void Editor(app::game::GOCHealth::Unk4& value) {
	Editor("byte190", value.byte190);
	Editor("qword198", value.qword198);
	Editor("multipliers", value.multipliers);
	Editor("float1B4", value.float1B4);
}

void Editor(app::game::GOCHealth::Unk5& value) {
	Editor("byte1B8", value.byte1B8);
	Editor("dword1BC", value.dword1BC);
	Editor("byte1C0", value.byte1C0);
}

static const char* debuggingTypeNames[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
	"UNK4",
	"UNK5"
};

void Editor(app::game::GOCHealth::DebuffingInfo& value) {
	ComboEnum("debuffingType", value.debuffingType, debuggingTypeNames);
	Editor("debuffingDamage", value.debuffingDamage);
	Editor("qword1D8", value.qword1D8);
	Editor("timeUntilDebuffingEnds", value.timeUntilDebuffingEnds);
	Editor("timeUntilNextDebuffingTick", value.timeUntilNextDebuffingTick);
	Editor("debuffingTickInterval", value.debuffingTickInterval);
	Editor("debuffingDirection", value.debuffingDirection);
}

static const char* deathActionNames[]{
	"SHUTDOWN",
	"KILL",
	"RESPAWN"
};

void RenderComponentInspector(app::game::GOCHealth& component) {
	Editor("currentHealth", component.currentHealth);
	Editor("maxHealth", component.maxHealth);
	if (ImGui::TreeNode("stunEffectManager")) {
		Editor(component.damageEffectManagers[0]);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("staggerEffectManager")) {
		Editor(component.damageEffectManagers[1]);
		ImGui::TreePop();
	}
	Editor("damageMultiplier", component.damageMultiplier);
	ComboEnum("deathAction", component.deathAction, deathActionNames);
	Editor("respawnTime", component.respawnTime);
	if (ImGui::TreeNode("unk150")) {
		Editor(component.unk150);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("unk190")) {
		Editor(component.unk190);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("unk1B8")) {
		Editor(component.unk1B8);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("debuffingInfo")) {
		Editor(component.debuffingInfo);
		ImGui::TreePop();
	}
	Editor("byte200", component.byte200);
	Editor("dword204", component.dword204);
}
