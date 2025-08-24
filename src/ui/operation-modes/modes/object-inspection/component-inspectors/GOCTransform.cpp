#include "GOCTransform.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <utilities/GameObjectUtils.h>

using namespace hh::game;

void RenderComponentInspector(GOCTransform& component) {
	auto transform = component.GetTransform();

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("GOCTransformTransformEditable", transform)) {
		component.SetLocalTransform(transform);
		EnsureGOCTransformIsUpdating(component);
	}

	//CheckboxFlags("Transform position", component.GetFrame().flags, hh::fnd::HFrame::Flag::TRANSFORM_POSITION);
	//CheckboxFlags("Transform rotation", component.GetFrame().flags, hh::fnd::HFrame::Flag::TRANSFORM_ROTATION);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("GOCTransformTransform", component.GetTransform());

	Viewer("HFrame", component.GetFrame());
}
