#include "GOCSprite.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::ui;

void RenderComponentInspector(GOCSprite& component) {
	CheckboxFlags("Enabled", component.flags, GOCSprite::Flag::ENABLED);
}
