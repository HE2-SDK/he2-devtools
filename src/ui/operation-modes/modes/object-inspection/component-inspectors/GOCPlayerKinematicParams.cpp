#include "GOCPlayerKinematicParams.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
static const char* layerTypeNames[]{
	"NONE",
	"SOLID",
	"LIQUID",
	"THROUGH",
	"CAMERA",
	"SOLID_ONEWAY",
	"SOLID_THROUGH",
	"SOLID_TINY",
	"SOLID_DETAIL",
	"LEAF",
	"LAND",
	"RAYBLOCK",
	"EVENT",
	"RESERVED13",
	"RESERVED14",
	"PLAYER",
	"ENEMY",
	"ENEMY_BODY",
	"GIMMICK",
	"DYNAMICS",
	"RING",
	"CHARACTER_CONTROL",
	"PLAYER_ONLY",
	"DYNAMICS_THROUGH",
	"ENEMY_ONLY",
	"SENSOR_PLAYER",
	"SENSOR_RING",
	"SENSOR_GIMMICK",
	"SENSOR_LAND",
	"SENSOR_ALL",
	"RESERVED30",
	"RESERVED31"
};

static const char* materialTypeNames[]{
	"NONE",
	"STONE",
	"EARTH",
	"WOOD",
	"GRASS",
	"IRON",
	"SAND",
	"LAVA",
	"GLASS",
	"SNOW",
	"NO_ENTRY",
	"ICE",
	"WATER",
	"SEA",
	"DAMAGE",
	"DEAD",
	"FLOWER0",
	"FLOWER1",
	"FLOWER2",
	"AIR",
	"DEADLEAVES",
	"WIREMESH",
	"DEAD_ANYDIR",
	"DAMAGE_THROUGH",
	"DRY_GRASS",
	"RELIC",
	"GIANT",
	"GRAVEL",
	"MUD_WATER",
	"SAND2",
	"SAND3"
};

void Editor(hh::physics::GOCCollider::CollisionInfo& collisionInfo) {
	ComboEnum("Layer", collisionInfo.layer, layerTypeNames);
	Editor("unk0", collisionInfo.unk0);
	Editor("unk1", collisionInfo.unk1);
	ComboEnum("Material", collisionInfo.material, materialTypeNames);
}

void Editor(const char* name, hh::physics::GOCCollider::CollisionInfo& collisionInfo) {
	if (ImGui::TreeNode(name)) {
		Editor(collisionInfo);
		ImGui::TreePop();
	}
}

void Editor(app::player::GOCPlayerKinematicParams::GroundInfo& groundInfo) {
	Editor("Position", groundInfo.position);
	Editor("Ground Normal", groundInfo.groundNormal);
	Editor("Altitude", groundInfo.altitude);
	Editor("unk3a", groundInfo.unk3a);
	Editor("Collision Info", groundInfo.collisionInfo);
	if (*groundInfo.collider)
		Viewer("Collider owner name", groundInfo.collider->owner->name.c_str());
	Editor("unk6", groundInfo.unk6);
	Editor("unk7", groundInfo.unk7);
	CheckboxFlags("Has Ground", groundInfo.flags, app::player::GOCPlayerKinematicParams::GroundInfo::Flag::HAS_GROUND);
	CheckboxFlags("Is Grounded", groundInfo.flags, app::player::GOCPlayerKinematicParams::GroundInfo::Flag::IS_GROUNDED);
}

void Editor(const char* name, app::player::GOCPlayerKinematicParams::GroundInfo& groundInfo) {
	if (ImGui::TreeNode(name)) {
		Editor(groundInfo);
		ImGui::TreePop();
	}
}

void Editor(app::player::GOCPlayerKinematicParams::LiquidInfo& liquidInfo) {
	Editor("Position", liquidInfo.position);
	Editor("Up Vector", liquidInfo.upVector);
	Editor("Collision Info", liquidInfo.collisionInfo);
	Editor("Distance to Surface", liquidInfo.distanceToSurface);
	Editor("Underwater", liquidInfo.underwater);
}

void Editor(const char* name, app::player::GOCPlayerKinematicParams::LiquidInfo& liquidInfo) {
	if (ImGui::TreeNode(name)) {
		Editor(liquidInfo);
		ImGui::TreePop();
	}
}
#endif

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
#ifdef DEVTOOLS_TARGET_SDK_rangers
	Editor("Input Direction", component.inputDirection);
	Editor("Last Input Direction", component.lastInputDirection);
	ImGui::Text("World Transform");
	Viewer("World Transform", component.worldTransform);
	Editor("Ground Info", component.groundInfo0);
	Editor("Final Ground Info", component.groundInfo1);
	Editor("Liquid Info", component.liquidInfo);
#else
	Editor("Unk5", component.unk5);
	Editor("Unk6", component.unk6);
	ImGui::Text("Unk7");
	Viewer("Unk7", component.unk7);
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
	Editor("Unk9.unk1", component.unk9.unk1);
	Editor("Unk9.unk2", component.unk9.unk2);
#endif
	Editor("Unk11.unk3", component.unk11.unk3);
	Editor("Unk14.unk3", component.unk14.unk3);
	Editor("Unk14.unk102", component.unk14.unk102);
	Editor("Unk14.unk103", component.unk14.unk103);
	Editor("Unk15.unk3", component.unk15.unk3);
	Editor("Unk15.unk102", component.unk15.unk102);
	Editor("Unk15.unk103", component.unk15.unk103);

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
