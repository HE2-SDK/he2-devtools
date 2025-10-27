#include "GOCPlayerBlackboard.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderBlackboardAmyInspector(app::player::BlackboardAmy& blackboard) {
	ImGui::Text("Unk1: %zx", blackboard.unk1);
	ImGui::Text("Unk2: %x", blackboard.unk2);
}

void RenderBlackboardSpeedInspector(app::player::BlackboardSpeed& blackboard) {
	ImGui::Text("Unk1: %x", blackboard.speed);
	ImGui::Text("Unk2: %x", blackboard.speed2);
}

void RenderBlackboardItemInspector(app::player::BlackboardItem& blackboard) {
	static uint32_t zero{};

	ImGui::DragScalar("Ring capacity", ImGuiDataType_U32, &blackboard.ringCapacity);
	ImGui::SliderScalar("Ring count", ImGuiDataType_U32, &blackboard.ringCount, &zero, &blackboard.ringCapacity);
	ImGui::DragScalar("Unk1", ImGuiDataType_U32, &blackboard.unk1);
	ImGui::DragScalar("unk2", ImGuiDataType_U16, &blackboard.unk2);
	ImGui::DragScalar("unk3", ImGuiDataType_U8, &blackboard.unk3);
	ImGui::DragScalar("unk3b", ImGuiDataType_U8, &blackboard.unk3b);
	ImGui::DragScalar("unk4", ImGuiDataType_U8, &blackboard.unk4);
	ImGui::DragScalar("unk5", ImGuiDataType_U8, &blackboard.unk5);
	ImGui::DragScalar("unk5b", ImGuiDataType_U8, &blackboard.unk5b);
	ImGui::DragScalar("unk5c", ImGuiDataType_U8, &blackboard.unk5c);
	ImGui::DragScalar("unk6", ImGuiDataType_U16, &blackboard.unk6);
	ImGui::DragScalar("unk7", ImGuiDataType_U16, &blackboard.unk7);
	ImGui::DragScalar("unk8", ImGuiDataType_U8, &blackboard.unk8);
}

void RenderBlackboardBattleInspector(app::player::BlackboardBattle& blackboard) {
	ImGui::DragScalar("powerLevel", ImGuiDataType_U8, &blackboard.powerLevel);
	ImGui::DragScalar("defenseLevel", ImGuiDataType_U8, &blackboard.defenseLevel);
	ImGui::DragScalar("unk1", ImGuiDataType_U8, &blackboard.unk1);
	ImGui::DragScalar("unk2", ImGuiDataType_U8, &blackboard.unk2);
	Editor("unk3", blackboard.unk3);
	csl::ut::MoveArray<void*> debugArray1;
	csl::ut::MoveArray<void*> debugArray2;
	csl::ut::MoveArray<void*> unk4;
	ImGui::DragScalar("Unk5", ImGuiDataType_U32, &blackboard.unk5);
	ImGui::DragScalar("unk6a", ImGuiDataType_U8, &blackboard.unk6a);
	ImGui::DragScalar("unk6b", ImGuiDataType_U8, &blackboard.unk6b);
	ImGui::DragScalar("unk7", ImGuiDataType_U8, &blackboard.unk7);
	ImGui::DragScalar("Combo count", ImGuiDataType_U32, &blackboard.comboCount);
	ImGui::DragFloat("Phantom rush amount", &blackboard.phantomRushAmount);
	Editor("Unk9", blackboard.unk9);
	ImGui::DragFloat("Unk10", &blackboard.unk10);
	ImGui::DragFloat("Quick Cyloop amount", &blackboard.quickCyloopAmount);
	ImGui::Text("%zx", blackboard.unkself12);
	ImGui::Text("%zx", blackboard.unk12);
	ImGui::Text("%zx", blackboard.unk13);
	ImGui::Text("%zx", blackboard.unk14);
	ImGui::Text("Unk16: %s", blackboard.unk16.c_str());
	ImGui::DragScalar("flags0", ImGuiDataType_U8, &blackboard.flags0);
	ImGui::DragScalar("flags1", ImGuiDataType_U8, &blackboard.flags1);
}

