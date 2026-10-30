#include <ui/game-services/game-service-inspectors/LevelInfo.h>

bool Editor(const char* label, app::player::CharacterIdU8& charId);

#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>

static const char* chunkTypeNames[] = { "INITIAL", "AFTER" };
static const char* cyberModeNames[] = { "UNKNOWN", "LOW_GRAVITY", "TIME_EXTEND", "SPEED_SCALE", "NITRO", "MAX_SPEED_CHALLENGE" };

template<typename T>
bool PerRankEditor(const char* label, T (&ranks)[4]) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("S rank", ranks[0]);
		edited |= Editor("A rank", ranks[1]);
		edited |= Editor("B rank", ranks[2]);
		edited |= Editor("C rank", ranks[3]);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::level::StageData& data) {
	bool edited{};

	ImGui::PushID(&data);

	edited |= Editor("Name", data.name);
	edited |= Editor("Stage", data.stage);
	edited |= Editor("Cyber name", data.cyberName);
	edited |= Editor("SceneParam name", data.sceneParamName);
	edited |= Editor("SceneParam stage", data.sceneParamStage);
	edited |= Editor("Gedit resource name", data.geditResourceName);

	int missingIdx{ -1 };
	edited |= DragScalar("World index", data.worldIndex, &missingIdx);
	edited |= DragScalar("Stage index", data.stageIndex, &missingIdx);
	edited |= DragScalar("Cyber stage index", data.cyberStageIndex, &missingIdx);
	edited |= Editor("Default SceneParam index", data.defaultSceneParamIndex);

	if (ImGui::TreeNode("Attributes")) {
		edited |= CheckboxFlags("Unknown", data.attributeFlags, static_cast<app::level::StageData::AttributeFlags>(0));
		edited |= CheckboxFlags("Cyber", data.attributeFlags, app::level::StageData::AttributeFlags::CYBER);
		edited |= CheckboxFlags("Minigame", data.attributeFlags, app::level::StageData::AttributeFlags::MINIGAME);
		edited |= CheckboxFlags("Hacking", data.attributeFlags, app::level::StageData::AttributeFlags::HACKING);
		edited |= CheckboxFlags("Last boss", data.attributeFlags, app::level::StageData::AttributeFlags::LAST_BOSS);
		edited |= CheckboxFlags("Master trial", data.attributeFlags, app::level::StageData::AttributeFlags::MASTER_TRIAL);
		edited |= CheckboxFlags("Tutorial", data.attributeFlags, app::level::StageData::AttributeFlags::TUTORIAL);
		edited |= CheckboxFlags("Navmesh", data.attributeFlags, app::level::StageData::AttributeFlags::NAVMESH);
		edited |= CheckboxFlags("Height field", data.attributeFlags, app::level::StageData::AttributeFlags::HEIGHT_FIELD);
		edited |= CheckboxFlags("Point cloud", data.attributeFlags, app::level::StageData::AttributeFlags::POINT_CLOUD);
		edited |= CheckboxFlags("Autosave", data.attributeFlags, app::level::StageData::AttributeFlags::AUTOSAVE);
		edited |= CheckboxFlags("Diving", data.attributeFlags, app::level::StageData::AttributeFlags::DIVING);
		edited |= CheckboxFlags("Side step", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_STEP);
		edited |= CheckboxFlags("Athletic", data.attributeFlags, app::level::StageData::AttributeFlags::ATHLETIC);
		edited |= CheckboxFlags("Boarding", data.attributeFlags, app::level::StageData::AttributeFlags::BOARDING);
		edited |= CheckboxFlags("Drift", data.attributeFlags, app::level::StageData::AttributeFlags::DRIFT);
		edited |= CheckboxFlags("Side view", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_VIEW);
		edited |= CheckboxFlags("Lava", data.attributeFlags, app::level::StageData::AttributeFlags::LAVA);
		edited |= CheckboxFlags("Sonic", data.attributeFlags, app::level::StageData::AttributeFlags::SONIC);
		edited |= CheckboxFlags("Tails", data.attributeFlags, app::level::StageData::AttributeFlags::TAILS);
		edited |= CheckboxFlags("Amy", data.attributeFlags, app::level::StageData::AttributeFlags::AMY);
		edited |= CheckboxFlags("Knuckles", data.attributeFlags, app::level::StageData::AttributeFlags::KNUCKLES);
		edited |= CheckboxFlags("Battle rush", data.attributeFlags, app::level::StageData::AttributeFlags::BATTLE_RUSH);
		edited |= CheckboxFlags("Boss rush", data.attributeFlags, app::level::StageData::AttributeFlags::BOSS_RUSH);
		edited |= CheckboxFlags("Extra", data.attributeFlags, app::level::StageData::AttributeFlags::EXTRA);
		edited |= CheckboxFlags("Delete fall dead collision", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_FALL_DEAD_COLLISION);
		edited |= CheckboxFlags("Delete air wall collision", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_AIR_WALL_COLLISION);
		edited |= CheckboxFlags("Delete grind rail", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_GRIND_RAIL);
		edited |= CheckboxFlags("Change new collision", data.attributeFlags, app::level::StageData::AttributeFlags::CHANGE_NEW_COLLISION);
		edited |= CheckboxFlags("Restrict debris", data.attributeFlags, app::level::StageData::AttributeFlags::RESTRICT_DEBRIS);
		edited |= CheckboxFlags("Special parry effect", data.attributeFlags, app::level::StageData::AttributeFlags::SPECIAL_PARRY_EFFECT);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mission flags")) {
		edited |= CheckboxFlags("Unknown", data.missionFlags, static_cast<app::level::StageData::MissionFlags>(0));
		edited |= CheckboxFlags("Goal", data.missionFlags, app::level::StageData::MissionFlags::GOAL);
		edited |= CheckboxFlags("Rank", data.missionFlags, app::level::StageData::MissionFlags::RANK);
		edited |= CheckboxFlags("Ring", data.missionFlags, app::level::StageData::MissionFlags::RING);
		edited |= CheckboxFlags("Red ring", data.missionFlags, app::level::StageData::MissionFlags::RED_RING);
		edited |= CheckboxFlags("Number ring", data.missionFlags, app::level::StageData::MissionFlags::NUMBER_RING);
		edited |= CheckboxFlags("Silver moon ring", data.missionFlags, app::level::StageData::MissionFlags::SILVER_MOON_RING);
		edited |= CheckboxFlags("Hide goal", data.missionFlags, app::level::StageData::MissionFlags::HIDE_GOAL);
		edited |= CheckboxFlags("Shadow tails", data.missionFlags, app::level::StageData::MissionFlags::SHADOW_TAILS);
		edited |= CheckboxFlags("Mine", data.missionFlags, app::level::StageData::MissionFlags::MINE);
		edited |= CheckboxFlags("Animal", data.missionFlags, app::level::StageData::MissionFlags::ANIMAL);
		ImGui::TreePop();
	}

	edited |= Editor("Static sectors", data.staticSectors);
	edited |= Editor("Dynamic sectors", data.dynamicSectors);
	edited |= ComboEnum("Chunk type", data.chunkType, chunkTypeNames);
	edited |= Editor("Time limit", data.timeLimit);
	edited |= Editor("Death plane height", data.deathPlaneHeight);
	edited |= Editor("Noise time", data.noiseTime);
	//ImGui::Text("Unk6: %d", data.unk6);
	edited |= PerRankEditor("Red Ring mission thresholds", data.redRingMissionThreshold);

	//ImGui::Text("Unk7: %d", data.unk7);
	edited |= Editor("Ring mission threshold", data.ringMissionThreshold);
	edited |= ComboEnum("Cyber mode", data.cyberMode, cyberModeNames);

	//csl::ut::MoveArray<uint8_t> staticSectors;
	//csl::ut::MoveArray<uint8_t> dynamicSectors;

	if (ImGui::TreeNode("Rank times")) {
		edited |= PerRankEditor("Normal", data.rankTimes);
		edited |= PerRankEditor("Very hard", data.rankTimesVeryHard);
		edited |= PerRankEditor("Challenge", data.rankTimesChallenge);
		edited |= PerRankEditor("Challenge all", data.rankTimesChallengeAll);
		ImGui::TreePop();
	}

	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, app::player::CharacterIdU8& charId) {
	return Editor(label, reinterpret_cast<uint8_t&>(charId));
}

