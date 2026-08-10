#pragma once
#include <ui/operation-modes/Panel.h>
#include <ui/common/TreeView.h>
#include "Context.h"

namespace ui::operation_modes::modes::scene_editor {
	class SceneNodeGraph;
	struct SceneNode {
		ucsl::resources::scene::v106::SceneData* sceneData;
		ucsl::resources::scene::v106::SceneNode* node;
		ucsl::resources::scene::v106::SceneGraphNode* graphNode;
		SceneNodeGraph& graph;

		const void* GetID() const;
		const char* GetLabel() const;
		bool MatchesSearchString(const char* searchString) const;
		bool IsSelected() const;
		void PreRender() const;
		void PostRender() const;
		SceneNode(SceneNodeGraph& graph, ucsl::resources::scene::v106::SceneGraphNode* graphNode, ucsl::resources::scene::v106::SceneData* sceneData);
		SceneNode(SceneNode&& other);
	};

	class SceneNodeGraph : public Panel<Context> {
	public:
		friend struct SceneNode;

		using Panel::Panel;

		hh::fnd::Reference<TreeView<SceneNode>> treeView;
		bool dirty{ false };

		TreeViewNode<SceneNode> BuildTreeNode(ucsl::resources::scene::v106::SceneData* sceneData, ucsl::resources::scene::v106::SceneGraphNode* graphNode);
		TreeViewNode<SceneNode> BuildTreeNode(ucsl::resources::scene::v106::SceneData* sceneData);

		virtual void ProcessAction(const ActionBase& action) override;
		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		inline void InvalidateTree() {
			dirty = true;
		}
		void RebuildTree();
	};
}
