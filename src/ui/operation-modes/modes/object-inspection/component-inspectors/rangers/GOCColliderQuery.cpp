#include <ui/operation-modes/modes/object-inspection/component-inspectors/GOCColliderQuery.h>
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(app::physics::GOCColliderQuery& component)
{
	Editor("Local transform", component.localTransform);
	Editor("World transform", component.worldTransform);
	CheckboxFlags("Enter Flag", component.ownerOverlapFlags, app::physics::GOCColliderQuery::OverlapFlag::ENTER);
	CheckboxFlags("Leave Flag", component.ownerOverlapFlags, app::physics::GOCColliderQuery::OverlapFlag::LEAVE);
	CheckboxFlags("Stay Flag", component.ownerOverlapFlags, app::physics::GOCColliderQuery::OverlapFlag::STAY);
	CheckboxFlags("Enabled", component.flags, app::physics::GOCColliderQuery::Flag::ENABLED);
}

void RenderComponentInspector(app::physics::GOCMoveSphereColliderQuery& component)
{
	RenderComponentInspector(static_cast<app::physics::GOCColliderQuery&>(component));

	Editor("Position", component.position);
	Editor("Velocity", component.velocity);
	Editor("Radius", component.radius);
	Editor("unk204", component.unk204);
}