bool Editor(const char* label, app::level::PlayerInformation& playerInfo)
{
	bool edited{};
	ImGui::PushID(label);
	//Unk1<hh::fnd::Handle<hh::fnd::Messenger>> playerObject;
	edited |= Editor("characterId", playerInfo.characterId);
	edited |= Editor("position", playerInfo.position);
	edited |= Editor("vector440", playerInfo.vector440);
	edited |= Editor("rotation", playerInfo.rotation);
	edited |= Editor("dword80", playerInfo.dword80);
	edited |= Editor("vector490", playerInfo.vector490);
	edited |= Editor("vector4B0", playerInfo.vector4B0);
	edited |= Editor("vector4D0", playerInfo.vector4D0);
	edited |= Editor("vector4F0", playerInfo.vector4F0);
	edited |= Editor("vector4110", playerInfo.vector4110);
	edited |= Editor("vector4130", playerInfo.vector4130);
	edited |= Editor("vector4150", playerInfo.vector4150);
	edited |= Editor("vector4170", playerInfo.vector4170);
	edited |= Editor("vector4190", playerInfo.vector4190);
#ifndef DEVTOOLS_TARGET_SDK_rangers
	edited |= Editor("vector41B0", playerInfo.vector41B0);
	edited |= Editor("vector41D0", playerInfo.vector41D0);
	edited |= Editor("vector41F0", playerInfo.vector41F0);
#endif
	edited |= Editor("vector4210", playerInfo.vector4210);
	edited |= Editor("vector4230", playerInfo.vector4230);
	edited |= Editor("vector4250", playerInfo.vector4250);
	edited |= Editor("vector4270", playerInfo.vector4270);
	edited |= Editor("word290", playerInfo.word290);
	//if (playerInfo->qword294.isSet) Editor("qword294", playerInfo->qword294.value);
	//if (playerInfo->qword2A0.isSet) Editor("qword2A0", playerInfo->qword2A0.value);
	edited |= Editor("Boost gauge count", playerInfo.boostGaugeCount);
	edited |= Editor("Stamina", playerInfo.stamina);
	edited |= Editor("dword2BC", playerInfo.dword2BC);
	edited |= Editor("word2C4", playerInfo.word2C4);
	edited |= Editor("dword2C8", playerInfo.dword2C8);
	edited |= Editor("Quick CyLoop amount", playerInfo.quickCyloopAmount);
	edited |= Editor("qword2D8", playerInfo.qword2D8);
	edited |= Editor("dword2E4", playerInfo.dword2E4);
	//uint32_t dword2EC;
	//uint32_t dword2F0;
	edited |= Editor("Ground normal", playerInfo.groundNormal);
	edited |= Editor("Altitude", playerInfo.altitude);
	edited |= Editor("vector4330", playerInfo.vector4330);
	edited |= Editor("dword350", playerInfo.dword350);
	edited |= Editor("dword358", playerInfo.dword358);
	//uint32_t dword360;
	//uint32_t dword364;
	//uint32_t dword368;
	//uint32_t dword36C;
	//uint32_t dword370;
	//uint32_t dword374;
	//uint32_t dword378;
	//uint32_t dword37C;
	//uint32_t dword380;
	//uint32_t dword384;
	//uint32_t dword388;
	//uint32_t dword38C;
	//uint32_t dword390;
	//uint32_t dword394;
	//uint64_t qword398;
	//uint32_t dword3A0;
	//uint32_t dword3A4;
	//uint32_t dword3A8;
	//uint32_t dword3AC;
	//uint32_t dword3B0;
	//uint32_t dword3B4;
	//uint32_t dword3B8;
	//uint32_t dword3BC;
	//if (playerInfo->qword3C0.isSet) Editor("qword3C0", playerInfo->qword3C0.value);
	edited |= Editor("Max rings", playerInfo.maxRings);
	//uint32_t dword3D4;
	//uint32_t dword3D8;
	//if (playerInfo->unk13241.isSet) Editor("unk13241", playerInfo->unk13241.value);
#ifndef DEVTOOLS_TARGET_SDK_rangers
	edited |= Editor("vector4480", playerInfo.vector4480);
#endif
	//if (playerInfo->qword4A0.isSet) Editor("qword4A0", playerInfo->qword4A0.value);
	edited |= Editor("dword4AC", playerInfo.dword4AC);
	edited |= Editor("word4B4", playerInfo.word4B4);
	edited |= Editor("dword4B8", playerInfo.dword4B8);
	edited |= Editor("dword4C0", playerInfo.dword4C0);
	edited |= Editor("transform44F0", playerInfo.transform44F0);
	//Unk4 qword510[3];
	ImGui::PopID();
	return edited;
}
