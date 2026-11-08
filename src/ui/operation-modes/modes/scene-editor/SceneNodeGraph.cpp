#include "SceneNodeGraph.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>
#include <ui/Action.h>
#include "Behaviors.h"
#include "Context.h"

namespace ui::operation_modes::modes::scene_editor {
	static constexpr const char* resourceTypeNames[]{
		"Null",
		"Model",
		"Terrain Model",
		"Terrain Instance Info",
		"Effect",
		"Skeleton",
		"Animation",
		"Material Animation",
		"Pattern Animation",
		"UV Animation",
		"Visibility Animation",
		"Camera Animation",
		"Light Animation",
		"Light",
		"External"
	};

	TreeViewNode<SceneNode> SceneNodeGraph::BuildTreeNode(ucsl::resources::scene::v106::SceneData* sceneData, ucsl::resources::scene::v106::SceneGraphNode* graphNode)
	{
		TreeViewNode<SceneNode> node{ GetAllocator(), { *this, graphNode, sceneData } };

		for (auto x = 0; x < sceneData->sceneGraphCount; x++) {
			auto& cnode = sceneData->sceneGraphNodes[x];
			if (cnode.parentIndex == graphNode->index)
				node.AddChild(BuildTreeNode(sceneData, &cnode));
		}

		return node;
	}

	TreeViewNode<SceneNode> SceneNodeGraph::BuildTreeNode(ucsl::resources::scene::v106::SceneData* sceneData)
	{
		TreeViewNode<SceneNode> node{ GetAllocator(), { *this, nullptr, sceneData } };

		for (auto x = 0; x < sceneData->sceneGraphCount; x++) {
			auto& cnode = sceneData->sceneGraphNodes[x];
			if (cnode.parentIndex == -1)
				node.AddChild(BuildTreeNode(sceneData, &cnode));
		}

		return node;
	}

	void SceneNodeGraph::ProcessAction(const ActionBase& action)
	{
		switch (action.id) {
		case SceneChangedAction::id:
			InvalidateTree();
			break;
		}
	}

	void SceneNodeGraph::RenderPanel()
	{
		auto& context = GetContext();

		if (dirty)
			RebuildTree();

		if (&*treeView)
			treeView->Render("Content");
	}

	PanelTraits SceneNodeGraph::GetPanelTraits() const
	{
		return { "Scene Node Graph", ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}

	void SceneNodeGraph::RebuildTree() {
		dirty = false;
		treeView = nullptr;

		auto* sceneData = (ucsl::resources::scene::v106::SceneData*)GetContext().sceneCtrl->GetResource()->unpackedBinaryData;

		treeView = new (GetAllocator()) TreeView<SceneNode>(GetAllocator(), BuildTreeNode(sceneData));
	}

	SceneNode::SceneNode(SceneNodeGraph& graph, ucsl::resources::scene::v106::SceneGraphNode* graphNode, ucsl::resources::scene::v106::SceneData* sceneData) 
		: graph{ graph }, graphNode { graphNode }, sceneData{ sceneData }
	{
		if (!graphNode) return;

		node = &sceneData->sceneNodes[graphNode->index];
	}

	SceneNode::SceneNode(SceneNode&& other) : graph{ other.graph } {
		graphNode = other.graphNode;
		node = other.node;
		sceneData = other.sceneData;
	}

	const void* SceneNode::GetID() const {
		return graphNode;
	}

	const char* SceneNode::GetLabel() const {
		char buffer[0x100];
		snprintf(buffer, sizeof(buffer), "%s - %s", node->nodeName, resourceTypeNames[(int)node->resourceType]);
		return buffer;
	}

	bool SceneNode::MatchesSearchString(const char* searchString) const
	{
		return strstr(GetLabel(), searchString);
	}

	bool SceneNode::IsSelected() const {
		auto* selectionBehavior = graph.GetBehavior<SelectionBehavior<Context>>();
		csl::ut::MoveArray<Selection> selection = selectionBehavior->GetSelection();

		if (selection.size() == 0)
			return false;

		return selection[0].type == Selection::Type::SCENE_NODE && selection[0].sceneNode.graphNode == graphNode;
	}

	void SceneNode::PreRender() const {

	}

	void SceneNode::PostRender() const {
		if (!graphNode) return;

		auto* selectionBehavior = graph.GetBehavior<SelectionBehavior<Context>>();

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
			Selection selection{ sceneData, graphNode };
			selectionBehavior->Select(selection);
		}
	}
}
