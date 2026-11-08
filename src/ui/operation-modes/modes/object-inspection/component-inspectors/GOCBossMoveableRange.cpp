#include "GOCBossMoveableRange.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::game::GOCBossMoveableRange& component) {
	Viewer("HFrame", *component.objectHFrame);
	Editor("Object Transform", component.objectTransform);
	Editor("Height Target Node Name", component.heightTargetNodeName);
	Editor("heightMoveSpeed", component.heightMoveSpeed);
	Editor("bossToPlayerDistanceMin", component.bossToPlayerDistanceMin);
	Editor("bossToPlayerDistanceMax", component.bossToPlayerDistanceMax);
	Editor("offsetHeightMin", component.offsetHeightMin);
	Editor("offsetHeightMax", component.offsetHeightMax);
	Editor("areaDistanceMin", component.areaDistanceMin);
	Editor("areaDistanceMax", component.areaDistanceMax);
	Editor("playerSpawnDistance", component.playerSpawnDistance);
	Editor("playerRespawnDistance", component.playerRespawnDistance);
	Editor("playerRespawnDistance2", component.playerRespawnDistance2);
	Editor("unkParams0", component.unkParams0);
	Editor("unkParams1", component.unkParams1);
	Editor("unkParams1b", component.unkParams1b);
	Editor("unk1", component.unk1);
}
