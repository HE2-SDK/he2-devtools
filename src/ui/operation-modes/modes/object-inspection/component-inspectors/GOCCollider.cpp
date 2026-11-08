#include "GOCCollider.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::physics::GOCCollider& component)
{
	ImGui::SeparatorText("Collider information");
	ImGui::Text("Shape type: %d", component.shapeType);
	ImGui::Text("Filter category: %d", component.filterCategory);

	bool enabled = component.flags.test(hh::physics::GOCCollider::Flag::ENABLED);
	if (Editor("Enabled", enabled))
		component.SetEnabled(enabled);

#ifndef DEVTOOLS_TARGET_SDK_wars
	ImGui::SeparatorText("Local world position (editable form)");
	ImGui::BeginGroup();
	Editor("Local world position", component.localWorldPosition);
	Editor("Scale", component.scale);
	ImGui::EndGroup();

	if (ImGui::IsItemEdited()) {
		component.frame->MarkUpdate();
		component.flags.set(hh::physics::GOCCollider::Flag::FRAME_DIRTY);
	}
#endif

#ifndef DEVTOOLS_TARGET_SDK_wars
	ImGui::SeparatorText("Local world position (internal representation)");
	Viewer("Local world position", component.localWorldPosition);

	ImGui::SeparatorText("Transformed world position");
	Viewer("Transformed world position", component.transformedWorldPosition);
#endif

	if (component.frame)
		Viewer("HFrame 1", *component.frame);
	if (component.frame2)
		Viewer("HFrame 2", *component.frame2);
}

void RenderComponentInspector(hh::physics::GOCSphereCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCBoxCollider& component)
{
	ImGui::SeparatorText("Parameters");
	if (BeginVectorViewerTable("Dimensions", false))
	{
		Viewer("Dimensions", component.dimensions);
		EndVectorViewerTable();
	}
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCCapsuleCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.halfHeight);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCCylinderCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.halfHeight);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCMeshCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Resource name: %s", component.meshResource->GetName());
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCHeightFieldCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Resource name: %s", component.resource->GetName());
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}
