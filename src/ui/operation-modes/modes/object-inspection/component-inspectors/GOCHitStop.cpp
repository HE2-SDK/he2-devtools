#include "GOCHitStop.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::game::GOCHitStop& component) {
	Editor("unk0", component.unk0);
	Editor("unk1", component.unk1);
	Editor("unk2", component.unk2);
	Editor("unk3", component.unk3);
}
