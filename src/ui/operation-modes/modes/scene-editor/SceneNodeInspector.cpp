#include "SceneNodeInspector.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>
#include <ui/Action.h>
#include "Behaviors.h"

namespace ui::operation_modes::modes::scene_editor {
	void SceneNodeInspector::RenderPanel()
	{
		auto& context = GetContext();

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		csl::ut::MoveArray<Selection> selection = selectionBehavior->GetSelection();

		if (selection.size() == 0)
			return;

		if (selection[0].type != Selection::Type::SCENE_NODE)
			return;

		auto* sceneNode = selection[0].sceneNode.node;
		auto* resource = context.sceneCtrl->GetResource();

		InputText("Node Name", sceneNode->nodeName, resource);
		if (sceneNode->resourceName)
			InputText("Resource Name", sceneNode->resourceName, resource);
		if (sceneNode->resourceDirectory)
			InputText("Extra Resource Name", sceneNode->resourceDirectory, resource);

		Editor("Position", sceneNode->position);
		Editor("Rotation", sceneNode->rotation);
		Editor("Scale", sceneNode->scale);
	}

	PanelTraits SceneNodeInspector::GetPanelTraits() const
	{
		return { "Scene Node Inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}
}
