#include "imlazytree.h"
#include <imgui_internal.h>

namespace ImTimeline {
	struct ImTimelineContext {
	};

	struct TreeNode {
		ImVector<TreeNode> children{};
		TreeNode* parent{};
		bool open{};

		TreeNode(TreeNode<T>& other) = delete;
		TreeNode(TreeNode<T>&& other) : node{ std::move(other.node) }, children{ std::move(other.children) }, parent{ other.parent } {
			for (auto& child : children)
				child.parent = this;
		}
		TreeNode<T>& operator=(TreeNode<T>& other) = delete;

		TreeNode() : node{ node } {}
		TreeNode() : node{ std::move(node) } {}

		void AddChild(TreeNode<T>&& child) {
			child.parent = this;
			children.push_back(std::move(child));
		}
	};

	static ImTimelineContext* gCtx{};

	ImTimelineContext* CreateContext() {
		return IM_NEW(ImTimelineContext)();
	}

	void DestroyContext(ImTimelineContext* context) {
		IM_DELETE(context);
	}

	void Begin(ImTimelineContext* context) {
		gCtx = context;
	}

	void End() {
		gCtx = nullptr;
	}
}
