#include "ObjectLibrary.h"
#include "Behaviors.h"
#include "Context.h"
#include "Actions.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/Icons.h>
#include <ui/operation-modes/behaviors/Placement.h>
#include <utilities/ObjectDataUtils.h>
#include <utilities/math/MathUtils.h>

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	void ObjectLibrary::RenderPanel() {
		auto& context = GetContext();

		auto* focusedChunk = context.GetFocusedChunk();
		if (focusedChunk) {
			const char* targetLayerPreview = "<none>";

			for (auto* layer : focusedChunk->GetLayers())
				if (layer == context.placementTargetLayer)
					targetLayerPreview = layer->GetName();

			if (ImGui::BeginCombo("Target placement layer", targetLayerPreview)) {
				for (auto* layer : focusedChunk->GetLayers()) {
					if (ImGui::Selectable(layer->GetName(), layer == context.placementTargetLayer))
						context.placementTargetLayer = layer;
					if (context.placementTargetLayer == layer)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			auto* placement = GetBehavior<PlacementBehavior<Context>>();

			if (!placement->CanPlace())
				ImGui::BeginDisabled();

			if (placement->IsInPlaceMode()) {
				bool clickedStop = ImGui::Button("Stop placing");
				ImGui::SameLine();
				ImGui::Text("Placing %s", context.objectClassToPlace->GetName());
				if (clickedStop)
					placement->SetPlaceMode(false);
			}
			else if (ImGui::Button("Place"))
				placement->SetPlaceMode(true);

			if (!placement->CanPlace())
				ImGui::EndDisabled();

			ImGui::SameLine();
			if (ImGui::Button("Array tool..."))
				Dispatch(OpenArrayToolAction{});
		}

		if (ImGui::BeginChild("List of objects"))
			tree.Render("ObjectLibrary");

		ImGui::EndChild();
	}

	PanelTraits ObjectLibrary::GetPanelTraits() const
	{
		return { "Object library", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	TreeView<ObjectLibraryTreeViewNode> ObjectLibrary::BuildTree()
	{
		auto* registry = GameObjectSystem::GetInstance()->gameObjectRegistry;
		TreeViewNode<ObjectLibraryTreeViewNode> rootNode{ GetAllocator(), { *this, GetAllocator() } };
		rootNode.node.groupLabel = "ROOT";

		for (auto* objClass : registry->GetGameObjectClasses()) {
			TreeViewNode<ObjectLibraryTreeViewNode>* currentNode = &rootNode;
			auto* categoryAttr = static_cast<const char*>(objClass->GetAttributeValue("category"));
			const char* category = categoryAttr == nullptr ? nullptr : categoryAttr;

			while (category != nullptr) {
				const char* divider = strstr(category, "/");
				size_t elementLen = divider == nullptr ? strlen(category) : divider - category;

				csl::ut::VariableString categoryElement{ category, static_cast<int>(elementLen), GetAllocator() };

				TreeViewNode<ObjectLibraryTreeViewNode>* categoryNode{};
				for (auto& child : currentNode->children) {
					if (child.node.type == ObjectLibraryTreeViewNode::Type::GROUP && !strcmp(child.node.groupLabel.c_str(), categoryElement.c_str())) {
						categoryNode = &child;
						break;
					}
				}

				if (categoryNode == nullptr) {
					currentNode->AddChild({ GetAllocator(), { *this, GetAllocator(), std::move(categoryElement) } });
					categoryNode = &currentNode->children[currentNode->children.size() - 1];
				}

				category = divider == nullptr ? nullptr : divider + 1;
				currentNode = categoryNode;
			}

			currentNode->AddChild({ GetAllocator(), { *this, GetAllocator(), objClass } });
		}

		return { GetAllocator(), std::move(rootNode) };
	}

	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, const GameObjectClass* objectClass) : library{ library }, type{ Type::OBJECT_CLASS }, objectClass{ objectClass }, groupLabel{ allocator } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, csl::ut::VariableString&& groupLabel) : library{ library }, type{ Type::GROUP }, groupLabel{ std::move(groupLabel) } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator) : library{ library }, type{ Type::GROUP }, groupLabel{ allocator } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibraryTreeViewNode&& other) : library{ other.library }, type{ other.type }, objectClass{ other.objectClass }, groupLabel{ std::move(other.groupLabel) } {}

	const void* ObjectLibraryTreeViewNode::GetID() const
	{
		switch (type) {
		case Type::OBJECT_CLASS:
			return objectClass;
		case Type::GROUP:
			return groupLabel;
		default:
			return nullptr;
		}
	}

	const char* ObjectLibraryTreeViewNode::GetLabel() const
	{
		switch (type) {
		case Type::OBJECT_CLASS:
			return objectClass->GetName();
		case Type::GROUP:
			return groupLabel.c_str();
		default:
			return nullptr;
		}
	}

	bool ObjectLibraryTreeViewNode::MatchesSearchString(const char* searchString) const
	{
		auto n = strlen(searchString);
		for (auto* h = GetLabel(); *h; h++)
			if (!_strnicmp(h, searchString, n)) return true;
		return false;
	}

	bool ObjectLibraryTreeViewNode::IsSelected() const {
		return type == Type::OBJECT_CLASS && library.GetContext().objectClassToPlace == objectClass;
	}

	void ObjectLibraryTreeViewNode::PreRender() const {
	}

	void ObjectLibraryTreeViewNode::PostRender() const
	{
		if (type == Type::OBJECT_CLASS) {
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("GameObjectClass", &objectClass, sizeof(objectClass));
				ImGui::EndDragDropSource();
			}
			else if (ImGui::IsItemClicked())
				library.Dispatch(SetObjectClassToPlaceAction{ objectClass });
		}
	}
}
