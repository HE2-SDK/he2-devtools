#include "GOCActivator.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::game::GOCActivator& component) {
	Editor("Range", component.range);
	Editor("Distance", component.distance);
	Editor("unk101", component.unk101);
	Viewer("Position", component.position);
	Viewer("Range + distance squared", component.rangePlusDistanceSquared);
	Editor("Enabled", component.enabled);
}