const char* difficultyNames[]{ "EASY", "NORMAL", "HARD", "EXTREME" };

const char* stateFlagNames[64]{
	"BOOST",
	nullptr,
	nullptr,
	nullptr,
	"AIR_BOOST",
	nullptr,
	"GRIND_JUMP",
	"GRIND",
	"JUMP",
	"DOUBLE_JUMP",
	"BOUNCE_JUMP",
	"FALL",
	"STOMP",
	"DIVING",
	"DIVING_BOOST",
	nullptr,

	nullptr,
	"CYLOOP",
	"CYLOOP_END",
	"DRIFT",
	nullptr,
	nullptr,
	nullptr,
	"HOMING",
	"PARRY",
	"WALL_CLIMB",
	"IDLE",
	nullptr,
	"WALL_CLIMB_IDLE",
	nullptr,
	"BOARDING",
	nullptr,

	nullptr,
	"SPRING_JUMP",
	"SPIN_DASH",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const char* combatFlagNames[128]{
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"SIDE_STEP",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"PARRY",

	"PERFECT_PARRY",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"AIR_TRICK",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	"GRAND_SLAM",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"QUICK_CYLOOP",
	nullptr,
	"PHANTOM_RUSH",
	"CYCLONE_KICK",
	"RECOVERY_SMASH",
	nullptr,
	"SONIC_BOOM",
	"WILD_RUSH",

	"LOOP_KICK",
	"SPIN_SLASH",
	"STOMP_ATTACK",
	nullptr,
	"HOMING_SHOT",
	"CROSS_SLASH",
	nullptr,
	nullptr,
	nullptr,
	"AUTO_COMBO",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"SPIN_DASH",

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const char* worldFlagNames[128]{
	nullptr,
	"KILLED",
	"DAMAGED_OR_REPELLED",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"OUT_OF_CONTROL",
	nullptr,
	nullptr,
	"AUTO_RUN",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"CYBER_SPACE",
	nullptr,

	nullptr,
	nullptr,
	"WALL_JUMP_LAND",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"POWER_BOOST",
	nullptr,
	"AIR_TRICK",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"HEIGHT_MAP_COLLISION",
	"NO_CLIP",
	nullptr,
	"BATTLE",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	"NITRO_BOOST",
	"MAX_SPEED_CHALLENGE",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const char* characterIdNames[] = {
	"SONIC",
	"AMY",
	"KNUCKLES",
	"TAILS",
	"NONE"
};

const char* formStateNames[] = {
	"NORMAL",
	"SUPER",
	"UNK0"
};

void RenderBlackboardStatusInspector(app::player::BlackboardStatus& blackboard) {
	ImGui::DragScalar("byte20", ImGuiDataType_U8, &blackboard.byte20);
	ComboEnum("characterId", blackboard.characterId, characterIdNames);
	ComboEnum("formState", blackboard.formState, formStateNames);

	if (ImGui::TreeNode("Combat flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(combatFlagNames[i] == nullptr ? idxName : combatFlagNames[i], blackboard.combatFlags[0], static_cast<app::player::BlackboardStatus::CombatFlag>(i));
		}
		for (size_t i = 64; i < 128; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(combatFlagNames[i] == nullptr ? idxName : combatFlagNames[i], blackboard.combatFlags[1], static_cast<app::player::BlackboardStatus::CombatFlag>(i - 64));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("State flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(stateFlagNames[i] == nullptr ? idxName : stateFlagNames[i], blackboard.stateFlags, static_cast<app::player::BlackboardStatus::StateFlag>(i));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("World flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(worldFlagNames[i] == nullptr ? idxName : worldFlagNames[i], blackboard.worldFlags[0], static_cast<app::player::BlackboardStatus::WorldFlag>(i));
		}
		for (size_t i = 64; i < 128; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(worldFlagNames[i] == nullptr ? idxName : worldFlagNames[i], blackboard.worldFlags[1], static_cast<app::player::BlackboardStatus::WorldFlag>(i - 64));
		}
		ImGui::TreePop();
	}

	ImGui::Text("Out of control time: %f", blackboard.outOfControlTime);
	ImGui::Text("In control time: %f", blackboard.inControlTime);
	ImGui::Text("%zx", blackboard.qword58);
	ImGui::Text("%zx", blackboard.dword60);
	ImGui::Text("%zx", blackboard.dword64);
	//ut::PriorityList<bool, hh::fnd::Handle<hh::fnd::Messenger>> qword68;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qword90;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qwordC0;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qwordF0;
	Editor("dword120", blackboard.dword120);
	//ut::PriorityList<Dimension, hh::fnd::Handle<hh::fnd::Messenger>> qword130;
	Editor("oword160", blackboard.oword160);
	ImGui::DragScalar("dword170", ImGuiDataType_U32, &blackboard.dword170);
	ImGui::DragScalar("qword174", ImGuiDataType_U32, &blackboard.qword174);
	ImGui::DragScalar("qword178", ImGuiDataType_U32, &blackboard.qword178);
	ImGui::DragScalar("dword17C", ImGuiDataType_U32, &blackboard.dword17C);
	ComboEnum("Difficulty", blackboard.byte180, difficultyNames);
	ImGui::DragScalar("qword184", ImGuiDataType_U32, &blackboard.qword184);
	DragScalar("qword188", blackboard.qword188);
}

void RenderBlackboardTailsInspector(app::player::BlackboardTails& blackboard) {
	ImGui::DragScalar("Unk1", ImGuiDataType_U32, &blackboard.unk1);
	ImGui::DragScalar("Unk2", ImGuiDataType_U32, &blackboard.unk2);
	ImGui::DragScalar("Unk3", ImGuiDataType_U32, &blackboard.unk3);
	ImGui::DragScalar("Unk4", ImGuiDataType_U32, &blackboard.unk4);
	ImGui::DragScalar("Unk5", ImGuiDataType_U32, &blackboard.unk5);
	ImGui::DragScalar("Unk6", ImGuiDataType_U32, &blackboard.unk6);
}

const char* GetBlackboardName(unsigned int nameHash) {
	switch (nameHash) {
		case csl::ut::HashString("BlackboardAmy"): return "BlackboardAmy";
		case csl::ut::HashString("BlackboardSpeed"): return "BlackboardSpeed";
		case csl::ut::HashString("BlackboardItem"): return "BlackboardItem";
		case csl::ut::HashString("BlackboardBattle"): return "BlackboardBattle";
		case csl::ut::HashString("BlackboardStatus"): return "BlackboardStatus";
		case csl::ut::HashString("BlackboardTails"): return "BlackboardTails";
		default: return "Unknown";
	}
}
void RenderComponentInspector(app::player::GOCPlayerBlackboard& component)
{
	for (auto& content : component.blackboard->contents) {
		if (ImGui::TreeNode(&*content, "%s", GetBlackboardName(content->GetNameHash()))) {
			switch (content->GetNameHash()) {
			case csl::ut::HashString("BlackboardAmy"): RenderBlackboardAmyInspector(static_cast<app::player::BlackboardAmy&>(*content)); break;
			case csl::ut::HashString("BlackboardSpeed"): RenderBlackboardSpeedInspector(static_cast<app::player::BlackboardSpeed&>(*content)); break;
			case csl::ut::HashString("BlackboardItem"): RenderBlackboardItemInspector(static_cast<app::player::BlackboardItem&>(*content)); break;
			case csl::ut::HashString("BlackboardBattle"): RenderBlackboardBattleInspector(static_cast<app::player::BlackboardBattle&>(*content)); break;
			case csl::ut::HashString("BlackboardStatus"): RenderBlackboardStatusInspector(static_cast<app::player::BlackboardStatus&>(*content)); break;
			case csl::ut::HashString("BlackboardTails"): RenderBlackboardTailsInspector(static_cast<app::player::BlackboardTails&>(*content)); break;
			}
			ImGui::TreePop();
		}
	}
}
