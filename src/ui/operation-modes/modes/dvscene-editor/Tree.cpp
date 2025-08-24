#include "Tree.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::dvscene_editor {
    void Tree::RenderNode(dv::DvNode* node){
		auto& ctx = GetContext();
		if (!ctx.GetRuntimeNode(node))
			return;
		ImGui::PushID(node);

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		DvNode selection{ node, GetContext() };

        auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		char nodeName[150];
		const char* type = GetNodeName(node->category);

		if (static_cast<hh::dv::DvNodeBase::NodeType>(node->category) == hh::dv::DvNodeBase::NodeType::ELEMENT) {
			unsigned int elemId = static_cast<unsigned int>(reinterpret_cast<hh::dv::DvNodeElement::Description<hh::dv::DvElementBase>*>(node->data)->elementId);
			type = GetElementName(elemId);
		}

		snprintf(nodeName, sizeof(nodeName), "%s (%s) - %zx", node->name, type, (size_t)node);

		bool isOpen = false;

		if(node->childNodes.size() == 0)
			ImGui::TreeNodeEx(nodeName, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		else
			isOpen = ImGui::TreeNodeEx(nodeName, nodeFlags);;

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);
        
		if (ImGui::BeginPopupContextItem("Options")) {
			if (ImGui::TreeNode("Add Node")) {
				for (auto& category : categorizedNodes) {
					if (ImGui::TreeNode(category.name)) {
						for (auto& cnode : category.items) {
							if (cnode != std::pair<unsigned int, bool>{}) {
								const char* nodeName = GetNodeName(cnode.first);
								unsigned int nodeType = cnode.first;
								unsigned int elementId = 0;
								if (cnode.second) {
									nodeType = static_cast<unsigned int>(hh::dv::DvNodeBase::NodeType::ELEMENT);
									elementId = cnode.first;
									nodeName = GetElementName(elementId);
								}
								if (ImGui::Selectable(nodeName)) {
									char* name = static_cast<char*>(hh::fnd::MemoryRouter::GetTempAllocator()->Alloc(strlen(nodeName) + 6, 1));

									strcpy(name, nodeName);
									srand(time(nullptr));
									int nodeNameLength = strlen(nodeName);
									for (int i = 0; i < 5; ++i)
										name[nodeNameLength + i] = '0' + (rand() % 10);

									name[nodeNameLength + 5] = '\0';
									auto newFileNode = ctx.CreateNode(name, nodeType, elementId);
									ctx.ParentNode(*node, newFileNode);

									auto* runtimeParent = ctx.GetRuntimeNode(node);
									auto* newNode = ctx.CreateNode(name, nodeType, elementId, runtimeParent);
									memcpy(newNode->guid, &newFileNode.guid, 16);
									ctx.ParentNode(runtimeParent, newNode);

									hh::fnd::MemoryRouter::GetTempAllocator()->Free(name);
								}
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			if(selection.fileNode != ctx.parsedScene->dvCommon->node)
				if (ImGui::Selectable("Delete Node")) {
					selection.DeleteNode();
					selectionBehavior->DeselectAll();
				}
			ImGui::EndPopup();
		}

        if(isOpen){
			for (auto& x : node->childNodes)
				RenderNode(&x);
            ImGui::TreePop();
        }

		ImGui::PopID();
    }

	void Tree::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr){
            ImGui::Text("No DvSceneControl selected");
			return;
		}
        
        RenderNode(context.parsedScene->dvCommon->node);
	}

	PanelTraits Tree::GetPanelTraits() const
	{
		return { "Node Tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}
}
