#include "InstanceInspector.h"
#include "Behaviors.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Pointcloud.h>

namespace ui::operation_modes::modes::pointcloud_editor {
	void InstanceInspector::RenderPanel()
	{
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
		if (selection.size() == 0)
			ImGui::Text("Select a instance in the left pane.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple instances selected");
		else
			if (Editor("Instance", *selection[0], GetContext().resource))
				Dispatch(SelectionTransformationBehavior<Context>::SelectionTransformChangedAction{});
	}

	PanelTraits InstanceInspector::GetPanelTraits() const
	{
		return { "Instance inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}
}
